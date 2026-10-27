#include <WiFi.h>

const char* WIFI_SSID = "YCHZHT";
const char* WIFI_PASS="22372222";

void setup() {
  pinMode(8, OUTPUT);
  Serial.begin(115200);
  delay(1000);

  Serial.printf("\n正在连接 %s", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // 等待连接结果，最多 15 秒
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    digitalWrite(8, !digitalRead(8));   // 连接中 LED 快闪
    Serial.print(".");
    if (millis() - start > 15000) {
      Serial.println("\n连接失败！检查 SSID/密码");
      return;                            // setup 直接返回，停在失败状态
    }
  }

  // 走到这里 = 连上了
  digitalWrite(8, HIGH);                 // LED 常亮 = 已联网（以后靠它一眼判断状态）
  Serial.println("\n=== 连接成功 ===");
  Serial.print("IP 地址:   "); Serial.println(WiFi.localIP());
  Serial.print("信号强度:  "); Serial.print(WiFi.RSSI()); Serial.println(" dBm");
  Serial.print("网关:      "); Serial.println(WiFi.gatewayIP());
}

void loop() { }
