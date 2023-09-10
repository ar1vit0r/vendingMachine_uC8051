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
#include <stdio.h>
#include <stdlib.h>
#include <at89x52.h>
<<<<<<< HEAD
//#include "lcd.h"
//#include "delay.h"
//#include "keyboard.h"
#include "routines.h"
//#include "vendingMachine.h"
//#include "configuration.h"
=======
#include "lcd.h"
#include "delay.h"
#include "keyboard.h"
>>>>>>> 4cedd5dffba0e30a87900aebe8d2c6f0720539a6



// Initialize servo motor positions for each product
int servo_positions[30];

// Define product prices
float product_prices[30];

void main() {
	start();
	
	while(1){
/////KEYBOARD
		//CHECA KEYBOARD, CASO HAJA ENTRADA DE 1 DIGITO (DIFERENTE DE * OU #), DESABILITA A INTERRUPÇÃO
		//CHAMA A ROTINA DE INSERÇÃO DE CÓDIGO E DESABILITA A INTERRUPÇÃO DE ENTRADA DE DINHEIRO ATÉ COLOCAR 2ºCOD E PRESSIONAR ENTER (#)
		//PRESSIONADO #, HABILITA A INTERRUPÇÃO
		//AVISA O PREÇO DO PRODUTO
		//ESPERA POR 30S
			//WHILE
				//SE COLOCAR DINHEIRO < VALOR DO PRODUTO
					//ESPERA POR MAIS 30S
				//DO CONTRARIO DEVOLVE O DINHEIRO E AVISA DO ERRO
		//VOLTA A CHECAR O KEYBOARD
/////DINHEIRO
		//SE HOUVER INTERRUPÇÃO DE DINHEIRO
		//INFORMA A QUANTIA INSERIDA
		//ESPERA 30S
			//SE NAO HOUVER MAIS INTERAÇÃO DEVOLVE O DINHEIRO
			//SE CANCELAR (*) A OPERAÇÃO
				//DEVOLVE O DINHEIRO
				//VOLTA A CHECAR O KEYBOARD
			//SE ACRESCENTAR DINHEIRO, ATUALIZA A QUANTIA INSERIDA
				//ESPERA 30S
			//SE PRESSIONAR QUALQUER DIGITO
				//PEDE O 2º DIGITO
				//ESPERA 30S
				//SE CANCELAR (*) A OPERAÇÃO
					//DEVOLVE O DINHEIRO
					//VOLTA A CHECAR O KEYBOARD
				//SE NAO HOUVER MAIS INTERAÇÃO DEVOLVE O DINHEIRO
					//VOLTA A CHECAR O KEYBOARD
				//SE COLOCAR O 2º DIGITO
				//INFORMA O VALOR E A DIFERENÇA
					//ESPERA 30S
					//SE CANCELAR (*) A OPERAÇÃO
						//DEVOLVE O DINHEIRO
						//VOLTA A CHECAR O KEYBOARD
					
					//SE PRESCIONAR ENTER (#)
						//INFORMA A DIFERENÇA (VALOR INSERIDO - VALOR DO PRODUTO)
						//SE A DIFERENA FOR 0
							//FORNECE O PRODUTO
						//SE A DIFERENÇA FOR <0
							//FORNECE O PRODUTO
							//DEVOLVE O TROCO
						//SE A DIFERENÇA FOR >0
							//INFORMA A QUANTIA QUE FALTA
							//ESERA 30S
							//SE CANCELAR (*) A OPERAÇÃO
								//DEVOLVE O DINHEIRO
								//VOLTA A CHECAR O KEYBOARD
							//SE COLOCAR DINHEIRO
								//VOLTA A INFORMAR A DIFERENÇA
				
		
		
		
	}
    // Initialize servo_positions and product_prices arrays
    int *selected_product = (int *)malloc(sizeof(int));
    *selected_product = 0;
    float *total_price = (float *)malloc(sizeof(float));
    *total_price = 0.0;
    float *inserted_amount = (float *)malloc(sizeof(float));
    *inserted_amount = 0.0;
    int *input = malloc(sizeof(int));
    *input = 0;

    vendingMachine(input, selected_product, total_price, inserted_amount);

    free(selected_product);
    free(total_price);
    free(inserted_amount);
    free(input);

<<<<<<< HEAD
    //return 0;
}
=======
    return 0;
}

