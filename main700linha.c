// ======================================= Universidade Federal de Pelotas
// ======================================= Centro de Desenvolvimento Tecnológico
// ======================================= Bacharelado em Engenharia de Computação
// ======================================= Disciplina: 22000279 --- Microcontroladores
// ======================================= Turma: 2023/1 --- M1
// ======================================= Professor: Alan Rossetto
//
//										  Descrição: Trabalho Final
//
// 										  Identificação:
// 										  Nome da(o) aluna(o) & Matricula: Ari Vitor da Silva Lazzarotto, 17200917
// 										  Nome da(o) aluna(o) & Matricula: Adriel Correa Matielo, 16105321
//										  Data: 12/09/2023			
//
//======================================= 
#include <at89x52.h>
#include <stdio.h>
#include <stdlib.h>
//keyboard 4x3
#define LIN0 P1_0
#define LIN1 P1_1
#define LIN2 P1_2
#define LIN3 P1_3
#define COL0 P1_4
#define COL1 P1_5
#define COL2 P1_6
//motors 3x3 on DEMUX 4:10
#define MOTOR0 P0_0
#define MOTOR1 P0_1
#define MOTOR2 P0_2
#define MOTOR3 P0_3
//Money Insert
#define PLUS1 P3_0
#define PLUS10 P3_1
#define MINUS1 P3_3
#define MINUS10 P3_4
extern float money;
// LCD pins
#define LCD			P2
#define EN			P3_7
#define RS			P3_6
#define MOTOR_MATRIX_LINE 3
#define MOTOR_MATRIX_COLLUM 3
#define MAX_PRODUCT 9
extern int PRODUCT[MOTOR_MATRIX_LINE][MOTOR_MATRIX_COLLUM];
extern int DIGIT1 = 0;
extern int DIGIT2 = 0;
// Define product prices
extern float product_prices[MAX_PRODUCT];
int selected_product = 0;
float total_price = 0.0;
float inserted_amount = 0.0;
int input = 0;
int isMoneyinsert = 0;
extern int FLAG = 0;
void ISR_External0(void) interrupt 0{
	isMoneyinsert = 1;
	EX0 = 0;
	return;
}
void ISR_Externa20(void) interrupt 2{
	P1_4 = 0;
	P1_3 = 1;
	EX1 = 0;
	FLAG = 1;
	return;
}
void start();
void store(int line, int coll, int qnt);
void returnInsertedMoney(float inserted_amount);
void dispenseProduct(int selected_product);
void dispenseChange(float change);
void dispenseProductAndChange(int selected_product, float change);
void waitTheRestOfcash(float product_price, float inserted_amount);
int checkIfNeedChange(float inserted_amount, float product_price);
void cancelRequest();
int sumMoney();
void ConfigLCD(void);
void WriteMSG(char msg[]);
void Line1(void);
void Line2(void);
void WrCMD(void);
void WrCHAR(void);
int CHECK_LINES();
void Delay5us(void);
void Delay5ms(void);
void DELAY(unsigned int ms);
void DELAY1_MS(unsigned int ms); 
void DELAY_30(unsigned int time);
int binary[3];
int i,j;

