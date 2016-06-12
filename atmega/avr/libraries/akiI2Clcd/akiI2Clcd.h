#ifndef akiI2Clcd_H
#define akiI2Clcd_H

class akiI2Clcd
{
public:
	akiI2Clcd(uint8_t slave_address);
	void Wire_lcd_cmd( char c );
	void Wire_lcd_data(char c );
	void Wire_lcd_nputs(char *data,short n);
	void I2CLCD_init(void);
	void I2CLCD_clear( void );
	void I2CLCD_home( void );
	void I2CLCD_setCursor(short col,short row);
	void I2CLCD_write( char c );
	void I2CLCD_print( char *data );
private:
	uint8_t LCD_ADD;
	uint8_t RS0;
	uint8_t RS1;
};

#endif
