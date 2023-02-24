#include <Arduino.h>
// 125mAくらい
// 1相励磁(ライブラリなし)
// 指で止めれるくらいのトルク
// 他のは褒めれなかった
 
#define PIN1 18  // 青
#define PIN2 19  // ピンク
#define PIN3 20  // 黄
#define PIN4 21  // オレンジ
 
int tim = 2;  // STEPごとに停止する間隔(小さくすると回転スピードが上昇)
 
void setup(){
  
  pinMode(PIN1, OUTPUT);  // デジタルピンを出力に設定
  pinMode(PIN2, OUTPUT);
  pinMode(PIN3, OUTPUT);
  pinMode(PIN4, OUTPUT);
  
}
 
 
void loop(){
 
// STEP1
  digitalWrite(PIN1, HIGH);
  digitalWrite(PIN2, LOW);
  digitalWrite(PIN3, LOW);
  digitalWrite(PIN4, LOW);
  delay(tim);
  
// STEP2
  digitalWrite(PIN1, LOW);
  digitalWrite(PIN2, HIGH);
  digitalWrite(PIN3, LOW);
  digitalWrite(PIN4, LOW);
  delay(tim);
  
// STEP3
  digitalWrite(PIN1, LOW);
  digitalWrite(PIN2, LOW);
  digitalWrite(PIN3, HIGH);
  digitalWrite(PIN4, LOW);
  delay(tim);
  
// STEP4
  digitalWrite(PIN1, LOW);
  digitalWrite(PIN2, LOW);
  digitalWrite(PIN3, LOW);
  digitalWrite(PIN4, HIGH);
  delay(tim);
 
}