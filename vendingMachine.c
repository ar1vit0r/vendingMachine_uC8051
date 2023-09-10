#include "configuration.h"

void vendingMachine(int input, int selected_product, float total_price, float inserted_amount) {
    
    CHECK_LINES(input);

    if( input == 10 || input == 12) {                                                     // case '*' to cancel the transaction and return any inserted money.
        ConfigLCD();
        Line1();
        WriteMSG("* Transaction canceled *");
        Line2();
        WriteMSG(" Returning inserted money ");
        //while(1);                                                                       
        printf("Transaction canceled. Returning inserted money.\n");
        returnInsertedMoney(inserted_amount);
        //break;
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
    WriteMSG("  or type '*' to cancel ");
    //while(1);    
    
    CHECK_LINES(input);
    if( input == 10 || input == 12) {                                     // case '*' to cancel the transaction and return any inserted money.
        ConfigLCD();
        Line1();
        WriteMSG("* Transaction canceled *");
        Line2();
        WriteMSG(" Returning inserted money ");
        //while(1);                                                                       
        printf("Transaction canceled. Returning inserted money.\n");
        returnInsertedMoney(inserted_amount);
        //break;
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
            while (inserted_amount < total_price) {
                float inserted_coin;
                ConfigLCD();
                Line1();
                WriteMSG("* Insert the money *");
                Line2();
                WriteMSG(" alguma coisa aqui");
                //while(1);    
                printf("Insert a coin: ");
                scanf("%f", &inserted_coin);
                inserted_amount += inserted_coin;
            }
            
            if (inserted_amount == total_price) {
                // No change to be dispensed
                // Move servo motor associated with selected_product
                ConfigLCD();
                Line1();
                WriteMSG("* Dispensing product *");
                Line2();
                WriteMSG(" Thank you for using the vending machine ");
                //while(1);
                printf("Thank you for using the vending machine!\n");
                //break;
            } else if (inserted_amount > total_price) {
                // Provide correct change and dispense the product
                // Move servo motor associated with selected_product
                ConfigLCD();
                Line1();
                WriteMSG("* Dispeling change *");
                Line2();
                WriteMSG(" Thank you for using the vending machine ");
                //while(1);        
                float change = inserted_amount - total_price;
                printf("Change: $%.2f\n", change);
                printf("Thank you for using the vending machine!\n");
                //break;
            } else {
                // The amount isn't correct, continue accepting money
                while(inserted_amount < total_price) {
                    float inserted_coin;
                    ConfigLCD();
                    Line1();
                    WriteMSG("* Insufficient funds. *");
                    Line2();
                    WriteMSG(" Please insert more coins ");
                    //while(1);        
                    printf("Insert a coin: ");
                    scanf("%f", &inserted_coin);
                    inserted_amount += inserted_coin;
                    printf("Insufficient funds. Please insert more coins.\n");
                    waitTheRestOfcash(total_price, inserted_amount);
                }
            }
        } else {
            ConfigLCD();
            Line1();
            WriteMSG("* Product out of stock. *");
            Line2();
            WriteMSG(" Returning inserted money ");
            //while(1);      
            printf("Product out of stock.\n");
        }
    } else {
        ConfigLCD();
        Line1();
        WriteMSG("* Invalid input. Please enter a valid product number (01-30) *");
        Line2();
        WriteMSG(" '*' to cancel, or '#' to confirm ");
        //while(1);      
        printf("Invalid input. Please enter a valid product number (01-30), '*' to cancel, or '#' to confirm.\n");
    }

    ConfigLCD();
    Line1();
    WriteMSG("* Value of product:  *");
    Line2();
    WriteMSG(" waiting for the money ");
    //while(1);    
    //FUNC ESPERA DO DINHEIRO
    //O DINHEIRO � SUFICIENTE
        //Y FUNC RODA O MOTOR CODIGO
        //Y DEVOLVE O TROCO
    //O DINHEIRO N � SUFICIENTE
        //Y FUNC ESPERA O RESTO DO DINHEIRO
        //Y FUNC RODA O MOTOR CODIGO
        //Y DEVOLVE O TROCO
return 0;
}