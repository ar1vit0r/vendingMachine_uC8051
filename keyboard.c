#include <at89x52.h>
#include "configuration.h"

int CHECK_LINES() {      // loop to check the keyboard and get the input, until the user press '*' or '#' key. Need a logic to take 2 digits input.
    while(1) {
        DELAY(100);

        LIN0 = 0;
        LIN1 = 1;
        LIN2 = 1;
        LIN3 = 1;
        
        if (!COL0) {
            DELAY(100)
						return 1;
        }
        if (!COL1) {
            DELAY(100);
            return 2;
        }
        if (!COL2) {
            DELAY(100);
            return 3;
        }

        LIN0 = 1;
        LIN1 = 0;
        LIN2 = 1;
        LIN3 = 1;
        
        if (!COL0) {
            DELAY(100);
            return 4;
        }
        if (!COL1) {
            DELAY(100);
            return 5;
        }
        if (!COL2) {
            DELAY(100);
            return 6;
        }

        LIN0 = 1;
        LIN1 = 1;
        LIN2 = 0;
        LIN3 = 1;
        if (!COL0) {
            DELAY(100);
            return 7;
        }
        if (!COL1) {
            DELAY(100);
            return 8;
        }
        if (!COL2) {
            DELAY(100);
            return 9;
        }

        LIN0 = 1;
        LIN1 = 1;
        LIN2 = 1;
        LIN3 = 0;
        
        if (!COL0) {
						//'*' key
            DELAY(100);
            return 10;
        }
        if (!COL1) {
            // '0' key 
            DELAY(100);
            return 11;
        }
        if (!COL2) {
						// '#' key
            DELAY(100);
            return 12;
        }
    }
}