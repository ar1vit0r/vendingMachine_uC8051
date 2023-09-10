#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#include "lcd.h"
#include "delay.h"
#include "keyboard.h"
#include "routines.h"
#include "vendingMachine.h"
#include "int0.h"

#include "lcd.c"
#include "delay.c"
#include "keyboard.c"
#include "routines.c"
#include "vendingMachine.c"
#include "int0.c"

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

//Money Insert
#define PLUS1 P3_0
#define PLUS10 P3_1
#define MINUS1 P3_3
#define MINUS10 P3_4

extern float money;

// LCD pins
#define LCD			P2
#define EN			P3_7
#define RS			P3_6

#define MOTOR_MATRIX_LINE 3
#define MOTOR_MATRIX_COLLUM 3
#define MAX_PRODUCT 10

int PRODUCT[MOTOR_MATRIX_LINE][MOTOR_MATRIX_COLLUM];

int DIGIT1 = 0;
int DIGIT2 = 0;

// Define product prices
float product_prices[9];

int selected_product = 0;
float total_price = 0.0;
float inserted_amount = 0.0;
int input = 0;

int isMoneyinsert = 1;

#endif