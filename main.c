#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include "Estruturas.h"


int main(int argc, char **argv){

    printf("%d\n",argc);

    for( int i = 0 ; i < argc ; i++){
        printf("Argumento = %d , elemento argv[%d] = %s\n", i ,i, argv[i] );
    }

    if(argc != 6){
        printf("Forma de uso ./a.out server: <localhost> <porta> client: <hostname> <porta> player <numero>\n");
        exit(1);
    }

    int player = atoi(argv[5]);

    printf("Inicia processo de montagem do socket_server\n");

    struct hostent *h;

    // não precisa ser necessariamente o localhost, pode ser usado da entrada.
    if ((h = gethostbyname(argv[1])) == NULL){
        printf("Não identifiquei meu endereço\n");
        exit(1);
    }

    struct sockaddr_in server;

    server.sin_port = htons(atoi(argv[2]));

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

    host = argv[3];

    host_port = argv[4];

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

    
    socklen_t len = sizeof server;


    Mao Hand;
    // pior caso numero de cartas sera 23
    Hand.cartas = malloc(sizeof(char) * 23);

    Game *jogo,*dummy; // inicia o jogo para o player 0
    dummy = malloc( sizeof(Game));
    jogo = malloc( sizeof(Game));

    dummy->Baralho = malloc(sizeof(Carta) * 56);

    jogo->Baralho = malloc(sizeof(Carta) * 56);

    int status_send;
    int status_receive;

    if(player == 0){
    printf("dentro do if \n");
    // Cria o baralho inicial
    Carta *baralho;

    baralho = malloc(sizeof(Carta) * 56);

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
	printf("depois de criar o baralho\n");

        jogo->tipo = 0;
        jogo->player = 0;
        jogo->qnt_cartas = 56;
        // jogo.jogada = primeira carta do player one   
        jogo->efeito = 0;
        // baralho embaralhado passa para a estrutura do jogo
        for(int i = 0 ; i < 56; i++){
            jogo->Baralho[i].cor = baralho[i].cor;
            jogo->Baralho[i].valor = baralho[i].valor;
        }   
        // adiciona na mão do player 0 as cartas
        for(int i = 55,j = 0; i > 55 - 7;i--,j++){
            Hand.cartas[j].cor = baralho[i].cor;
            Hand.cartas[j].valor = baralho[i].valor;
        } 
	printf("apos adicionar baralho a mao \n");
	Hand.quantidade_cartas = 7;

        for (int i = 0; i < Hand.quantidade_cartas; i++){
            printf("cor: %d valor: %d\n", Hand.cartas[i].cor, Hand.cartas[i].valor);
        }
        // seta em 7 o numero de cartas do player one
        // remove as 7 cartas que foram adicionadas no 
        jogo->qnt_cartas = jogo->qnt_cartas - 7;      

        status_send = sendto(c,&jogo,sizeof(jogo),0, (struct sockaddr * ) &sockaddr_in_client,sizeof(sockaddr_in_client));  
	printf("status send %d\n",status_send);
    }   
   printf("fora do if\n"); 

    while(1){        
        status_receive = recvfrom(s,&jogo,sizeof(jogo),0,(struct sockaddr * ) &server, &len);
        if(status_receive > -1){
            memcpy(dummy,jogo,sizeof(Game));
            printf("Dummy->tipo = %d",dummy->tipo);
            if(jogo->tipo == 0 && player){
		    printf("aki ocorrera o famoso acesso");
                for(int i = jogo->qnt_cartas - 1,j = 0; i > jogo->qnt_cartas - 8;i--,j++){
                    Hand.cartas[j].cor = jogo->Baralho[i].cor;
                    Hand.cartas[j].valor = jogo->Baralho[i].valor;
                }
                // printa as cartas for testing;
                for (int i = 0; i < Hand.quantidade_cartas; i++){
                    printf("cor: %d valor: %d\n", Hand.cartas[i].cor, Hand.cartas[i].valor);
                }
                Hand.quantidade_cartas = 7;
                jogo->qnt_cartas = jogo->qnt_cartas - 7;
                status_send = sendto(c,&jogo,sizeof(jogo),0, (struct sockaddr * ) &sockaddr_in_client,sizeof(sockaddr_in_client));
            }else{
                printf("Jogo esta prestes a iniciar \n");
                // exit(1);
            }
        }
    }


    // if(player == 0){
    //     // while(1){
    //     int envia = sendto(c,buffer,strlen(buffer),0, (struct sockaddr * ) &sockaddr_in_client,sizeof(sockaddr_in_client));
    //     perror("send");
    //     if(envia == -1){
    //         exit(1);
    //     }
    //     printf("mensagem : %s ... enviada\n",buffer);
    //     // printf("Estado do envio : %d\n",envia);
    //     // }
    //     exit(1);
    // }else{
    //     while(1){
    //         int test = recvfrom(s,recebe_buffer,strlen(buffer),MSG_DONTWAIT,(struct sockaddr * ) &server, &len);
    //         if(test > -1){
    //             printf("%s\n",recebe_buffer);
    //             printf("%ld\n",strlen(recebe_buffer));
    //         }
    //     }
    // }

    

    return 0;
}
