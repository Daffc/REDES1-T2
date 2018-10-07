#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>


int main(int argc, char **argv){

    printf("%d\n",argc);

    for( int i = 0 ; i < argc ; i++){
        printf("Argumento = %d , elemento argv[%d] = %s\n", i ,i, argv[i] );
    }

    if(argc != 4){
        printf("Forma de uso ./a.out server: <porta> client: <hostname> <porta> \n");
        exit(1);
    }

    printf("Inicia processo de montagem do socket_server\n");

    struct hostent *h;

    // não precisa ser necessariamente o localhost, pode ser usado da entrada.
    if ((h = gethostbyname("localhost")) == NULL){
        printf("Não identifiquei meu endereço\n");
        exit(1);
    }

    struct sockaddr_in server;

    server.sin_port = htons(atoi(argv[1]));

    memcpy ((char *) &server.sin_addr, (char *) h->h_addr_list[0], h->h_length);
    // por compatibilidade h_addr esta no primeiro elemento do h_addr_list
    // h_addr which is just the first element of the vector h_addr_list

    server.sin_family = h->h_addrtype;

    int s;

    if((s = socket( h->h_addrtype,SOCK_DGRAM,0)) < 0){
        printf("Não abriu o socket\n");
        exit(1);
    }

    if(bind(s, (struct sockaddr *) &server, sizeof(server)) < 0){
        printf("Não realizou bind\n");
        exit(1);
    }




    printf("Inicia processo de montagem do socket_client\n");

    char *host;
    char *host_port;

    host = argv[2];

    host_port = argv[3];

    struct hostent *h_client;

    if((h_client = gethostbyname(host)) == NULL){
        printf("não consegui me conectar ao ip do servidor\n");
        exit(1);
    }

    struct sockaddr_in sockaddr_in_client;

    memcpy ((char *) &sockaddr_in_client.sin_addr, (char *) h_client->h_addr_list[0], h->h_length);

    sockaddr_in_client.sin_family = h_client->h_addrtype;

    sockaddr_in_client.sin_port = htons(atoi(host_port));

    int c;

    if((c = socket(h_client->h_addrtype, SOCK_DGRAM,0)) < 0){
        printf("Não consegui abrir o socket");
        exit(1);
    }

    return 0;
}