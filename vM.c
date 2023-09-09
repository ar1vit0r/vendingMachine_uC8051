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
#include <at89x52.h>
#indlude lcd.h
#include delay.h
#include keyboard.h


// keyboard pins
#define LIN0 P1_0
#define LIN1 P1_1
#define LIN2 P1_2
#define LIN3 P1_3
#define COL0 P1_4
#define COL1 P1_5
#define COL2 P1_6

#define MOTOR0 P0_0
#define MOTOR1 P0_1
#define MOTOR2 P0_2
#define MOTOR3 P0_3

// LCD pins
#define LCD			P2
#define EN			P3_7
#define RS			P3_6

// delay function
void DELAY(unsigned int ms);
void Delay5ms(void);
void Delay5us(void);

//keyboard function
void CHECK_LINES(int *input) { };	// check the keyboard lines and return the input

// LCD functions
void ConfigLCD(void);
void Line1(void);
void Line2(void);
void WriteMSG(char msg[]);
void WrCMD(void);
void WrCHAR(void);

// Util Functions
					//int convertBCD(int decimal);

// Vending machine functions
void returnInsertedMoney(float *inserted_amount) { };
void dispenseProduct(int *selected_product) { 
	int binary[3];
	int i = 0;

	// Caso especial: se o valor decimal for zero, o binário também é zero
	if (*selected_product == 0) {
		MOTOR0 = 0;
		MOTOR1 = 0;
		MOTOR2 = 0;
		MOTOR3 = 0;
	}

	// Converte decimal para binário
	while (*selected_product > 0) {
			binary[i] = *selected_product % 2;
			decimal = *selected_product / 2;
			i++;
	}
		MOTOR0 = binary[0];
		MOTOR1 = binary[1];
		MOTOR2 = binary[2];
		MOTOR3 = binary[3];
};
void dispenseChange(float change) { };
void dispenseProductAndChange(int *selected_product, float change) { };
void waitTheRestOfcash(float *total_price, float *inserted_amount) { };
void vendingMachine(int *selected_product, float *total_price, float *inserted_amount) { };
int checkIfNeedChange(float *inserted_amount, float *total_price) { };

// Initialize product matrix 6x5
int product_matrix[6][5] = {       // the value of each position indicate the stock, 10 to all initially
    {10, 10, 10, 10, 10},          // 0 to 4
    {10, 10, 10, 10, 10},          // 5 to 9
    {10, 10, 10, 10, 10},          // 10 to 14
    {10, 10, 10, 10, 10},          // 15 to 19
    {10, 10, 10, 10, 10},          // 20 to 24
    {10, 10, 10, 10, 10},          // 25 to 29
};

// Initialize servo motor positions for each product
int servo_positions[30];

// Define product prices
float product_prices[30];

int main() {
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
            WriteMSG("* Invalid product numbers. *");
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








