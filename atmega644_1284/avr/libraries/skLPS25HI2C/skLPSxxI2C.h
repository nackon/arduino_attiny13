/*******************************************************************************
*  skLPSxxI2C.h - 大気圧センサLPS331AP/LPS25H用関数ライブラリのインクルードファイル*
*                                                                              *
* ============================================================================ *
*   VERSION  DATE        BY             CHANGE/COMMENT                         *
* ---------------------------------------------------------------------------- *
*   1.00     2015-01-05  きむ茶工房     Create                                 *
* ============================================================================ *
*   Arduino IDE 1.0.5-r2 (Ardino Duemilanove 328/UNO)                          *
*******************************************************************************/
#ifndef skLPSxxI2C_h
#define skLPSxxI2C_h

#include "arduino.h"

#define LPS331AP        0xBB       // LPS331APのＩＤ
#define LPS25H          0xBD       // LPS25HのＩＤ

// デバイスのレジスタアドレス
#define WHO_AM_I_ADRS   0x0F       // デバイスの識別ID格納レジスタアドレス
#define RES_CONF_ADRS   0x30       // 圧力分解能のモード設定アドレス
#define CTRL_REG1_ADRS  0x20       // 制御レジスタ1の設定レジスタアドレス
#define CTRL_REG2_ADRS  0x21       // 制御レジスタ2の設定レジスタアドレス
#define CTRL_REG3_ADRS  0x22       // 制御レジスタ3の設定レジスタアドレス
#define STATUS_REG_ADRS 0x27       // ステータス･レジスタアドレス
#define OUT_DATA_ADRS   0x28       // 読み出すデータの先頭レジスタアドレス
#define AMP_CTRL_ADRS   0x30       // アナログ･フロント･エンド制御レジスタアドレス

// 圧力分解能のモード設定(内部での平均値読込み回数の設定)
#define RES_CONF_DATA   0b01101001 // B6-4=AVGT(64回) B3-0=AVGP(384回)
// 制御レジスタ1の設定
#define CTRL_REG1_DATA  0b10010000 // B7=PD(1) B6-4=ODR(1Hz) B3-0=(0000)

extern float Press ;               // 圧力の値を保存する変数
extern float Temp ;                // 温度の値を保存する変数

/*******************************************************************************
*	クラスの定義                                                           *
*******************************************************************************/
class skLPSxxx
{
  private:
    int Sensor_adrs ;
    int Who_Am_I_ID ;

  public:
          skLPSxxx(int id ,int address) ;
    int   PressureInit() ;
    int   PressureReceive(char reg_adrs,unsigned char *data,char kosu) ;
    int   PressureSend(char reg_adrs,unsigned char *data,char kosu) ;
    int   PressurePD(char mode) ;
    int   PressureRead() ;
    float AltitudeCalc(float pressure,int Difference) ;
} ;

#endif
