#include "arduino.h"
#include "akiI2Clcd.h"
#include "Wire.h"

akiI2Clcd::akiI2Clcd(uint8_t slave_address){
	LCD_ADD = slave_address;
	RS0 = 0x00;
	RS1 = 0x80;
}

void akiI2Clcd::Wire_lcd_cmd(char c){
  Wire.beginTransmission(LCD_ADD); 
  Wire.write(RS0); 
  Wire.write(c); 
  Wire.endTransmission(); 
} 

void akiI2Clcd::Wire_lcd_data(char c){
  Wire.beginTransmission(LCD_ADD); 
  Wire.write(RS1); 
  Wire.write(c); 
  Wire.endTransmission(); 
} 

void akiI2Clcd::Wire_lcd_nputs(char *data,short n){
  Wire.beginTransmission(LCD_ADD); 
  while(n--){ 
    Wire.write(RS1); 
    Wire.write(*data++); 
  } 
  Wire.endTransmission(); 
} 

void akiI2Clcd::I2CLCD_init(){
  Wire_lcd_cmd(0x38); // function set 
  Wire_lcd_cmd(0x0C); // Display ON 
  Wire_lcd_cmd(0x01); // Clear display 
  delay(200); 
}

void akiI2Clcd::I2CLCD_clear(){
  Wire_lcd_cmd(0x01); 
  delay(200); 
} 

void akiI2Clcd::I2CLCD_setCursor(short col,short row){ 
  byte dat; 
  if(row == 0 ){ 
    dat = 0x80 + col; 
  }else{ 
    dat = 0xc0 + col; 
  } 
  Wire_lcd_cmd(dat); 
}

void akiI2Clcd::I2CLCD_write( char c ){ 
  Wire_lcd_data(c); 
}

void akiI2Clcd::I2CLCD_print( char *data ){ 
  byte n; 
  n = strlen(data); 
  Wire_lcd_nputs(data,n); 
}