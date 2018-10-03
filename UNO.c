#include "ConexaoDgramSocket.h"
#include "Estruturas.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
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

    return 0;
}
