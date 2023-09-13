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
#include "C:\Program Files\Keil_v5\C51\INC\Atmel\at89x52.h"
#include <stdio.h>

//pins P0
#define MOTOR0 P0_0
#define MOTOR1 P0_1
#define MOTOR2 P0_2
#define MOTOR3 P0_3
//pins P1
#define LINA    P1_0
#define LINB    P1_1
#define LINC    P1_2
#define LIND    P1_3
#define COL1    P1_4
#define COL2    P1_5
#define COL3    P1_6
//pins P2
#define LCD     P2
//pin P3
#define PLUS1   P3_0
#define PLUS10  P3_1
#define insert  P3_2
#define MINUS1  P3_3
#define MINUS10 P3_4

#define EN      P3_6
#define RS      P3_7
//Products
#define MAX_PRODUCT 9

#define PRICE_PRODUCT_0 6
#define PRICE_PRODUCT_1 10
#define PRICE_PRODUCT_2 7
#define PRICE_PRODUCT_3 13
#define PRICE_PRODUCT_4 2
#define PRICE_PRODUCT_5 15
#define PRICE_PRODUCT_6 20
#define PRICE_PRODUCT_7 18
#define PRICE_PRODUCT_8 1

#define COD_PRODUCT_0 00
#define COD_PRODUCT_1 01
#define COD_PRODUCT_2 02
#define COD_PRODUCT_3 10
#define COD_PRODUCT_4 11
#define COD_PRODUCT_5 12
#define COD_PRODUCT_6 20
#define COD_PRODUCT_7 21
#define COD_PRODUCT_8 22
//value of times in ms
#define TIME_WAIT 10000
#define LITTLE_WAIT 3000

//LCD
void ConfigLCD();
void WriteMSG(char msg[]);
void Line1();
void Line2();
void WrCMD();
void WrCHAR();

//timers
void timer0(unsigned int ms);   //like an alarm
void delayMs0(unsigned int ms); //wait for the delay to end
void timer1(unsigned int ms);   //like an alarm
void delayMs1(unsigned int ms); //wait for the delay to end
void Delay5us();                //wait for the delay to end

//machine
void start();                   //start registers and variables
//unsigned int startMoney();    //start machine with money
unsigned int startCode();       //start machine with code



unsigned int giveMeTheMoney();  //request money for product | when nows the code
void dispenseProduct();         //set motors of respective COD_PRODUCT
void convertIntToBinary();      //

unsigned int scanKeyboard();    //scan keyboard 4x3

//flags of interruption 8051
unsigned int TIMER0;            //Flag TF0
unsigned int TIMER1;            //Flag TF1
unsigned int EXTER0;            //Flag IE0
unsigned int EXTER1;            //Flag IE1
unsigned int moneyIsReady;      //Flag IE0

//global variables
unsigned int digit1;            //1st digit of COD_PRODUCT
unsigned int digit2;            //2nd digit of COD_PRODUCT
unsigned int digit12;           //COD_PRODUCT
unsigned int amount;            //amount of money entered
unsigned int price;             //price of COD_PRODUCT
unsigned int dispenser;         //position of COD_PRODUCT = MOTOR
unsigned int priceProduct[MAX_PRODUCT] = {PRICE_PRODUCT_0, PRICE_PRODUCT_1, PRICE_PRODUCT_2,
                                          PRICE_PRODUCT_3, PRICE_PRODUCT_4, PRICE_PRODUCT_5,
                                          PRICE_PRODUCT_6, PRICE_PRODUCT_7, PRICE_PRODUCT_8};
unsigned int codeProduct[MAX_PRODUCT] = {COD_PRODUCT_0, COD_PRODUCT_1, COD_PRODUCT_2,
                                         COD_PRODUCT_3, COD_PRODUCT_4, COD_PRODUCT_5,
                                         COD_PRODUCT_6, COD_PRODUCT_7, COD_PRODUCT_8};

void main() {
    start:
    start();
    while(1){
        digit1 = scanKeyboard();
        //10 == '*' | 12 == '#' | 77 == void
        if((digit1 != 10) && (digit1 != 12) && (digit1 != 77)){
            if (startCode())
                if (giveMeTheMoney())
                    dispenseProduct();
            goto start;
        }
        //start with money
        //if (EX0) {
        //    //startMoney();
        //    goto start;
        //}
    }
}

