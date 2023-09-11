#include "configuration.h"

void cashInput() {

	start();

	//Insert money
	EX0=0;
	//Cancel operation
	EX1=0;
	
	while (FLAG){
		cancelRequest();
		return;

		ConfigLCD();
		Line1();
		WriteMSG("* Press '*' for continue *");
		Line2();
		WriteMSG(" Inserted amount %d ", &inserted_amount);
		if(isMoneyinsert){
			isMoneyinsert = 0;
			inserted_amount += sumMoney();
		}
	}
	EX0=1
	EX1=1;
//===================DIGIT 1
	ConfigLCD();
    Line1();
    WriteMSG("* Insert the first digit *");
    Line2();
    WriteMSG("  type '*' or '#' to cancel ");
	input = CHECK_LINES();
	//CANCELA E DEVOLVE O DINHEIRO
	if (input == 10 || input == 12){
		cancelRequest();
		returnInsertedMoney(inserted_amount);
		return;
	}else{
		 // case '0' to cancel the transaction and return any inserted money.
			if( input == 11) {                                               
					DIGIT1 = 0;
			}else
					DIGIT1 = input;
//===================DIGIT 2
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
			// case '0' to cancel the transaction and return any inserted money.
        if( input == 11) {                                               
            DIGIT2 = 0;
        }else
            DIGIT2 = input;
    }
//===================DIGIT 1 + DIGIT 2

    input = DIGIT1 * 10 + DIGIT2;                                        // the value of the input is the concat of the 2 digits       
    selected_product = PRODUCT[DIGIT1][DIGIT2];                          // here we have the stock of selected product

    if (input >= 1 && input <= MAX_PRODUCT) {                                     // check if the input is a valid product number
        if (selected_product >= 1 ) {                                    // check if the selected product is in stock
            // Calculate the total price of the selected product
            total_price = product_prices[input];

            // Accept and validate coins until the total amount is reached
						IE = 1;
            while (inserted_amount < total_price){
							if (FLAG){
								cancelRequest();
								return;
							}
              ConfigLCD();
              Line1();
							WriteMSG("* Insert the money *");
							Line2();
							WriteMSG(" %d ", &inserted_amount);
            }
						IE = 0;
            
            if (inserted_amount == total_price) {
                // No change to be dispensed
                // Move motor associated with selected_product
                ConfigLCD();
                Line1();
                WriteMSG("* Dispensing product *");
                Line2();
                WriteMSG(" Thank you ");
								dispenseProduct(input);
                return;
            } else if (inserted_amount > total_price) {
                // Provide correct change and dispense the product
                // Move motor associated with selected_product
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
        WriteMSG("* Invalid input. (01~09) *");
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