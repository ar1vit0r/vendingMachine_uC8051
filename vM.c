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
// 										  Nome da(o) aluna(o) & Matr�cula: Ari Vitor da Silva Lazzarotto, 17200917
// 										  Nome da(o) aluna(o) & Matr�cula: Adriel Correa Matielo, 16105321
//										  Data: 12/09/2023			
//
//======================================= 
#include <stdio.h>
#include <at89x52.h>

// keyboard pins
#define LIN0 P1_0
#define LIN1 P1_1
#define LIN2 P1_2
#define LIN3 P1_3
#define COL0 P1_4
#define COL1 P1_5
#define COL2 P1_6

// LCD pins
#define LCD			P2
#define EN			P3_7
#define RS			P3_6

// delay function
void DELAY(unsigned int ms);

//keyboard function
void CHECK_LINES(int *input) { };	// check the keyboard lines and return the input

// LCD functions
void ConfigLCD(void);
void Line1(void);
void Line2(void);
void WriteMSG(char msg[]);
void Delay5us(void);
void Delay5ms(void);
void WrCMD(void);
void WrCHAR(void);

// Vending machine functions
void returnInsertedMoney(float *inserted_amount) { };
void dispenseProduct(int *selected_product) { };
void moveServo(int *selected_product) { };
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
    if( *input == 10) {                                                     // case '*' to cancel the transaction and return any inserted money.
        ConfigLCD();
        Line1();
        WriteMSG("* Transaction canceled *");
        Line2();
        WriteMSG(" Returning inserted money ");
        //while(1);                                                                       
        printf("Transaction canceled. Returning inserted money.\n");
        returnInsertedMoney(inserted_amount);
        break;
    }else
        digit1 = *input;

    ConfigLCD();
    Line1();
    WriteMSG("* Insert the second digit *");
    Line2();
    WriteMSG("  or type '*' to cancel ");
    //while(1);    
    
    CHECK_LINES(input);
    if( *input == 10) {                                                     // case '*' to cancel the transaction and return any inserted money.
        ConfigLCD();
        Line1();
        WriteMSG("* Transaction canceled *");
        Line2();
        WriteMSG(" Returning inserted money ");
        //while(1);                                                                       
        printf("Transaction canceled. Returning inserted money.\n");
        returnInsertedMoney(inserted_amount);
        break;
    }else
        digit2 = *input;

    *input = digit1 * 10 + digit2;                                        // the value of the input is the concat of the 2 digits       
    *selected_product = product_matrix[digit1][digit2];                   // here we have the selected product

    if (*input == 10) {                                                     // case '*' to cancel the transaction and return any inserted money.
        ConfigLCD();
        Line1();
        WriteMSG("* Transaction canceled *");
        Line2();
        WriteMSG(" Returning inserted money ");
        //while(1);                                                                       
        printf("Transaction canceled. Returning inserted money.\n");
        returnInsertedMoney(inserted_amount);
        break;
    } else if (*input == 12) {                                              // case '#' to confirm the transaction and dispense the product, if the amount is correct.
        if (*inserted_amount < *total_price) {                              // case the amount is not correct, wait the rest of cash.
            ConfigLCD();
            Line1();
            WriteMSG("* Insufficient funds *");
            Line2();
            WriteMSG(" Please insert more coins ");
            //while(1);    
            printf("Insufficient funds. Transaction canceled.\n");
            waitTheRestOfcash(total_price, inserted_amount);
        } else {                                                            // Dispense the selected product and provide change, if necessary
            ConfigLCD();
            Line1();
            WriteMSG("* Dispensing product *");
            Line2();
            WriteMSG(" Thank you for using the vending machine ");
            //while(1);    
            printf("Dispensing product %d...\n", selected_product);
            printf("Product delivered at servo position %d degrees.\n", servo_positions[*selected_product]);
            if(checkIfNeedChange(inserted_amount, total_price))
                dispenseProductAndChange(selected_product, *inserted_amount - *total_price);
            else{
                dispenseProduct(selected_product);
                printf("Thank you for using the vending machine!\n");
                break;
            }
        }
    } else if (*input >= 1 && *input <= 30) {                               // case the input is a valid product number, check if the product is available.
        if (*selected_product >= 1 && *selected_product <= 30) {
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

void DELAY(unsigned int ms) {       // delay vai ser diferente do esperado, mas deveria funcionar
	TMOD |= 0x01;					// A operaço OU preserva alguma eventual configuraço previa do T/C 1.
	
	while(ms){
		TH0 = 0xFC;					// Valor de recarga para 1 ms @ f = 12 MHz (i.e. 64535).
		TL0 = 0x17;
		TR0 = 1;
		while(!TF0);
		TF0 = 0;
		TR0 = 0;
		ms--;
	}
}

void CHECK_LINES(int *input) {      // loop to check the keyboard and get the input, until the user press '*' or '#' key. Need a logic to take 2 digits input.
    while(1) {
        DELAY(100);

        LIN0 = 0;
        LIN1 = 1;
        LIN2 = 1;
        LIN3 = 1;
        
        if (!COL0) {
            *input = 1;
            DELAY(100);
            break;
        }
        if (!COL1) {
            *input = 2;
            DELAY(100);
            break;
        }
        if (!COL2) {
            *input = 3;
            DELAY(100);
            break;
        }

        LIN0 = 1;
        LIN1 = 0;
        LIN2 = 1;
        LIN3 = 1;
        
        if (!COL0) {
            *input = 4;
            DELAY(100);
            break;
        }
        if (!COL1) {
            *input = 5;
            DELAY(100);
            break;
        }
        if (!COL2) {
            *input = 6;
            DELAY(100);
            break;
        }

        LIN0 = 1;
        LIN1 = 1;
        LIN2 = 0;
        LIN3 = 1;
        
        if (!COL0) {
            *input = 7;
            DELAY(100);
            break;
        }
        if (!COL1) {
            *input = 8;
            DELAY(100);
            break;
        }
        if (!COL2) {
            *input = 9;
            DELAY(100);
            break;
        }

        LIN0 = 1;
        LIN1 = 1;
        LIN2 = 1;
        LIN3 = 0;
        
        if (!COL0) {
            *input = 10;        // '*' key
            DELAY(100);
            break;
        }
        if (!COL1) {
            *input = 11;        // '0' key 
            DELAY(100);
            break;
        }
        if (!COL2) {
            *input = 12;        // '#' key
            DELAY(100);
            break;
        }
    }
}

void ConfigLCD(void)
{
	LCD = 0x38;
	WrCMD();
	LCD = 0x06;
	WrCMD();
	LCD = 0x0E;
	WrCMD();
	LCD = 0x01;
	WrCMD();
}

void Line1(void)
{	
	LCD = 0x00;
	WrCMD();
}

void Line2(void)
{
	LCD = 0xC0;
	WrCMD();
}

void WriteMSG(char msg[])
{
	unsigned char i;
	for(i = 0; i < 16; i++)
	{
		LCD = msg[i];
		WrCHAR();
	}
}

void Delay5us(void)
{
	unsigned char i;
	for(i = 0; i < 5; i++){}
}

void Delay5ms(void)
{
	DELAY(5);
}

void WrCMD(void)
{
	RS = 0;
	EN = 1;
	Delay5us();
	EN = 0;
	Delay5ms();
}

void WrCHAR(void)
{
	RS = 1;
	EN = 1;
	Delay5us();
	EN = 0;
	Delay5ms();
}