unsigned int scanKeyboard() {
    while(1) {
        delayMs0(100);
        LINA = 0;
        LINB = 1;
        LINC = 1;
        LIND = 1;
        if (!COL1) {
            delayMs0(100);
            return 1;
        }
        if (!COL2) {
            delayMs0(100);
            return 2;
        }
        if (!COL3) {
            delayMs0(100);
            return 3;
        }

        LINA = 1;
        LINB = 0;
        LINC = 1;
        LIND = 1;
        if (!COL1) {
            delayMs0(100);
            return 4;
        }
        if (!COL2) {
            delayMs0(100);
            return 5;
        }
        if (!COL3) {
            delayMs0(100);
            return 6;
        }

        LINA = 1;
        LINB = 1;
        LINC = 0;
        LIND = 1;
        if (!COL1) {
            delayMs0(100);
            return 7;
        }
        if (!COL2) {
            delayMs0(100);
            return 8;
        }
        if (!COL3) {
            delayMs0(100);
            return 9;
        }

        LINA = 1;
        LINB = 1;
        LINC = 1;
        LIND = 0;
        if (!COL1) {
            //'*' key
            delayMs0(100);
            return 10;
        }
        if (!COL2) {
            // '0' key
            delayMs0(100);
            return 11;
        }
        if (!COL3) {
            // '#' key
            delayMs0(100);
            return 12;
        }
        if (TIMER1)
            return 77;
    }
}

void ConfigLCD(){
    LCD = 0x38;
    WrCMD();
    LCD = 0x06;
    WrCMD();
    LCD = 0x0E;
    WrCMD();
    LCD = 0x01;
    WrCMD();
}
void WriteMSG(char msg[]){
    unsigned char i;
    for(i = 0; i <= 16; i++){
        LCD = msg[i];
        WrCHAR();
    }
}
void WriteMSG_DIGIT1(char msg[]){
    unsigned char i;
    for(i = 0; i <= 16; i++){
        if(i==7) {
            if (digit1 == 11) {
                LCD = '0';
            } else {
                LCD = '0' + digit1;
            }
        }else if(i==8)
                LCD = '*';
        else
            LCD = msg[i];
        WrCHAR();
    }
}
void WriteMSG_DIGIT2(char msg[]){
    unsigned char i;
    for(i = 0; i <= 16; i++){
        if(i==7) {
            if (digit1 == 11) {
                LCD = '0';
            } else {
                LCD = '0' + digit1;
            }
        }else if(i==8) {
            if (digit2 == 11) {
                LCD = '0';
            } else {
                LCD = '0' + digit2;
            }
        }
        else
            LCD = msg[i];
        WrCHAR();
    }
}
void Line1(){
    LCD = 0x00;
    WrCMD();
}
void Line2(){
    LCD = 0xC0;
    WrCMD();
}
void WrCMD(){
    RS = 0;
    EN = 1;
    delayMs0(5);
    EN = 0;
    delayMs0(5);
}
void WrCHAR(){
    RS = 1;
    EN = 1;
    delayMs0(5);
    EN = 0;
    delayMs0(5);
}

