#include <WiFi.h>
void setup() {
  
  pinMode(8,OUTPUT);// put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  WiFi.scanNetworks();
  int n = WiFi.scanComplete();
  Serial.printf(" 发现 %d 个网络:\n", n ");
  for ( int i = 0; i < n; i++){
    Serial.printf("%2d | %-32s | CH%02d | %d dBm | %s\n",i + 1,WiFi.SSID(i),c_str(),WiFi.channel(i),WiFi.RSSI(i),WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "开放" : "加密");
  }

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
