#include <at89x52.h>
#include "lcd.h"
#include "delay.h"

void ConfigLCD(void) {
	LCD = 0x38;
	WrCMD();
	LCD = 0x06;
	WrCMD();
	LCD = 0x0E;
	WrCMD();
	LCD = 0x01;
	WrCMD();
}



void WriteMSG(char msg[]){
	unsigned char i;
	for(i = 0; i < 16; i++)
	{
		LCD = msg[i];
		WrCHAR();
	}
}

void Line1(void){	
	LCD = 0x00;
	WrCMD();
}

void Line2(void){
	LCD = 0xC0;
	WrCMD();
}

void WrCMD(void){
	RS = 0;
	EN = 1;
	Delay5us();
	EN = 0;
	Delay5ms();
}

void WrCHAR(void){
	RS = 1;
	EN = 1;
	Delay5us();
	EN = 0;
	Delay5ms();
}