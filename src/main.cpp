#include <Arduino.h>
#include "HardwareSerial.h"
#include <Wire.h>

void setup()
{
  Serial.begin(115200);
  Serial1.setRX(17);    // 標準設定以外の端子を使用する場合（UART0）
  Serial1.setTX(16);
  Serial1.begin(19200);
  while(!Serial1); //wait untill it opens
  Serial.println("start");
}

void loop() {
  while (not Serial1.available()){}
  int hoge = Serial1.read();
  if (hoge == 10){
    Serial1.write(10);
    Serial.println("received 10 and sent 10");
  } else {
    Serial.print(hoge);
    Serial.println(",not 10 was sent");
  }
}
