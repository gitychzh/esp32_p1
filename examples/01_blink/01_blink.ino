#include <WiFi.h>
void setup() {
  
  pinMode(8,OUTPUT);// put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.print("=== ESP32-C3 串口测试 ===");
  Serial.println(WiFi.macAddress()); //顺便打印网卡mac地址

}

void loop() {
  digitalWrite(8,HIGH);
  Serial.println("LED ON");
  delay(500);
  digitalWrite(8,LOW);
  Serial.println("LED OFF");
  delay(500);
  
  
  // put your main code here, to run repeatedly:

}
