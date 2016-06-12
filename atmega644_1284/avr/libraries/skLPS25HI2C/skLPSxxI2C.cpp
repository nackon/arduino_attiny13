/*******************************************************************************
*  skLPSxxI2C - 大気圧センサLPS331AP/LPS25H(I2C接続)用関数ライブラリ           *
*                                                                              *
*    skLPSxxx        - この関数ライブラリを生成する時の初期化処理              *
*    PressureInit    - デバイスの初期化を行う処理                              *
*    PressureReceive - デバイスから指定個数のデータを受信する処理              *
*    PressureSend    - デバイスに指定個数のデータを送信する処理                *
*    PressurePD      - デバイスの"パワーダウン"と"動作中"を切り替える処理      *
*    PressureRead    - 圧力･温度を読込み気圧値を計算する処理                   *
*    AltitudeCalc    - 気圧値(hPa)から高度を計算する処理                       *
*                                                                              *
* ============================================================================ *
*   VERSION  DATE        BY             CHANGE/COMMENT                         *
* ---------------------------------------------------------------------------- *
*   1.00     2015-01-05  きむ茶工房     Create                                 *
* ============================================================================ *
*   Arduino IDE 1.0.5-r2 (Ardino Duemilanove 328/UNO)                          *
*******************************************************************************/
#include <Wire.h>
#include "arduino.h"
#include "skLPSxxI2C.h"

float Press ;                 // 圧力の値を保存する変数
float Temp ;                  // 温度の値を保存する変数