void main(void) {
	start();
	while(1){
		if (isMoneyinsert){
				 start();
			EX0 = 0;
					input = CHECK_LINES();
					if (input == 77)
						return;
			EX0 = 0;					// case '*' or '#' to cancel the transaction and return any inserted money.
					if( input == 10 || input == 12) {                                     
							ConfigLCD();
							Line1();
							WriteMSG("* Transaction canceled *");
							Line2();
							returnInsertedMoney(inserted_amount);
							return;
					}else{
							if( input == 11) {                                                // case '0' to cancel the transaction and return any inserted money.
									DIGIT1 = 0;
							}else
									DIGIT1 = input;
					}
			ConfigLCD();
			Line1();
			WriteMSG("* Insert the second digit *");
			Line2();
			WriteMSG("  type '*' or '#' to cancel "); 
			input = CHECK_LINES();
			// case '*' or '#' to cancel the transaction and return any inserted money.
			if( input == 10 || input == 12) {                                          
					cancelRequest();
					returnInsertedMoney(inserted_amount);
					return;
			}else{
					if( input == 11) {                                               // case '0' to cancel the transaction and return any inserted money.
							DIGIT2 = 0;
					}else
							DIGIT2 = input;
			}
			input = DIGIT1 * 10 + DIGIT2;                                        // the value of the input is the concat of the 2 digits       
			selected_product = PRODUCT[DIGIT1][DIGIT2];                          // here we have the stock of selected product
			if (input >= 1 && input <= MAX_PRODUCT) {                                     // check if the input is a valid product number
					if (selected_product >= 1 ) {                                    // check if the selected product is in stock , calculate the total price of the selected product
							total_price = product_prices[input];												// Accept and validate coins until the total amount is reached
							EX0 = 0;
							while (inserted_amount < total_price){
								if (FLAG){
									cancelRequest();
									return;
								}
								ConfigLCD();
								Line1();
								WriteMSG("* Insert the money *");
								Line2();
								WriteMSG(" inserted_amount: ");	// dont show the inserted money
								if(isMoneyinsert){
									isMoneyinsert = 0;
									inserted_amount += sumMoney();
								}
							}
							EX0 = 1;
							if (inserted_amount == total_price) {
									ConfigLCD();
									Line1();
									WriteMSG("* Dispensing product *");
									Line2();
									WriteMSG(" Thank you ");
									dispenseProduct(input);
									return;
							} else if (inserted_amount > total_price) {
									ConfigLCD();
									Line1();
									WriteMSG("* Dispeling change *");
									Line2();
									WriteMSG(" Thank you ");  
									dispenseProductAndChange(input, checkIfNeedChange(inserted_amount, total_price));
									return;
							}
					} else {
							ConfigLCD();
							Line1();
							WriteMSG("* Product out of stock. *");
							Line2();
							WriteMSG(" Chose other product now! ");
							return;
					}
			} else {
					ConfigLCD();
					Line1();
					WriteMSG("* Invalid input. (01-09) *");
					Line2();
					WriteMSG(" '*' to cancel, or '#' to confirm ");
					return;
			}
			ConfigLCD();
			Line1();
			WriteMSG("* Deu problema *");
			Line2();
			WriteMSG(" nao deveria chegar aqui ");
			return;
		}
		else{
			printf("nada n");
		
	}
	}
}
void Delay5us(void){
	unsigned char i;
	for(i = 0; i < 5; i++){}
}
void Delay5ms(void){
	DELAY(5);
}
void DELAY(unsigned int ms) {       // delay vai ser diferente do esperado, mas deveria funcionar
	TMOD |= 0x01;					// A opera?o OU preserva alguma eventual configura?o previa do T/C 1.
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
void DELAY1_MS(unsigned int ms) {//mode 01 for timer1
	TMOD |= 0x10;
	while(ms){
		TH1 = 0xFE;
		TL1 = 0x18;
		TR1 = 1;
		while(!TF1);
		TF1 = 0;
		TR1 = 0;
		ms--;
	}
}
void DELAY_30(unsigned int time) {//mode 01 for timer1
	TMOD |= 0x10;
	while(time){
		TH1 = 0xFE;
		TL1 = 0x18;
		TR1 = 1;
		while(!TF1);
		TF1 = 0;
		TR1 = 0;
		time--;
	}
}
int CHECK_LINES() {      // loop to check the keyboard and get the input, until the user press '*' or '#' key. Need a logic to take 2 digits input.
    while(1) {
        DELAY(100);
        LIN0 = 0;
        LIN1 = 1;
        LIN2 = 1;
        LIN3 = 1; 
        if (!COL0) {
            DELAY(100);
						return 1;
        }
        if (!COL1) {
            DELAY(100);
            return 2;
        }
        if (!COL2) {
            DELAY(100);
            return 3;
        }
        LIN0 = 1;
        LIN1 = 0;
        LIN2 = 1;
        LIN3 = 1;
        if (!COL0) {
            DELAY(100);
            return 4;
        }
        if (!COL1) {
            DELAY(100);
            return 5;
        }
        if (!COL2) {
            DELAY(100);
            return 6;
        }
        LIN0 = 1;
        LIN1 = 1;
        LIN2 = 0;
        LIN3 = 1;
        if (!COL0) {
            DELAY(100);
            return 7;
        }
        if (!COL1) {
            DELAY(100);
            return 8;
        }
        if (!COL2) {
            DELAY(100);
            return 9;
        }
        LIN0 = 1;
        LIN1 = 1;
        LIN2 = 1;
        LIN3 = 0;
        if (!COL0) {
						//'*' key
            DELAY(100);
            return 10;
        }
        if (!COL1) {
            // '0' key 
            DELAY(100);
            return 11;
        }
        if (!COL2) {
						// '#' key
            DELAY(100);
            return 12;
        }
    }
}
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
	for(i = 0; i < 16; i++){
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
void start(){//IE [EA - - ES ET1 EX1 ET0 EX0]
		P0 = 1;
		EA = 1;	//Charge machine with products
		for ( i=0; i < MOTOR_MATRIX_LINE; i++){
			for ( j=0; j < MOTOR_MATRIX_COLLUM; j++){
				store(i, j, MAX_PRODUCT);
			}
		}//Charge vetor with product prices
		for (i=0; i < MOTOR_MATRIX_LINE*MOTOR_MATRIX_COLLUM; i++){
			product_prices[i] = 1.0 * (i+1);
		}//Set first message on display
		ConfigLCD();
    Line1();
    WriteMSG("* Insert the COD of product *");
    Line2();
    WriteMSG("  or the money ");
}
void store(int x, int y, int qnt){
	PRODUCT[x][y] = qnt;
}
void returnInsertedMoney_time(float inserted_amount) {
		ConfigLCD();
    Line1();
    WriteMSG("* Time exceded *");
    Line2();
		inserted_amount += 1;
    WriteMSG("%f.1 Returned amount ");									// dont work
}
void dispenseProduct(int selected_product) {
	int temp_product = selected_product;
	ConfigLCD();
	Line1();
	WriteMSG("* Working on demand *");
	Line2();
	WriteMSG(" %d temp_product");												// dont work
	i = 0;// Converte decimal para bin?rio
	while (selected_product > 0) {
			binary[i] = selected_product % 2;
			selected_product = selected_product / 2;
			i++;
	}
	MOTOR0 = binary[0];
	MOTOR1 = binary[1];
	MOTOR2 = binary[2];
	MOTOR3 = binary[3];
	DELAY(5000);
	ConfigLCD();
	Line1();
	WriteMSG("Your product is ");
	Line2();
	WriteMSG(" %d temp_product");									// dont work
	DELAY(3000);
}
void dispenseChange(float change) { 
	ConfigLCD();
    Line1();
    WriteMSG("Your change is");
    Line2();
    WriteMSG("%f.1 troco");								// dont work
		change += 1;
	  DELAY(5000);
}
void dispenseProductAndChange(int selected_product, float change) {
	dispenseProduct(selected_product);
	dispenseChange(change);
}
void waitTheRestOfcash(float product_price, float inserted_amount) {
	ConfigLCD();
	Line1();
	WriteMSG("Insert the rest of");
	Line2();
	WriteMSG(" %f.1 product_price-inserted_amount");								// dont work
	product_price = inserted_amount + 1;
	DELAY(3000);
}
int checkIfNeedChange(float inserted_amount, float product_price) { 
	if (inserted_amount > product_price){
		dispenseChange(inserted_amount-product_price);
	}
	return 1;
}
int sumMoney(){
	if (PLUS1)
		money += 1;
	if (PLUS10)
		money += 10;
	if (MINUS1){
		money -= 1;
		if (money < 0){
			money += 1;
		}
	}
	if (MINUS10)
		money -= 10;{
		if (money < 0){
			money += 10;
		}
	}
	return money;
}
void cancelRequest(){
	ConfigLCD();
        Line1();
        WriteMSG("* Transaction canceled *");
        Line2();
        WriteMSG(" Returning inserted money ");
}