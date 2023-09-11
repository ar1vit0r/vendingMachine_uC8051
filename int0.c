#include "configuration.h"

void ISR_External0(void) interrupt 0{
	isMoneyinsert = 1;
	EX0 = 0
return;
}