void vendingMachine(int *input, int *selected_product, float *total_price, float *inserted_amount) {
    int digit1 = 0, digit2 = 0;

    ConfigLCD();
    Line1();
    WriteMSG("* Insert the COD of product *");
    Line2();
    WriteMSG("  or the money ");
    //while(1);    
    
    CHECK_LINES(input);
    if( *input == 10 || *input == 12) {                                                     // case '*' to cancel the transaction and return any inserted money.
        ConfigLCD();
        Line1();
        WriteMSG("* Transaction canceled *");
        Line2();
        WriteMSG(" Returning inserted money ");
        //while(1);                                                                       
        printf("Transaction canceled. Returning inserted money.\n");
        returnInsertedMoney(inserted_amount);
        break;
    }else{
        if( *input == 11) {                                                 // case '0' to cancel the transaction and return any inserted money.
            digit1 = 0;
        }else
            digit1 = *input;
    }

    ConfigLCD();
    Line1();
    WriteMSG("* Insert the second digit *");
    Line2();
    WriteMSG("  or type '*' to cancel ");
    //while(1);    
    
    CHECK_LINES(input);
    if( *input == 10 || *input == 12) {                                                     // case '*' to cancel the transaction and return any inserted money.
        ConfigLCD();
        Line1();
        WriteMSG("* Transaction canceled *");
        Line2();
        WriteMSG(" Returning inserted money ");
        //while(1);                                                                       
        printf("Transaction canceled. Returning inserted money.\n");
        returnInsertedMoney(inserted_amount);
        break;
    }else{
        if( *input == 11) {                                                 // case '0' to cancel the transaction and return any inserted money.
            digit2 = 0;
        }else
            digit2 = *input;
    }

    *input = digit1 * 10 + digit2;                                        // the value of the input is the concat of the 2 digits       
    *selected_product = product_matrix[digit1][digit2];                   // here we have the stock of selected product

    if (*input >= 1 && *input <= 30) {                                    // check if the input is a valid product number
        if (*selected_product >= 1 ) {                                    // check if the selected product is in stock
            // Calculate the total price of the selected product
            *total_price = product_prices[*selected_product];

            // Accept and validate coins until the total amount is reached
            while (*inserted_amount < *total_price) {
                float inserted_coin;
                ConfigLCD();
                Line1();
                WriteMSG("* Insert the money *");
                Line2();
                WriteMSG(" alguma coisa aqui");
                //while(1);    
                printf("Insert a coin: ");
                scanf("%f", &inserted_coin);
                *inserted_amount += inserted_coin;
            }
            
            if (*inserted_amount == *total_price) {
                // No change to be dispensed
                // Move servo motor associated with selected_product
                ConfigLCD();
                Line1();
                WriteMSG("* Dispensing product *");
                Line2();
                WriteMSG(" Thank you for using the vending machine ");
                //while(1);        
                printf("Dispensing product %d...\n", selected_product);
                printf("Product delivered at servo position %d degrees.\n", servo_positions[*selected_product]);
                printf("Thank you for using the vending machine!\n");
                break;
            } else if (*inserted_amount > *total_price) {
                // Provide correct change and dispense the product
                // Move servo motor associated with selected_product
                ConfigLCD();
                Line1();
                WriteMSG("* Dispeling change *");
                Line2();
                WriteMSG(" Thank you for using the vending machine ");
                //while(1);        
                printf("Dispensing product %d...\n", selected_product);
                printf("Product delivered at servo position %d degrees.\n", servo_positions[*selected_product]);
                float change = *inserted_amount - *total_price;
                printf("Change: $%.2f\n", change);
                printf("Thank you for using the vending machine!\n");
                break;
            } else {
                // The amount isn't correct, continue accepting money
                while(*inserted_amount < *total_price) {
                    float inserted_coin;
                    ConfigLCD();
                    Line1();
                    WriteMSG("* Insufficient funds. *");
                    Line2();
                    WriteMSG(" Please insert more coins ");
                    //while(1);        
                    printf("Insert a coin: ");
                    scanf("%f", &inserted_coin);
                    *inserted_amount += inserted_coin;
                    printf("Insufficient funds. Please insert more coins.\n");
                    waitTheRestOfcash(total_price, inserted_amount);
                }
            }
        } else {
            ConfigLCD();
            Line1();
            WriteMSG("* Product out of stock *");
            Line2();
            WriteMSG(" Returning inserted money ");
            //while(1);      
            printf("Invalid product number.\n");
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
    //O DINHEIRO É SUFICIENTE
        //Y FUNC RODA O MOTOR CODIGO
        //Y DEVOLVE O TROCO
    //O DINHEIRO N É SUFICIENTE
        //Y FUNC ESPERA O RESTO DO DINHEIRO
        //Y FUNC RODA O MOTOR CODIGO
        //Y DEVOLVE O TROCO
return 0;
}








>>>>>>> 4cedd5dffba0e30a87900aebe8d2c6f0720539a6
