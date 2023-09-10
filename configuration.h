
#include "lcd.h"
#include "delay.h"
#include "keyboard.h"
#include "routines.h"
#include "vendingMachine.h"

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

// LCD pins
#define LCD			P2
#define EN			P3_7
#define RS			P3_6

#define MOTOR_MATRIX_SIZE 3
#define MAX_PRODUCT 10

int PRODUCT[MOTOR_MATRIX_SIZE][MOTOR_MATRIX_SIZE];

int DIGIT1 = 0;
int DIGIT2 = 0;

int selected_product = 0;
float total_price = 0.0;
float inserted_amount = 0.0;
int input = 0;