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

// delay function
void DELAY(unsigned int ms);

//keyboard functions
void CHECK_LINE0(int *input) { };	
void CHECK_LINE1(int *input) { };
void CHECK_LINE2(int *input) { };
void CHECK_LINE3(int *input) { };

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
    while (1) {             // loop to check the keyboard and get the input, until the user press '*' or '#' key. Need a logic to take 2 digits input.
        CHECK_LINE0(input);
        DELAY(100);
        CHECK_LINE1(input);
        DELAY(100);
        CHECK_LINE2(input);
        DELAY(100);
        CHECK_LINE3(input);

        if (*input == 10) {                                                     // case '*' to cancel the transaction and return any inserted money.
            printf("Transaction canceled. Returning inserted money.\n");
            returnInsertedMoney(inserted_amount);
            break;
        } else if (*input == 12) {                                              // case '#' to confirm the transaction and dispense the product, if the amount is correct.
            if (*inserted_amount < *total_price) {                              // case the amount is not correct, wait the rest of cash.
                printf("Insufficient funds. Transaction canceled.\n");
                waitTheRestOfcash(total_price, inserted_amount);
            } else {                                                            // Dispense the selected product and provide change, if necessary
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
            int tens_digit = input[0] - '0';
            int ones_digit = input[1] - '0';
            *selected_product = product_matrix[tens_digit][ones_digit];

            if (*selected_product >= 1 && *selected_product <= 30) {
                // Calculate the total price of the selected product
                *total_price = product_prices[*selected_product];

                // Accept and validate coins until the total amount is reached
                while (*inserted_amount < *total_price) {
                    float inserted_coin;
                    printf("Insert a coin: ");
                    scanf("%f", &inserted_coin);
                    *inserted_amount += inserted_coin;
                }
                
                if (*inserted_amount == *total_price) {
                    // No change to be dispensed
                    // Move servo motor associated with selected_product
                    printf("Dispensing product %d...\n", selected_product);
                    printf("Product delivered at servo position %d degrees.\n", servo_positions[*selected_product]);
                    printf("Thank you for using the vending machine!\n");
                    break;
                } else if (*inserted_amount > *total_price) {
                    // Provide correct change and dispense the product
                    // Move servo motor associated with selected_product
                    printf("Dispensing product %d...\n", selected_product);
                    printf("Product delivered at servo position %d degrees.\n", servo_positions[*selected_product]);
                    float change = *inserted_amount - *total_price;
                    printf("Change: $%.2f\n", change);
                    printf("Thank you for using the vending machine!\n");
                    break;
                } else {
                    // The amount isn't correct, continue accepting money
                }
            } else {
                printf("Invalid product number.\n");
            }
        } else {
            printf("Invalid input. Please enter a valid product number (01-30), '*' to cancel, or '#' to confirm.\n");
        }
    }
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

void CHECK_LINE0(int *input) {
    LIN0 = 0;
    LIN1 = 1;
    LIN2 = 1;
    LIN3 = 1;
    
    if (!COL0) {
        *input = 1;
    }
    if (!COL1) {
        *input = 2;
    }
    if (!COL2) {
        *input = 3;
    }
}

void CHECK_LINE1(int *input) {
    LIN0 = 1;
    LIN1 = 0;
    LIN2 = 1;
    LIN3 = 1;
    
    if (!COL0) {
        *input = 4;
    }
    if (!COL1) {
        *input = 5;
    }
    if (!COL2) {
        *input = 6;
    }
}

void CHECK_LINE2(int *input) {
    LIN0 = 1;
    LIN1 = 1;
    LIN2 = 0;
    LIN3 = 1;
    
    if (!COL0) {
        *input = 7;
    }
    if (!COL1) {
        *input = 8;
    }
    if (!COL2) {
        *input = 9;
    }
}

void CHECK_LINE3(int *input) {
    LIN0 = 1;
    LIN1 = 1;
    LIN2 = 1;
    LIN3 = 0;
    
    if (!COL0) {
        *input = 10;        // '*' key
    }
    if (!COL1) {
        *input = 11;        // '0' key 
    }
    if (!COL2) {
        *input = 12;        // '#' key
    }
}
