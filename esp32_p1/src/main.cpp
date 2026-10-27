#include <Arduino.h>
#include <WiFi.h>

// 05_proxy_bucket v2: 反向连接式代理桶（生产拓扑）
// 背景: 路由器开了 AP 隔离，局域网里其他设备连不进 ESP32；
//       且真实场景中 ECS 在公网也无法主动连入家庭 NAT。
// 所以由 ESP32 主动外连控制端(本机调试=192.168.1.199:9999；生产=ECS:9999)，
// 控制端发一行指令控制行为:
//   "T host:port\n"  -> ESP32 另开一条隧道连接到控制端:9999 报"TREADY"，然后双向泵
// 断线自动重连控制端。

const char *WIFI_SSID = "YCHZHT";
const char *WIFI_PASS = "22372222";

// 控制端（调试=本机 Windows；生产=改 ECS 公网IP）
IPAddress CTRL_HOST(115, 29, 231, 25);
constexpr uint16_t CTRL_PORT = 9999;

#define LED_PIN 8

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.printf("[WiFi] 连接 %s ", WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) { delay(300); Serial.print("."); }
  Serial.printf(" OK IP=%s\n", WiFi.localIP().toString().c_str());
}

void pump(WiFiClient &a, WiFiClient &b) {
  uint8_t buf[1024];
  uint32_t lastData = millis();
  while (a.connected() && b.connected()) {
    bool act = false;
    int n;
    while ((n = a.available()) > 0) {
      int r = a.read(buf, n < 1024 ? n : 1024);
      if (r > 0) { b.write(buf, r); act = true; }
    }
    while ((n = b.available()) > 0) {
      int r = b.read(buf, n < 1024 ? n : 1024);
      if (r > 0) { a.write(buf, r); act = true; }
    }
    if (act) lastData = millis();
    if (millis() - lastData > 60000) break;  // 60s 空闲
    delay(1);
  }
}

// 从控制连接读一行指令
bool readLine(WiFiClient &in, String &line, uint32_t timeoutMs) {
  line = "";
  uint32_t t0 = millis();
  while (millis() - t0 < timeoutMs) {
    while (in.available()) {
      char ch = (char)in.read();
      if (ch == '\n') return true;
      if (ch != '\r') line += ch;
      if (line.length() > 4096) return false;
    }
    if (!in.connected()) return false;
    delay(1);
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  Serial.println("\n=== esp32_p1 proxy bucket v2 (reverse) ===");
  connectWiFi();
}

void loop() {
  // LED: 未连控制端快闪
  if (WiFi.status() != WL_CONNECTED) { WiFi.reconnect(); delay(400); }

  WiFiClient ctrl;
  Serial.printf("[ctrl] 连接控制端 %s:%u ...\n", CTRL_HOST.toString().c_str(), CTRL_PORT);
  if (!ctrl.connect(CTRL_HOST, CTRL_PORT)) {
    Serial.println("[ctrl] 失败, 3s 重试");
    digitalWrite(LED_PIN, HIGH); delay(150); digitalWrite(LED_PIN, LOW);
    delay(2850);
    return;
  }
  ctrl.setNoDelay(true);
  ctrl.printf("HELLO esp32_p1 %s\n", WiFi.localIP().toString().c_str());
  Serial.println("[ctrl] 已连上, 等指令");

  // 指令循环
  while (ctrl.connected()) {
    String cmd;
    if (!readLine(ctrl, cmd, 120000)) {
      if (!ctrl.connected()) break;
      continue;  // 超时继续等
    }
    if (cmd.startsWith("T ")) {
      String hp = cmd.substring(2);
      int colon = hp.lastIndexOf(':');
      if (colon < 0) { ctrl.print("ERR bad target\n"); continue; }
      String host = hp.substring(0, colon);
      uint16_t port = hp.substring(colon + 1).toInt();

      // 开数据隧道: 新连接到控制端, 报 TREADY, 控制端把业务流量接到这条
      WiFiClient data;
      Serial.printf("[bucket] 目标 %s:%u, 开数据隧道...\n", host.c_str(), port);
      if (!data.connect(CTRL_HOST, CTRL_PORT)) { ctrl.print("ERR tunnel fail\n"); continue; }
      data.setNoDelay(true);
      data.print("TREADY\n");

      // 连目标
      WiFiClient remote;
      remote.setNoDelay(true);
      if (!remote.connect(host.c_str(), port)) {
        Serial.println("[bucket] 目标连接失败");
        data.print("TFAIL\n");
        data.stop();
        ctrl.print("DONE fail\n");
        continue;
      }
      ctrl.print("OK\n");
      Serial.println("[bucket] 双向转发中...");
      pump(data, remote);
      Serial.println("[bucket] 结束");
      data.stop(); remote.stop();
    } else if (cmd.startsWith("PING")) {
      ctrl.print("PONG\n");
    } else if (cmd.startsWith("BYE")) {
      break;
    } else if (cmd.length()) {
      ctrl.printf("ECHO %s\n", cmd.c_str());
    }
  }
  Serial.println("[ctrl] 断开");
  ctrl.stop();
}
