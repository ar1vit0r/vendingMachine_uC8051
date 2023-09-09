#include <at89x52.h>
#include keyboard.h
#include delay.h

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