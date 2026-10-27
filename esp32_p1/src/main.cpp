#include <Arduino.h>

// 01_blink: 板载 LED 闪烁 (ESP32-C3-DevKitC-02)
// C3 DevKitC-02 的板载 LED 接在 GPIO8，低电平点亮（部分板子是 GPIO3，如果 8 不亮换 3 试试）

#define LED_PIN 8

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("\n=== esp32_p1 blink start ===");
}

void loop() {
  digitalWrite(LED_PIN, LOW);   // C3 板载 LED 低电平点亮
  Serial.println("LED ON");
  delay(500);
  digitalWrite(LED_PIN, HIGH);  // 熄灭
  Serial.println("LED OFF");
  delay(500);
}
