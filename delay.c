#include <at89x52.h>
#include "configuration.h"

void Delay5us(void){
	unsigned char i;
	for(i = 0; i < 5; i++){}
}

void Delay5ms(void){
	DELAY(5);
}

void DELAY(unsigned int ms) {       // delay vai ser diferente do esperado, mas deveria funcionar
	TMOD |= 0x01;					// A operaço OU preserva alguma eventual configuraço previa do T/C 1.
	
	while(ms){
		TH0 = 0xFE;					// Valor de recarga para 1 ms @ f = 12 MHz (i.e. 64535).
		TL0 = 0x18;
		TR0 = 1;
		while(!TF0);
		TF0 = 0;
		TR0 = 0;
		ms--;
	}
}

void DELAY1_MS(unsigned int ms) {
	//mode 01 for timer1
	TMOD = 0x10;
	
	while(ms){
		TH0 = 0xFC;
		TL0 = 0x17;
		TR0 = 1;
		while(!TF0);
		TF0 = 0;
		TR0 = 0;
		ms--;
	}
}