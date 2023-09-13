#include "C:\Program Files\Keil_v5\C51\INC\Atmel\at89x52.h"

#define MOTOR0 P0_0
#define MOTOR1 P0_1
#define MOTOR2 P0_2
#define MOTOR3 P0_3

#define LINA    P1_0
#define LINB    P1_1
#define LINC    P1_2
#define LIND    P1_3
#define COL1    P1_4
#define COL2    P1_5
#define COL3    P1_6

#define LCD     P2

#define PLUS1   P3_0
#define PLUS10  P3_1
#define insert  P3_2
#define MINUS1  P3_3
#define MINUS10 P3_4

#define EN      P3_6
#define RS      P3_7

#define MAX_PRODUCT 9

#define PRODUCT0 6
#define PRODUCT1 10
#define PRODUCT2 7
#define PRODUCT3 13
#define PRODUCT4 2
#define PRODUCT5 15
#define PRODUCT6 20
#define PRODUCT7 18
#define PRODUCT8 1

//LCD
void ConfigLCD();
void WriteMSG(char msg[]);
void Line1();
void Line2();
void WrCMD();
void WrCHAR();

//timers
#define TIME_WAIT 10000
#define LITTLE_WAIT 3000
void timer0(unsigned int ms);
void delayMs0(unsigned int ms);
void timer1(unsigned int ms);
void delayMs1(unsigned int ms);
void Delay5us();

//machine
void start();
//unsigned int startMoney(); //start machine with money
unsigned int startCode(); //start machine with code


//start with code
unsigned int giveMeTheMoney(); //request money for product
void dispenseProduct();
void convertIntToBinary();

unsigned int scanKeyboard();

//flags 8051
unsigned int TIMER0;    //Flag TF0
unsigned int TIMER1;    //Flag TF1
unsigned int EXTER0;    //Flag IE0
unsigned int EXTER1;    //Flag IE1

//global variables
unsigned int digit1;
unsigned int digit2;
unsigned int digit12;
unsigned int amount;
unsigned int price;
unsigned int dispenser;
unsigned int moneyIsReady;
unsigned int priceProduct[MAX_PRODUCT] = {PRODUCT0, PRODUCT1, PRODUCT2,
                                          PRODUCT3, PRODUCT4, PRODUCT5,
                                          PRODUCT6, PRODUCT7, PRODUCT8};
unsigned int codeProduct[MAX_PRODUCT] = {00, 01, 02,
                                         10, 11, 12,
                                         20, 21, 22};


int main() {
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
        if (IE) {
            //startMoney();
            goto start;
        }
    }
    return 0;
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
    WriteMSG("        *       ");
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
    WriteMSG("     Denied!    ");
}
void msg_Code2Insert(){
    msg_CLEANER();
    ConfigLCD();
    Line1();
    WriteMSG("  Product code: ");
    Line2();
    WriteMSG((char*) digit12);
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
    WriteMSG("msg_insertMoney ");
    Line2();
    WriteMSG("msg_insertMoney ");
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
    Line2();
    WriteMSG("msg_CanceledByUs");
}
void msg_dispenseProduct(unsigned int value){
    msg_CLEANER();
    ConfigLCD();
    Line1();
    WriteMSG("   Successful   ");
    Line2();
    WriteMSG("msg_dispenseProd");
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
    EA = 0x8F;
    IE = 0;
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
unsigned int codeValidation(){
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
        }if (digit2 != 77){
            digit12 = digit1*10 + digit2;
            msg_Code2Insert();
            delayMs1(LITTLE_WAIT); //time for read msg
            if(codeValidation()) {
                return 1; //digit 1 ok, digit2 ok | without money
            }else{
                msg_IllegalCode();
                timer1(TIME_WAIT);
                delayMs1(LITTLE_WAIT); //time for read msg
                return 0; //canceled | illegal code
            }
        }else{ //digit2 == 77
            //digit 1 ok, digit2 waiting | without money
        }
    }
    msg_TimeOut_Code2_WTMoney();
    delayMs1(LITTLE_WAIT); //time for read msg
    return 0; //digit1 ok, digit2 timeout | without money
}

unsigned int giveMeTheMoney(){
    IE = 0; //interrupt of money enable
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
        TH0 = 0xFE;			//1 ms @ f = 12 MHz (i.e. 64535) on Mode01
        TL0 = 0x18;
        TR0 = 1;
        while(!TF0);
        TF0 = 0;
        TR0 = 0;
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
    EX0 = 0;
    moneyIsReady = 1;
return;
}
