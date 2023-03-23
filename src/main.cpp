#include <Arduino.h>
#include "HardwareSerial.h"
#include <Wire.h>
#include <VL53L1X.h>
#include <VL53L0X.h>
//
// 3桁の7セグメントLEDを光らせるプログラム
//
#define BI_pin 7

VL53L1X vl_left;
VL53L1X vl_right;

VL53L0X sensor;

const int xshut_left = 15;
const int xshut_right = 14;

byte vl_left_address = 0x10; // 左のVLのアドレスは0x10
byte vl_right_address = 0x11; // 左のVLのアドレスは0x10

volatile uint num_3digits = 769;

void setup() {
  Serial.begin(115200);
  pinMode(25, OUTPUT); // LED pin
  Serial1.setRX(17);    // 標準設定以外の端子を使用する場合（UART0）
  Serial1.setTX(16);
  Serial1.begin(19200);
  while(!Serial1); //wait untill it opens
  
  // ================================================================================
  Wire.setSDA(12);
  Wire.setSCL(13);
  Wire.begin();

  // まず全てのGPIOをLOW
  pinMode(xshut_left, OUTPUT);
  pinMode(xshut_right, OUTPUT);
  pinMode(22,OUTPUT);
  digitalWrite(xshut_left, LOW);
  digitalWrite(xshut_right, LOW);
  digitalWrite(22,LOW);
  delay(1);

  digitalWrite(xshut_left, HIGH); // 左センサを有効状態に戻す
  delay(1);
  vl_left.setTimeout(500); //[msec]
  if (!vl_left.init())
  {
    while (1){
      Serial.println("Failed to detect and initialize vl_left!");
      delay(1000);
    }
  }
  vl_left.setAddress(vl_left_address); //2台目センサーのアドレス変更
  // -------------------------------------------------------------------------------

  digitalWrite(xshut_right, HIGH); //右のVLセンサを有効状態に戻す
  delay(1);
  vl_right.setTimeout(500); //[msec]
  if (!vl_right.init())
  {
    while (1){
      Serial.println("Failed to detect and initialize vl_right!");
      delay(1000);
    }
  }
  vl_right.setAddress(vl_right_address);

  vl_right.setDistanceMode(VL53L1X::Long);
  vl_left.setDistanceMode(VL53L1X::Long);
  vl_right.setMeasurementTimingBudget(50000); //測定タイミングバジェット(1回の距離測定)に許容される時間[micros]
  vl_left.setMeasurementTimingBudget(50000); 
  vl_right.startContinuous(50);
  vl_left.startContinuous(50);

  digitalWrite(22,HIGH);
  delay(1);
  sensor.setTimeout(500);
  if (!sensor.init())
  {
    while (1) {
      Serial.println("Failed to detect and initialize vl0 sensor!");
      delay(1000);
    }
  }
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor.setMeasurementTimingBudget(20000);
  // ================================================================================
  Serial.println("start");
}

void loop() {
  while (not Serial1.available()){}
  uint8_t hoge = Serial1.read();
  if (hoge == 10){
    Serial1.write(10);
    Serial.println("received 10 and sent 10");
  } else if(hoge == 3){
    Serial.println("change");
    uint8_t num[2];
    delay(1); // ここで遅延を挟んだげないと連続では読めない 理由は知らん
    num[0] = Serial1.read();
    num[1] = Serial1.read();
    num_3digits = num[0] * 256 + num[1];
    Serial.println(num_3digits);
  } else if(hoge == 2){
    rescue();
  } else {
    Serial.print(hoge);
    Serial.println(",not 10 was sent");
  }
}

void rescue(){
  while (not Serial1.available()){
    uint16_t vl_left_mm = vl_left.read();
    if (vl_left.timeoutOccurred()) { 
      Serial.println("TIMEOUT LEFT");
      vlxReset();
    }

    uint16_t vl_right_mm = vl_right.read();
    if (vl_right.timeoutOccurred()) {
      Serial.println("TIMEOUT RIGHT");
      vlxReset();
    }

    uint16_t vl0_mm = sensor.readRangeSingleMillimeters();
    if (sensor.timeoutOccurred()) {
      Serial.print("TIMEOUT VL0");
      vlxReset();
    }
    char sendtoEV3 = 0;
    if (vl0_mm > 180){ // 18㎝以下をボールの有無とするZ
      sendtoEV3 += 10;
    } 
    if (vl_left_mm < 50 && vl_left_mm != 0 && vl_right_mm < 50 && vl_right_mm != 0){
      sendtoEV3 += 3;
    } else if (vl_left_mm < 50 && vl_left_mm != 0){
      sendtoEV3 += 1;
    } else if (vl_right_mm < 50 && vl_right_mm != 0){
      sendtoEV3 += 2;
    }
    Serial1.write(sendtoEV3);
    delay(10);
  }
  Serial1.read();
}