void msg_CLEANER(){
    LCD = 0x01;
    WrCMD();
}
void msg_Start(){
    msg_CLEANER();
    ConfigLCD();
    Line1();
    delayMs0(5);
    WriteMSG(" Insert product ");
    Line2();
    delayMs0(5);
    WriteMSG(" code or money  ");
}
void msg_InsertCode2(){
    msg_CLEANER();
    ConfigLCD();
    Line1();
    WriteMSG("   Second code  ");
    Line2();
    WriteMSG_DIGIT1("                ");
}
void msg_CanceledByUsr(){
    msg_CLEANER();
    ConfigLCD();
    Line1();
    WriteMSG("    Canceled    ");
    Line2();
    WriteMSG("  By custumer   ");
}
void msg_Code2Required(){
    msg_CLEANER();
    ConfigLCD();
    Line1();
    WriteMSG("   Second code! ");
    Line2();
    WriteMSG("    # Denied!   ");
}
void msg_Code2Insert(){
    msg_CLEANER();
    ConfigLCD();
    Line1();
    WriteMSG("  Product code: ");
    Line2();
    WriteMSG_DIGIT2("                ");
}
void msg_IllegalCode(){
    msg_CLEANER();
    ConfigLCD();
    Line1();
    WriteMSG("Illegal Operatio");
    Line2();
    WriteMSG(" Code not exist ");
}
void msg_TimeOut_Code2_WTMoney(){
    msg_CLEANER();
    ConfigLCD();
    Line1();
    WriteMSG(" Time is Over!  ");
    Line2();
    WriteMSG("Vending restarte");
}
void msg_insertMoney(){
    msg_CLEANER();
    ConfigLCD();
    Line1();
    WriteMSG(" Amount of money");
    Line2();
    WriteMSG_Amount("      $**       ");
}
void msg_ConfirmBuy(){
    msg_CLEANER();
    ConfigLCD();
    Line1();
    WriteMSG("'#' for confirm ");
    Line2();
    WriteMSG("'#' for confirm ");
}
void msg_CanceledByUsr_money(unsigned int value){
    msg_CLEANER();
    ConfigLCD();
    Line1();
    WriteMSG("    Canceled    ");
    WriteMSG_Change("  Change: $**   ");
}
void msg_dispenseProduct(unsigned int value){
    msg_CLEANER();
    ConfigLCD();
    Line1();
    WriteMSG("   Successful   ");
    Line2();
    WriteMSG("                ");
}
void msg_timeoutWithMoney(unsigned int value){
    msg_CLEANER();
    ConfigLCD();
    Line1();
    WriteMSG("  Time is Out!  ");
    Line2();
    WriteMSG("  Time is Out!  ");
}
void msg_WorkingOnDispenser(){
    msg_CLEANER();
    ConfigLCD();
    Line1();
    WriteMSG("   Working on   ");
    Line2();
    WriteMSG("    request!    ");
}
void msg_done(){
    msg_CLEANER();
    ConfigLCD();
    Line1();
    WriteMSG("      Done      ");
    Line2();
    WriteMSG("    Enjoy! :)   ");
}

void start() {
    msg_Start();
    P0 = 1;
    //IE = 0x8F;
    moneyIsReady = 0;
    TMOD = 0x11;
    digit1 = 77;
    digit2 = 77;
    digit12 = 0;
    price = 0;
    TIMER0 = 1;
    TIMER1 = 1;
    amount = 0;
}
int codeValidation(){
    for(dispenser = 0; dispenser <= MAX_PRODUCT; dispenser++) {
        if (digit12 == codeProduct[dispenser]) {
            price = priceProduct[dispenser];
            return 1;
        }
    }
    return 0;
}
 int sumOfMoney(){
    if(!PLUS1)
        return 1;
    if(!PLUS10)
        return 10;
    if(!MINUS1)
        return -1;
    if(!MINUS10)
        return -10;
}
unsigned int changeOfCanceled(){
    return amount;
}
unsigned int change(){
    return amount - price;
}

unsigned int startCode(){
    IE1 = 1; //interrupt of money disabled
    msg_InsertCode2();
    delayMs1(LITTLE_WAIT); //time for read msg
    timer1(TIME_WAIT);
    while(TIMER1){
        digit2 = scanKeyboard();
        //10 == '*' | CANCEL
        if (digit2 == 10){
            msg_CanceledByUsr();
            timer1(TIME_WAIT);
            delayMs1(LITTLE_WAIT); //time for read msg
            return 0; //canceled by user | without money
        }
        //12 == '#' CONFIRM | required digit 2
        if(digit2 == 12){
            msg_Code2Required();
            digit2 = 77;
            timer1(TIME_WAIT);
            //digit 2 is a number
        }if (digit2 != 77) {
            digit12 = digit1 * 10 + digit2;
            msg_Code2Insert();
            delayMs1(LITTLE_WAIT); //time for read msg
            if (codeValidation()) {
                return 1; //digit 1 ok, digit2 ok | without money
            } else {
                msg_IllegalCode();
                timer1(TIME_WAIT);
                delayMs1(LITTLE_WAIT); //time for read msg
                return 0; //canceled | illegal code
            }
        }
    }
    msg_TimeOut_Code2_WTMoney();
    delayMs1(LITTLE_WAIT); //time for read msg
    return 0; //digit1 ok, digit2 timeout | without money
}

