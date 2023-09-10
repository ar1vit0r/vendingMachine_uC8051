#include <at89x52.h>
#include "configuration.h"

void ISR_External0(void) interrupt 2{
	P1_4 = 0;
	P1_3 = 1;
	EX1 = 0;
	FLAG = 1;
return;
}