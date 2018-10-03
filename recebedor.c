#include "ConexaoDgramSocket.h"
#include "Estruturas.h"

#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
    int file = ConexaoDgramSocket("lo");
    char *msgRecebimento;

    msgRecebimento = malloc(20);

        printf("oi\n");  
        fflush(stdin);
        recv(file, msgRecebimento, 20, 0);
        printf("%s\n", msgRecebimento);
    
    
    return 0;
}