unsigned int giveMeTheMoney(){
    IE0 = 0; //interrupt of money enable
    msg_insertMoney();
    delayMs1(LITTLE_WAIT); //time for read msg
    timer1(TIME_WAIT);
    while(TIMER1){
        if(moneyIsReady) {
            amount += sumOfMoney();
            timer1(TIME_WAIT);
            moneyIsReady = 0;
        }
        if (amount >= price){
            timer1(TIME_WAIT);
            msg_ConfirmBuy();
            delayMs1(LITTLE_WAIT);
            timer0(TIME_WAIT);
            while(TIMER0){
                int x = scanKeyboard();
                if(x == 10){
                    msg_CanceledByUsr_money(changeOfCanceled());
                    delayMs1(LITTLE_WAIT);
                    return 0; //canceled by user | with money
                }
                if(x == 12) {
                    msg_dispenseProduct(change());
                    delayMs1(LITTLE_WAIT);
                    return 1; //confirmed by user
                }
            }
        }
    }
    msg_timeoutWithMoney(changeOfCanceled());
    delayMs1(LITTLE_WAIT);
    return 0; //digit1 ok digit2 ok | money timeout
}
void dispenseProduct() {
    convertIntToBinary();
    msg_WorkingOnDispenser();
    delayMs1(LITTLE_WAIT);
    msg_done();
    delayMs1(LITTLE_WAIT);
    MOTOR0 = 0;
    MOTOR1 = 0;
    MOTOR2 = 0;
    MOTOR3 = 0;
    return;
}
void convertIntToBinary(){
    MOTOR0 = (dispenser >> 0) & 1;
    MOTOR1 = (dispenser >> 1) & 1;
    MOTOR2 = (dispenser >> 2) & 1;
    MOTOR3 = (dispenser >> 3) & 1;
}
//Wait until delay ends
void delayMs0(unsigned int ms){          //use timer0 Mode01 (16 bits)
    while(ms){
        TH0 = 0xFE;			//1 ms @ f = 12 MHz (i.e. 64535) on Mode01
        TL0 = 0x18;
        TR0 = 1;
        while(!TF0);
        TF0 = 0;
        TR0 = 0;
        ms--;
    }
}
//Wait until delay ends
void delayMs1(unsigned int ms){          //use timer1 Mode01 (16 bits)
    while(ms){
        TH1 = 0xFE;			//1 ms @ f = 12 MHz (i.e. 64535) on Mode01
        TL1 = 0x18;
        TR1 = 1;
        while(!TF1);
        TF1 = 0;
        TR1 = 0;
        ms--;
    }
}
//Wait until MICRO delay ends
void Delay5us(){
    unsigned char i;
    for(i = 0; i < 5; i++){}
}
//like set an alarm
void timer0(unsigned int ms){            //use timer0 Mode01 (16 bits)
    TH0 = 0xFE;				//1 ms @ f = 12 MHz (i.e. 64535) on Mode01
    TL0 = 0x18;
    TR0 = 1;
    TF0 = 0;
    TIMER0 = 1;
    if(!ms){
        TR0 = 0;
        TIMER0 = 0;
    }
    ms--;
}
//like set an alarm
void timer1(unsigned int ms){            //use timer0 Mode01 (16 bits)
    TH1 = 0xFE;				//1 ms @ f = 12 MHz (i.e. 64535) on Mode01
    TL1 = 0x18;
    TR1 = 1;
    TF1 = 0;
    TIMER1 = 1;
    ms--;
    if(!ms){
        TR1 = 0;
        TIMER1 = 0;
    }
    ms--;
}
//interruptions

void ISR_External0(void) interrupt 0{
    IE0 = 0;
    moneyIsReady = 1;
return;
}
