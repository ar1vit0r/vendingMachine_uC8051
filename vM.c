#include <stdio.h>
//#include <at89c51ed2.h>

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

void returnInsertedMoney(float *inserted_amount) { };
void dispenseProduct(int *selected_product) { };
void moveServo(int *selected_product) { };
void dispenseChange(float change) { };
void dispenseProductAndChange(int *selected_product, float change) { };
void waitTheRestOfcash(float *total_price, float *inserted_amount) { };
void vendingMachine(int *selected_product, float *total_price, float *inserted_amount) { };
int checkIfNeedChange(float *inserted_amount, float *total_price) { };

int main() {
    // Initialize servo_positions and product_prices arrays
    int *selected_product = (int *)malloc(sizeof(int));
    *selected_product = 0;
    float *total_price = (float *)malloc(sizeof(float));
    *total_price = 0.0;
    float *inserted_amount = (float *)malloc(sizeof(float));
    *inserted_amount = 0.0;

    vendingMachine(selected_product, total_price, inserted_amount);

    return 0;
}

void vendingMachine(int *selected_product, float *total_price, float *inserted_amount) {
    while (1) {
        char input[3];

        printf("Enter product number (01-30), '*' to cancel, or '#' to confirm: ");
        scanf("%s", input);

        if (input[0] == '*' && input[1] == '\0') {                              // case '*' to cancel the transaction and return any inserted money.
            // Cancel the transaction and return any inserted money
            printf("Transaction canceled. Returning inserted money.\n");
            returnInsertedMoney(inserted_amount);
            break;
        } else if (input[0] == '#' && input[1] == '\0') {                       // case '#' to confirm the transaction and dispense the product, if the amount is correct.
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
        } else if (input[0] >= '0' && input[0] <= '3' && input[1] >= '1' && input[1] <= '9') {  // 
            // Two numbers (0-9) are typed, combine them to form the product number (1-30)
            int tens_digit = input[0] - '0';
            int ones_digit = input[1] - '0';
            *selected_product = product_matrix[tens_digit][ones_digit];

            if (*selected_product >= 1 && *selected_product <= 30) {
                // Calculate the total price of the selected product
                *total_price = product_prices[*selected_product];

                // Accept and validate coins until the total amount is reached
                while (*inserted_amount < *total_price) {
                    float inserted_coin;
                    printf("Insert a coin or bill: ");
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