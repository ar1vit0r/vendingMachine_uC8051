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
#include <stdio.h>
#include <at89x52.h>
#include "lcd.h"
#include "delay.h"
#include "keyboard.h"
#include "routines.h"
#include "vendingMachine.h"
#include "configuration.h"



// Initialize servo motor positions for each product
int servo_positions[30];

// Define product prices
float product_prices[30];

void main() {
	start();
	
	while(1){
/////KEYBOARD
		//CHECA KEYBOARD, CASO HAJA ENTRADA DE 1 DIGITO (DIFERENTE DE * OU #), DESABILITA A INTERRUPÇÃO
		//CHAMA A ROTINA DE INSERÇÃO DE CÓDIGO E DESABILITA A INTERRUPÇÃO DE ENTRADA DE DINHEIRO ATÉ COLOCAR 2ºCOD E PRESSIONAR ENTER (#)
		//PRESSIONADO #, HABILITA A INTERRUPÇÃO
		//AVISA O PREÇO DO PRODUTO
		//ESPERA POR 30S
			//WHILE
				//SE COLOCAR DINHEIRO < VALOR DO PRODUTO
					//ESPERA POR MAIS 30S
				//DO CONTRARIO DEVOLVE O DINHEIRO E AVISA DO ERRO
		//VOLTA A CHECAR O KEYBOARD
/////DINHEIRO
		//SE HOUVER INTERRUPÇÃO DE DINHEIRO
		//INFORMA A QUANTIA INSERIDA
		//ESPERA 30S
			//SE NAO HOUVER MAIS INTERAÇÃO DEVOLVE O DINHEIRO
			//SE CANCELAR (*) A OPERAÇÃO
				//DEVOLVE O DINHEIRO
				//VOLTA A CHECAR O KEYBOARD
			//SE ACRESCENTAR DINHEIRO, ATUALIZA A QUANTIA INSERIDA
				//ESPERA 30S
			//SE PRESSIONAR QUALQUER DIGITO
				//PEDE O 2º DIGITO
				//ESPERA 30S
				//SE CANCELAR (*) A OPERAÇÃO
					//DEVOLVE O DINHEIRO
					//VOLTA A CHECAR O KEYBOARD
				//SE NAO HOUVER MAIS INTERAÇÃO DEVOLVE O DINHEIRO
					//VOLTA A CHECAR O KEYBOARD
				//SE COLOCAR O 2º DIGITO
				//INFORMA O VALOR E A DIFERENÇA
					//ESPERA 30S
					//SE CANCELAR (*) A OPERAÇÃO
						//DEVOLVE O DINHEIRO
						//VOLTA A CHECAR O KEYBOARD
					
					//SE PRESCIONAR ENTER (#)
						//INFORMA A DIFERENÇA (VALOR INSERIDO - VALOR DO PRODUTO)
						//SE A DIFERENA FOR 0
							//FORNECE O PRODUTO
						//SE A DIFERENÇA FOR <0
							//FORNECE O PRODUTO
							//DEVOLVE O TROCO
						//SE A DIFERENÇA FOR >0
							//INFORMA A QUANTIA QUE FALTA
							//ESERA 30S
							//SE CANCELAR (*) A OPERAÇÃO
								//DEVOLVE O DINHEIRO
								//VOLTA A CHECAR O KEYBOARD
							//SE COLOCAR DINHEIRO
								//VOLTA A INFORMAR A DIFERENÇA
				
		
		
		
	}
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

    //return 0;
}