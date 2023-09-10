#include "configuration.h"

void vendingMachine() {

    start();
    
    while(!isMoneyinsert){
        input = CHECK_LINES();
		IE = 0;

        if( input == 10 || input == 12) {                                     // case '*' to cancel the transaction and return any inserted money.
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
    }

    ConfigLCD();
    Line1();
    WriteMSG("* Insert the second digit *");
    Line2();
    WriteMSG("  or type '*' to cancel "); 
    
    input = CHECK_LINES();
    if( input == 10 || input == 12) {                                          // case '*' to cancel the transaction and return any inserted money.
        ConfigLCD();
        Line1();
        WriteMSG("* Transaction canceled *");
        Line2();
        WriteMSG(" Returning inserted money ");
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

    if (input >= 1 && input <= 30) {                                     // check if the input is a valid product number
        if (selected_product >= 1 ) {                                    // check if the selected product is in stock
            // Calculate the total price of the selected product
            total_price = product_prices[selected_product];

            // Accept and validate coins until the total amount is reached
			IE = 1;
            while (inserted_amount < total_price) {
                ConfigLCD();
                Line1();
                WriteMSG("* Insert the money *");
                Line2();
                WriteMSG(" Insert a coin: ");
            }
            
            if (inserted_amount == total_price) {
                // No change to be dispensed
                // Move motor associated with selected_product
                ConfigLCD();
                Line1();
                WriteMSG("* Dispensing product *");
                Line2();
                WriteMSG(" Thank you ");
                return;
            } else if (inserted_amount > total_price) {
                // Provide correct change and dispense the product
                // Move motor associated with selected_product
                ConfigLCD();
                Line1();
                WriteMSG("* Dispeling change *");
                Line2();
                WriteMSG(" Thank you ");      
                return;
            } else {
                // The amount isn't correct, continue accepting money
                while(inserted_amount < total_price) {
                    float inserted_coin;
                    ConfigLCD();
                    Line1();
                    WriteMSG("* Insufficient funds. *");
                    Line2();
                    WriteMSG(" Please insert more coins ");
                    waitTheRestOfcash(total_price, inserted_amount);
                }
                return;
            }
        } else {
            ConfigLCD();
            Line1();
            WriteMSG("* Product out of stock. *");
            Line2();
            WriteMSG(" Returning inserted money ");
            return;
        }
    } else {
        ConfigLCD();
        Line1();
        WriteMSG("* Invalid input. Please enter a valid product number (01-30) *");
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