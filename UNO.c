#include "UNO.h"

#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void Embaralha(Carta * baralho){
    Carta aux;
    int primeiro, segundo;

    srand(time(NULL));

    for(int i = 0; i < 224; i++){
        primeiro = rand() % 55;
        segundo = rand() % 55;
        aux = *(baralho + primeiro);
        *(baralho + primeiro) = *(baralho + segundo);
        *(baralho + segundo) = aux;
    }

}

void GeraBaralho(Carta * baralho){

    // Gerando todas as cartas Do Baralho.
    for(int i = 0; i < 12; i++){
        if(i < 10)
        {
            for(int j = 0; j < 4; j++){
                (baralho + (i*4) + j) -> valor = i;
                (baralho + (i*4) + j) -> cor = j;
            } 
        }        
        else{
            for(int j = 0; j < 4; j++){
                (baralho + (i*4) + j) -> valor = i;
                (baralho + (i*4) + j) -> cor = j;
                (baralho + ((i+2)*4) + j) -> valor = i;
                (baralho + ((i+2)*4) + j) -> cor = j;                
            } 
        }
    }

    Embaralha(baralho);
}

void compraCartas(Mao *hand,int qnt_compra, Game * jogo){
        CartaMao * aux;

        aux = hand->cartas;

        /**
         * Percorre toda a mão ate achar carta cujo ponteiro proximo seja null
        */
        while(aux->proxima)
            aux = aux->proxima;

        printf("oiasd\n");
        /**
         *  Aloca espaço para proxima carta, já apontando para essa nova carta e muda o ponteoro "aux" para a proxima carta a ser comprada.
        */
        for(int i = 1; i < (qnt_compra + 1); i++){
            aux->proxima = malloc(sizeof(CartaMao));
            aux->proxima->carta = *(jogo->baralho + (jogo->qnt_cartas - i));
            aux = aux->proxima;
            hand->qnt_cartas ++;
        } 

        /**
         * Subtrai a "qnt_compra" do contador de cartas do baralho.
        */
        jogo->qnt_cartas -= qnt_compra;
}
