#include <at89x52.h>
#include "configuration.h"

void start(){
	P0 = 1;
//Charge machine with productis
	for (int i=0; i < MOTOR_MATRIX_SIZE; i++){
		for (int j=0; j < MOTOR_MATRIX_SIZE; j++){
			store(i, j, MAX_PRODUCT);
		}
	}
//Set first message on display
	ConfigLCD();
    Line1();
    WriteMSG("* Insert the COD of product *");
    Line2();
    WriteMSG("  or the money ");
}

void store(int line, int coll, int qnt){
	PRODUCT[line][colum] = qnt;
}	

void returnInsertedMoney_time(float inserted_amount) {
	
	ConfigLCD();
    Line1();
    WriteMSG("* Time exceded *");
    Line2();
    WriteMSG("%f.1 Returned amount ", inserted_amount);
}

void dispenseProduct(int selected_product) {
	int temp_product = selected_product;
	ConfigLCD();
    Line1();
    WriteMSG("* Working on demand *");
    Line2();
    WriteMSG(" %d ", temp_product);
	
	int binary[3];
	int i = 0;

	// Converte decimal para bin�rio
	while (selected_product > 0) {
			binary[i] = selected_product % 2;
			selected_product = selected_product / 2;
			i++;
	}
	MOTOR0 = binary[0];
	MOTOR1 = binary[1];
	MOTOR2 = binary[2];
	MOTOR3 = binary[3];
	void DELAY(5000);
	
	ConfigLCD();
    Line1();
    WriteMSG("Your product is ");
    Line2();
    WriteMSG(" %d ", temp_product);
	void DELAY(3000);
}

void dispenseChange(float change) { 
	ConfigLCD();
    Line1();
    WriteMSG("Your change is");
    Line2();
    WriteMSG("%f.1 ", change);
	void DELAY(5000);
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
	WriteMSG(" %f.1 ", product_price-inserted_amount);
	void DELAY(3000);
}

int checkIfNeedChange(float inserted_amount, float product_price) { 
	if (inserted_amount > product_price)
		dispenseChange(inserted_amount-product_price);
}