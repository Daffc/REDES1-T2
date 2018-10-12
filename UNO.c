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
        int i;

        aux = hand->cartas;

        /**
         * Percorre toda a mão ate achar carta cujo ponteiro proximo seja null
        */
        while(aux->proxima)
            aux = aux->proxima;

        /**
         * Compra cartas enquanto i for igual a quantidade a ser comprada e 
         * enquanto ainda houverem cartas a serem compraas (jogo->qnt_cartas != 0).
        */
        for(i = 1; (i < (qnt_compra + 1) && jogo->qnt_cartas); i++){
            /**
             *  Aloca espaço para proxima carta, já apontando para essa nova carta e muda o ponteoro 
             * "aux" para a proxima carta a ser comprada.
            */
            aux->proxima = malloc(sizeof(CartaMao));
            aux->proxima->carta = *(jogo->baralho + (jogo->qnt_cartas - i));
            aux = aux->proxima;
            hand->qnt_cartas ++;
            /**
             * Subtrai a "qnt_compra" do contador de cartas do baralho para que nenhum dos 
             * usuário compre cartas alem do final do baralho..
            */
            jogo->qnt_cartas = jogo->qnt_cartas-1;
        } 
        
        /**
         * Verifica pela variável "i" se o jogador conseguiu comprar a "qnt_compra" pretendida, 
         * caso contrário, informa ao usuário que o baralho acabou.
        */
        if((qnt_compra + 1) != i){
            printf("Baralho não possui mais cartas para serem compradas.\n");
        }
}
