#include <WiFi.h>
#include <HTTPClient.h>

const char* WIFI_SSID = "YCHZHT";
const char* WIFI_PASS="22372222";

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.printf("\n已连接, IP: %s\n", WiFi.localIP().toString().c_str());

  // 宿迁: 纬度33.96 经度118.28 请求明天 weathercode/temp_max/temp_min
  String url = "https://api.open-meteo.com/v1/forecast"
               "?latitude=33.96&longitude=118.28"
               "&daily=weathercode,temperature_2m_max,temperature_2m_min"
               "&timezone=Asia%2FShanghai&forecast_days=2";

  HTTPClient http;
  http.begin(url);
  int code = http.GET();                 // 发 GET 请求，返回 HTTP 状态码
  Serial.printf("HTTP 状态码: %d\n", code);

  if (code == 200) {
    Serial.println("=== 返回内容 ===");
    Serial.println(http.getString());    // JSON 原文
  } else {
    Serial.println("请求失败");
  }
  http.end();                            // 释放连接资源，必写
}

void loop() { }