void setup1(){
    //1～10番ピン　デジタル出力へセット
    for (int i=0; i<=6; i++){
        pinMode(i,OUTPUT);
        digitalWrite(i,LOW);
    }
    pinMode(BI_pin,OUTPUT);
    digitalWrite(BI_pin,HIGH);
}
//LEDレイアウトを定義
boolean Num_Array[11][4]={
//pin21,20,19,18
    {0,0,0,0}, // 0
    {1,0,0,0}, // 1
    {0,1,0,0}, // 2
    {1,1,0,0}, // 3
    {0,0,1,0}, // 4
    {1,0,1,0}, // 5
    {0,1,1,0}, // 6
    {1,1,1,0}, // 7
    {0,0,0,1}, // 8
    {1,0,0,1}, // 9
    {0,1,0,1}  // empty
};

//LED表示
void NumPrint(int number){
    for (int i=0; i<=3; i++){
//       Serial.print(number);
//       Serial.print(",");
//       Serial.print(21-i);
//       Serial.print(",");
//       Serial.println(Num_Array[number][21-i]);
        digitalWrite(i,Num_Array[number][i]);
    }
    digitalWrite(BI_pin,HIGH);
    delayMicroseconds(10); // 明るくするためにちょっと待ったげる
}

void loop1(){
    // 4桁以上だったら999に抑える
    if (num_3digits >= 1000){
        num_3digits = 999;
    }

    // 3桁目
    digitalWrite(4,HIGH);
    digitalWrite(5,HIGH);
    digitalWrite(6,LOW);
    NumPrint(num_3digits / 100);
    digitalWrite(BI_pin,LOW); // clear

    // 2桁目
    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);
    digitalWrite(6,HIGH);
    NumPrint(num_3digits % 100 / 10);
    digitalWrite(BI_pin,LOW);// clear

    // 1桁目
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);
    digitalWrite(6,HIGH);
    NumPrint(num_3digits % 10);
    digitalWrite(BI_pin,LOW); // clear
}

void vlxReset()
{
  Serial.println("Resetting Sensor");
 /* Stop the Sensor reading and even stop the i2c Transmission */
  vl_left.stopContinuous();
  vl_right.stopContinuous();
  Wire.endTransmission();

/* Reset the sensor over here, you can change the delay */
  digitalWrite(xshut_left, LOW);
  digitalWrite(xshut_right, LOW);
  digitalWrite(22, LOW);
  delay(600);
  digitalWrite(xshut_left, HIGH); // 左センサを有効状態に戻す
  delay(1);
  vl_left.setTimeout(500); //[msec]
  if (!vl_left.init())
  {
    while (1){
      Serial.println("Failed to detect and initialize vl_left!");
      delay(1000);
    }
  }
  vl_left.setAddress(vl_left_address); //2台目センサーのアドレス変更
  // -------------------------------------------------------------------------------

  digitalWrite(xshut_right, HIGH); //右のVLセンサを有効状態に戻す
  delay(1);
  vl_right.setTimeout(500); //[msec]
  if (!vl_right.init())
  {
    while (1){
      Serial.println("Failed to detect and initialize vl_right!");
      delay(1000);
    }
  }
  vl_right.setAddress(vl_right_address);

  vl_right.setDistanceMode(VL53L1X::Long);
  vl_left.setDistanceMode(VL53L1X::Long);
  vl_right.setMeasurementTimingBudget(50000); //測定タイミングバジェット(1回の距離測定)に許容される時間[micros]
  vl_left.setMeasurementTimingBudget(50000); 
  vl_right.startContinuous(50);
  vl_left.startContinuous(50);

  // vl0
  digitalWrite(22,HIGH);
  delay(1);
  sensor.setTimeout(500);
  if (!sensor.init())
  {
    while (1) {
      Serial.println("Failed to detect and initialize vl0 sensor!");
      delay(1000);
    }
  }
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor.setMeasurementTimingBudget(20000);
}