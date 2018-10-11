#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include "UNO.h"


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

    Game jogo; // inicia o jogo para o player 0

    int status_send;
    int status_receive;

    if(player == 0){
        printf("dentro do if \n");

        memcpy( &(jogo.baralho), GeraBaralho(), 56 * sizeof(Carta));
	    printf("depois de criar o baralho\n");

        jogo.tipo = DISTRIBUINDO;
        jogo.player = 0;
        jogo.qnt_cartas = 56;
        jogo.jogada;   
        jogo.efeito = 0;

        // adiciona na mão do player 0 as cartas
        for(int i = (jogo.qnt_cartas - 1),j = 0; i > (jogo.qnt_cartas - 8); i--,j++){
            Hand.cartas[j].cor = jogo.baralho[i].cor;
            Hand.cartas[j].valor = jogo.baralho[i].valor;
        } 

        printf("apos adicionar baralho a mao \n");
        Hand.qnt_cartas = 7;

        // seta em 7 o numero de cartas do player one
        for (int i = 0; i < Hand.qnt_cartas; i++){
            printf("cor: %d valor: %d\n", Hand.cartas[i].cor, Hand.cartas[i].valor);
        }

        // remove as 7 cartas que foram adicionadas no 
        jogo.qnt_cartas = jogo.qnt_cartas - 7;

        printf("Precione ENTER para iniciar a distribuição de cartas.\n");
        int dummy;
        scanf("%d", &dummy);      
        
        status_send = sendto(c, &jogo, sizeof(Game), 0, (struct sockaddr * ) &sockaddr_in_client,sizeof(sockaddr_in_client));  
        if(status_send < 0){
            perror("Passando Baralho Jogador 0");
            exit(-1);
        }

    }

    while(1){        
        status_receive = recvfrom(s, &jogo, sizeof(Game), 0, (struct sockaddr *) &server, &len);
        if(status_receive < 0){
            perror("Recebimento de meságem");
            exit(-1);
        }
        switch(jogo.tipo){
            case DISTRIBUINDO:

                if(player){
                    printf("aki ocorrera o famoso acesso");
                    for(int i = jogo.qnt_cartas ,j = 0; i > jogo.qnt_cartas - 7;i--,j++){
                        Hand.cartas[j].cor = jogo.baralho[i].cor;
                        Hand.cartas[j].valor = jogo.baralho[i].valor;
                    }
                    // printa as cartas for testing;

                    Hand.qnt_cartas = 7;
                    for (int i = 0; i < Hand.qnt_cartas; i++){
                        printf("cor: %d valor: %d\n", Hand.cartas[i].cor, Hand.cartas[i].valor);
                    }
                    jogo.qnt_cartas = jogo.qnt_cartas - 7;
                }
                else{
                    jogo.tipo = JOGADA;
                    //player 0 joga uma carta.
                }
                
                status_send = sendto(c, &jogo, sizeof(Game), 0, (struct sockaddr * ) &sockaddr_in_client,sizeof(sockaddr_in_client)); 

            break;

            case JOGADA:
                /**
                 * - Jogador atual le e imprime a jogada de outro jogador
                 * - Jogador atual recebe sua própria jogada e passa a vez paraq próximo jogador.
                */
            break;
            case PASSAVEZ:
                /**
                 * Jogador atual recebe mensagem de jogador anterior e faz sua jogada a menos que carta jogada seja PULAR e jogo.efeito seja 1;
                */
            break;
            case UNO:
                /**
                 * Jogador atual recebe a mensagem de que jogador contido em jogo.player ira ficar com aṕenas uma carta.
                */
            break;
            case FIM:
                /**
                * Jogador atual recebe mensagem de que o jogo acabou, informando o vencedor ou que houve empate.
                */
            break;
        }
    }  
    return 0;
}
