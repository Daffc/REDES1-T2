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

/**
 * Definição de cores utilizadas para printar cartas.
*/
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"


void imrpimrCartas(Mao *Hand){
        /**
        * ------------------------------------------------------------------
        * LEITURA DE CARTAS DA MÃO DE USUÁRIO, EXEMPLO PARA USO EM OUTRAS LOCALIDADES. 
        * EXCLUIR APOS USO.
        */
        CartaMao *aux = Hand->cartas->proxima;

        int posicao = 0;    

        while(aux){

            /**
             * Verifica a cor da carta atual e imprime esta carta de acordo com a corinformada.
            */
            switch(aux->carta.cor){
                case VERDE:
                    printf(ANSI_COLOR_GREEN "Posicão %d\t valor: %d\n" ANSI_COLOR_RESET, posicao, aux->carta.valor);
                break;
                case VERMELHO:
                    printf(ANSI_COLOR_RED "Posicão %d\t valor: %d\n" ANSI_COLOR_RESET, posicao,aux->carta.valor);
                break;
                case AZUL:
                    printf(ANSI_COLOR_BLUE "Posicão %d\t valor: %d\n" ANSI_COLOR_RESET, posicao, aux->carta.valor);
                break;
                case AMARELO:
                    printf(ANSI_COLOR_YELLOW "Posicão %d\t valor: %d\n" ANSI_COLOR_RESET, posicao, aux->carta.valor);
                break;                
            }

            aux = aux->proxima; 

            posicao++;           
        }
}

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

    // Cabeça da lista (Serve apenas para guardar inicio da lista ligada da mão).
    Hand.cartas = malloc(sizeof(CartaMao)); 
    Hand.cartas->proxima = NULL;

    Game jogo; // inicia o jogo para o player 0

    int status_send;
    int status_receive;

    if(player == 0){
        printf("dentro do if \n");

        GeraBaralho(jogo.baralho);
	    printf("depois de criar o baralho\n");

        jogo.tipo = DISTRIBUINDO;
        jogo.player = 0;
        jogo.qnt_cartas = 56;
        jogo.jogada;   
        jogo.efeito = 0;


        Hand.qnt_cartas = 0;
        compraCartas(&Hand, 7, &jogo);

        imrpimrCartas(&Hand);

        /**
         * Espera por entrada de usuário para que jogo prossiga.
        */
        printf("Precione ENTER para iniciar a distribuição de cartas.\n");
        getchar();      
        
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
                    compraCartas(&Hand, 7, &jogo);
                    imrpimrCartas(&Hand);
                }
                else{
                    //player 0 joga uma carta.
                    jogo.tipo = JOGADA;
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
