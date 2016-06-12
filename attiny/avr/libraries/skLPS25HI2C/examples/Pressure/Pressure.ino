// 大気圧センサLPS331AP(I2C接続)のテスト
#include <Wire.h>
#include "skLPSxxI2C.h"

#define SENSOR_ADRS     0x5D            // デバイスのI2Cアドレス(SDO/SAO is HIGH)

skLPSxxx LPS(LPS331AP,SENSOR_ADRS) ;    // デバイスはLPS331APを指定

void setup()
{
     int ans ;

     // シリアルモニターの設定
     Serial.begin(9600) ;
     // Ｉ２Ｃの初期化
     Wire.begin() ;                     // マスターとする

     delay(3000) ;                      // 3Sしたら開始
     // 気圧センサの初期化を行う(更新速度は1Hz)
     ans = LPS.PressureInit() ;
     if (ans == 0) Serial.println("Initialization normal") ;
     else {
          Serial.print("Initialization abnormal ans=") ;
          Serial.println(ans) ;
     }
     delay(1000) ;
}
void loop()
{
     int ans ;

     ans = LPS.PressureRead() ;         // 圧力と温度を読み出す
     Serial.print("[LPS331AP]") ;
     Serial.print(Press) ;              // 気圧値の表示を行う
     Serial.print(" hPa  ") ;
     Serial.print(Temp) ;               // 温度の表示を行う
     Serial.println(" 'C") ;

     delay(1000) ;                      // １秒後に繰り返す
}
