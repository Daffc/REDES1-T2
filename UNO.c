#include "ConexaoRawSocket.h"
#include "Estruturas.h"

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
        aux = *(baralho + primeirogit);
        *(baralho + primeiro) = *(baralho + segundo);
        *(baralho + segundo) = aux;
    }

}

// Carta * GeraBaralho(){
void main(int argc, char const *argv[]){
    
    Carta card;
    Carta *baralho;

    // Aloca todas as cartas do jogo.
    baralho = malloc(sizeof(Carta) * 56);

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

    for(int i = 0; i < 56; i++){
        printf("cor: %d valor: %d\n", (baralho + i) -> cor, (baralho + i) -> valor);
    }

    Embaralha(baralho);
    for(int i = 0; i < 56; i++){
        printf("cor: %d valor: %d\n", (baralho + i) -> cor, (baralho + i) -> valor);
    }
    
    // return baralho;
}
