#include <Arduino.h>
#include <Wire.h>
//
// 3桁の7セグメントLEDを光らせるプログラム
//
uint num_3digits = 256;

void setup(){
    Serial.begin(115200);
    //1～10番ピン　デジタル出力へセット
    for (int i=18; i<=21; i++){
        pinMode(i,OUTPUT);
        digitalWrite(i,LOW);
    }
    for (int i=10; i<=12; i++){
        pinMode(i,OUTPUT);
        digitalWrite(i,HIGH);
    }
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
    for (int i=21; i<=18; i--){
       Serial.print(number);
       Serial.print(",");
       Serial.print(21-i);
       Serial.print(",");
       Serial.println(Num_Array[number][21-i]);
        digitalWrite(i,Num_Array[number][21-i]);
    }
}

void loop(){
    // 4桁以上だったら999に抑える
    if (num_3digits >= 1000){
        num_3digits = 999;
    }

    // 3桁目
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    digitalWrite(12,LOW);
    NumPrint(num_3digits / 100);
    NumPrint(10);

    // 2桁目
    digitalWrite(10,HIGH);
    digitalWrite(11,LOW);
    digitalWrite(12,HIGH);
    NumPrint(num_3digits % 100 / 10);
    NumPrint(10);

    // 1桁目
    digitalWrite(10,LOW);
    digitalWrite(11,HIGH);
    digitalWrite(12,HIGH);
    NumPrint(num_3digits % 10);
    NumPrint(10);
}