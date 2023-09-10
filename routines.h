#ifndef _ROUTINES_H
#define _ROUTINES_H

void start();
void store(int line, int coll, int qnt);
void returnInsertedMoney_time(float inserted_amount);
void dispenseProduct(int selected_product);
void dispenseChange(float change);
void dispenseProductAndChange(int selected_product, float change);
void waitTheRestOfcash(float product_price, float inserted_amount);
int checkIfNeedChange(float inserted_amount, float product_price);

#endif