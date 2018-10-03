#include "ConexaoDgramSocket.h"
#include "Estruturas.h"

#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
    int file = ConexaoDgramSocket("lo");
    char *msgEnvio;
    char *msgRecebimento;

    msgEnvio = malloc(20);
    msgRecebimento = malloc(20);

    msgEnvio = "SUP";    
    send(file, msgEnvio, 20, 0);
    // recv(file, msgRecebimento, 20, 0);

    // printf("%s", msgRecebimento);
    
    return 0;
}