/*******************************************************************************
*  skLPSxxx(id,address)                                                        *
*    この関数ライブラリを生成する時の初期化処理(コンストラクタ)                *
*    id      : デバイスの識別IDを指定します                                    *
*    address : デバイス(スレーブ)のI2Cアドレスを指定します                     *
*******************************************************************************/
skLPSxxx::skLPSxxx(int id ,int address)
{
     Who_Am_I_ID = id ;
     Sensor_adrs = address ;
}
/*******************************************************************************
*  ans = PressureInit()                                                        *
*    デバイスの初期化を行う処理                                                *
*    デバイスの動作を確かめる為にデバイスIDのチェックを行います。              *
*    分解能のモード(内部平均の数)設定は、デフォルトで動作させます。            *
*    制御レジスタ1は"動作中モード","1Hzの更新速度",Bit0-3=0 で初期化しています *
*    初期化データを変更する場合は、skLPSxx.hのRES_CONF_DATA/CTRL_REG1_DATAを   *
*    書き換え変更して下さい。                                                  *
*    ans  : 戻り値、0=正常終了　それ以外I2C通信エラー下記                      *
*                   1=送ろうとしたデータが送信バッファのサイズを超えた         *
*                   2=スレーブ・アドレスを送信し、NACKを受信した               *
*                   3=データ・バイトを送信し、NACKを受信した                   *
*                   4=その他のエラー                                           *
*                   5=データ受信エラー                                         *
*                   6=デバイスのIDチェックエラー                               *
*******************************************************************************/
int skLPSxxx::PressureInit()
{
     int  ans ;
     unsigned char data[2] ;

     // デバイスの識別ＩＤをチェックする処理
     ans = PressureReceive(WHO_AM_I_ADRS,&data[0],1) ;
     if (ans == 0) {
          // WHO_AM_Iの内容をチェック
          if (data[0] == Who_Am_I_ID) ans = 0 ; // ＩＤは一致した
          else                        ans = 6 ; // ＩＤが一致しない
     }
     // デバイスを初期化する処理
     if (ans == 0) {
          // 圧力分解能のモード設定(圧力=384回 温度=64回 で平均を行う)
          //data[0] = RES_CONF_DATA ;
          //ans = PressureSend(RES_CONF_ADRS,&data[0],1) ;
          // 制御レジスタ1の設定(動作中モード,出力データ速度は1Hz)
          data[0] = CTRL_REG1_DATA ;
          ans = PressureSend(CTRL_REG1_ADRS,&data[0],1) ;
          delay(1) ;                            // 確実に設定が終了するのを待つ
     }
     return ans ;
}
/*******************************************************************************
*  ans = PressureReceive(reg_adrs,*data,kosu)                                  *
*  デバイスから指定個数のデータを受信する処理                                  *
*    reg_adrs : 読出すデータのレジスターアドレスを指定する                     *
*               連続的に読出す場合は、読出すレジスターの先頭アドレスを指定     *
*    *data    : 読出したデータの格納先を指定する                               *
*    kosu     : 読出すデータのバイト数を指定する                               *
*    ans      : 戻り値、0=正常終了　それ以外PressureInit()のans値を参照        *
*******************************************************************************/
int skLPSxxx::PressureReceive(char reg_adrs,unsigned char *data,char kosu)
{
     int  ans , i ;
     char sub ;

     if (kosu == 1) sub = reg_adrs ;                   // 単発読み出しモード
     else           sub = reg_adrs | 0x80 ;            // 連続読み出しモード
     Wire.beginTransmission(Sensor_adrs) ;             // 通信の開始
     Wire.write(sub) ;                                 // レジスタアドレスを指定
     ans = Wire.endTransmission() ;                    // データの送信と通信の終了
     if (ans == 0) {
          ans = Wire.requestFrom(Sensor_adrs,kosu) ;   // データの受信を行う
          if (ans == kosu) {
               for (i=0 ; i<kosu ; i++) {
                    *data = Wire.read() ;
                    data++ ;
               }
               ans = 0 ;
          } else ans = 5 ;                             // 受信バイト数がおかしい？
     }
     return ans ;
}
/*******************************************************************************
*  ans = PressureSend(reg_adrs,*data,kosu)                                     *
*  デバイスに指定個数のデータを送信する処理                                    *
*    reg_adrs : 書出すデータのレジスターアドレスを指定する                     *
*               連続的に書出す場合は、書出すレジスターの先頭アドレスを指定     *
*    *data    : 書出すデータの格納先を指定する                                 *
*    kosu     : 書出すデータのバイト数を指定する                               *
*    ans      : 戻り値、0=正常終了　それ以外PressureInit()のans値を参照        *
*******************************************************************************/
int skLPSxxx::PressureSend(char reg_adrs,unsigned char *data,char kosu)
{
     int  ans , i ;
     char sub ;

     if (kosu == 1) sub = reg_adrs ;              // 単発書き込みモード
     else           sub = reg_adrs | 0x80 ;       // 連続書き込みモード
     Wire.beginTransmission(Sensor_adrs) ;        // 通信の開始
     Wire.write(sub) ;                            // レジスタのアドレスを通知
     for (i=0 ; i<kosu ; i++) {
          Wire.write(*data) ;                     // データを通知
          data++ ;
     }
     ans = Wire.endTransmission() ;               // データの送信と通信の終了
     return ans ;
}
/*******************************************************************************
*  ans = PressurePD(mode)                                                      *
*  デバイスの"パワーダウン"と"動作中"を切り替える処理                          *
*    mode : 0=パワーダウンモード , 1=動作中モード                              *
*    ans  : 戻り値、0=正常終了　それ以外PressureInit()のans値を参照            *
*******************************************************************************/
int skLPSxxx::PressurePD(char mode)
{
     int  ans ;
     unsigned char data[2] ;

     // 制御レジスタ1の現在情報を受信する
     ans = PressureReceive(CTRL_REG1_ADRS,&data[0],1) ;
     if (ans == 0) {
          if (mode == 0) {
               data[0] = data[0] & 0x7F ;    // パワーダウンモードにする
          } else {
               data[0] = data[0] | 0x80 ;    // 動作中モードにする
          }
          // 制御レジスタ1の情報を送信する
          ans = PressureSend(CTRL_REG1_ADRS,&data[0],1) ;
          delay(1) ;                         // 確実に設定が終了するのを待つ
     }
     return ans ;
}
/*******************************************************************************
*  ans = PressureRead()                                                        *
*  圧力･温度を読込み気圧値を計算する処理                                       *
*  計算された気圧値(hPa)はPress変数に、温度値(℃)はTemp変数に其々格納されます。*
*    ans  : 戻り値、0=正常終了　それ以外PressureInit()のans値を参照            *
*******************************************************************************/
int skLPSxxx::PressureRead()
{
     int  ans ;
     unsigned char data[6] ;

     // 圧力と温度データを受信する(このデータはデバイス内部で平均化されています)
     ans = PressureReceive(OUT_DATA_ADRS,&data[0],5) ;
     if (ans == 0) {
          // 大気圧の計算を行う
          Press = ((unsigned long)data[2]<<16) + ((unsigned long)data[1]<<8) + data[0] ;
          Press = Press / 4096.0 ;      // hPa
          // 温度の計算を行う
          Temp = (int)(1 + ~((unsigned int)data[4]<<8 | data[3])) * -1 ;
          Temp = 42.5 + (Temp / 480.0) ;// ℃
     }
     return ans ;
}
/*******************************************************************************
*  ans = AltitudeCalc(pressure,Difference)                                     *
*  気圧値(hPa)から高度を計算する処理                                           *
*    pressure   : 計算した大気圧値を hPa でセットする。                        *
*    Difference : 標高の差を指定する                                           *
*    ans        : 高度値を ｍ で返す。                                         *
*******************************************************************************/
float skLPSxxx::AltitudeCalc(float pressure,int Difference)
{
     float h ;

     h = 44330.8 * (1.0 - pow( (pressure/1013.25) ,  0.190263 )) ;
     h = h + Difference ;
     return h ;
}
