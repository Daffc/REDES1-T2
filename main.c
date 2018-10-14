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
#define ANSI_COLOR_RED     "\x1b[31m\x1b[47m"
#define ANSI_COLOR_GREEN   "\x1b[32m\x1b[47m"
#define ANSI_COLOR_YELLOW  "\x1b[33m\x1b[47m"
#define ANSI_COLOR_BLUE    "\x1b[34m\x1b[47m"
#define ANSI_COLOR_RESET   "\x1b[0m"


/**
 * !!!!!!!!!!!!!!!PARA TESTE COM 3 JOGADORES, MUDAR NO CASO DE MAIS JOGADORES !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/
#define NUM_JOGADORES 3



void inicia_server(int *file_desc_server,struct hostent *h,struct sockaddr_in *servidor,char *host,char *port){
     if ((h = gethostbyname(host)) == NULL){
        printf("Não identifiquei meu endereço\n");
        exit(1);
    }   

    struct sockaddr_in server = *servidor;

    server.sin_port = htons(atoi(port));

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

    *servidor = server;
    *file_desc_server = s;
}

void inicia_cliente(int *file_desc_client,struct hostent *h_client,struct sockaddr_in *sock_client,char *host,char *port){

   

    if((h_client = gethostbyname(host)) == NULL){
        printf("não consegui me conectar ao ip do servidor\n");
        exit(1);
    }

    struct sockaddr_in sockaddr_in_client = *sock_client;


    memcpy ((char *) &sockaddr_in_client.sin_addr, (char *) h_client->h_addr_list[0], h_client->h_length);

    sockaddr_in_client.sin_family = h_client->h_addrtype;

    sockaddr_in_client.sin_port = htons(atoi(port));

    int c;

    if((c = socket(h_client->h_addrtype, SOCK_DGRAM,0)) < 0){
        printf("Não consegui abrir o socket");
        exit(1);
    }

    *sock_client = sockaddr_in_client;
    *file_desc_client = c;

}

void imprimeCarta(Carta carta){
    /**
     * Define a cor impressa de acordo com a cor da carta informada.
    */
    switch(carta.cor){
        case VERDE:
            printf(ANSI_COLOR_GREEN);
        break;
        case VERMELHO:
            printf(ANSI_COLOR_RED);
        break;
        case AZUL:
            printf(ANSI_COLOR_BLUE);
        break;
        case AMARELO:
            printf(ANSI_COLOR_YELLOW);
        break;                
    }

    /**
     * Define o valor a ser impresso e desabilita estilo após impressão.
    */
    if(carta.valor < 10)
        printf("%02d" ANSI_COLOR_RESET,  carta.valor);
    if(carta.valor == MAIS2)
        printf("+2" ANSI_COLOR_RESET);
    if(carta.valor == PULAR)
        printf("ØØ" ANSI_COLOR_RESET);
        
}

void imprimirCartas(Mao *hand){
        system("clear");
        /**
        * LEITURA DE CARTAS DA MÃO DE USUÁRIO. 
        */
        CartaMao *aux = hand->cartas->proxima;

        int posicao = 0;    

        while(aux){

            printf("Posicão %d\t Carta: ", posicao);
            
            /**
             * Verifica a cor da carta atual e imprime esta carta de acordo com a corinformada.
            */
            imprimeCarta(aux->carta);
            printf("\n");

            aux = aux->proxima; 
            posicao++;           
        }        
}


void recebe_jogada(struct Game *jogo, int player){
   
    printf("A carta do em jogo é:\n");        
    imprimeCarta(jogo->jogada);             
    printf("\n");
    printf("Aguarde a Próxima Jogada.\n");     
    
    if(jogo->player == player){
        jogo->player = (jogo->player + 1) % NUM_JOGADORES;
        jogo->tipo = PASSAVEZ;
    }
}
void recebe_uno(struct Game *jogo,int player){
    if(jogo->player == player){
        jogo->tipo = PASSAVEZ;
    }else{
        printf("O player %d declarou UNO , bora jogar uma para ele comprar",jogo->player);        
        // imprime carta jogo->jogada
    }
}

void recebe_fim(struct Game *jogo,int player){
    if(jogo->player == player){
        printf("O jogo acabou ganhei seus pato");
        exit(1);
    }else{
        printf("O player %d ganhou o jogo",jogo->player);  
        exit(1);
    }
}

void recebe_empate(struct Game *jogo,int player){   
    printf("O jogo acabou em empate, ninguem ganhou");    
    exit(1);   
}

void realiza_jogada(Mao *hand, Game *jogo){
    
    int escolha_carta;
    
    ESCOLHA:
                
    /**
     * Cartas do jogador são impressas e este deve escolher qual de suas cartas ele irá jogar, 
     * ou '-1' para comprar uma nova carta.
    */
    imprimirCartas(hand);
    printf("Carta em jogo é:\n");        
    imprimeCarta(jogo->jogada);
    printf("\n");                        
    printf("Informe a posição da carta de sua jogada dentre as opções acima ou -1 para comprar uma carta:\n");
    scanf("%d", &escolha_carta); 

    /**
    * Entra em laço caso usuário tenha digitado algum valor inválido (menor que -1 ou maior ou igual que a quantidade 
    * de cartas na mão).
    */
    while(escolha_carta < -1 || escolha_carta > hand->qnt_cartas){
        imprimirCartas(hand);
        printf("Carta em jogo é:\n");        
        imprimeCarta(jogo->jogada);
        printf("\n");        
        printf("Valor inválido, escolha apenas entre as opções acima ou -1 para comprar uma carta:\n");
        scanf("%d", &escolha_carta);                         
    }
    
    /**
    * Define ponteiros de controle para gerenciamento de catas da mão do jogador.
    */
    CartaMao *atual = hand->cartas->proxima, *anterior = hand->cartas;


    /**
     * Caso jogador tenha escolhido jogar uma de suas cartas.
    */
    if(escolha_carta > -1){      

        /**
        * Busca por carta escolhida por jogador.
        */
        for(int i = 0; i < escolha_carta; i++){
            anterior = atual;
            atual = atual->proxima;  
        }
        
        /**
        * Caso carta escolhida não possua cor ou valor compativel com jogada atual, jogador deve volvtar a fase de escolha.
        */
        if(atual->carta.cor != jogo->jogada.cor && atual->carta.valor != jogo->jogada.valor){
            goto ESCOLHA;
        }

        /**
         * Caso carta escolhida seja válida, carta é indicada como jogada atual e é removida da mão do jogador.
        */
        anterior->proxima = atual->proxima;
        jogo->jogada = atual->carta;
        free(atual);
        hand->qnt_cartas --;

        /**
         * Caso carta jogada seja MAIS2 ou PULAR, indica que o seu efeito está ativo.
        */
        if(jogo->jogada.valor > NOVE)
            jogo->efeito = 1;
            
        /**
         * Indica para os demais jogadores a nova jogada.
        */
        jogo->tipo = JOGADA;
    }
    else{

        /**
         * Compra nova carta e guarda estado da compra em "compra"
        */
        int compra = compraCartas(hand, 1, jogo);

        imprimirCartas(hand);

        printf("Carta em jogo é:\n");        
        imprimeCarta(jogo->jogada);
        printf("\n");    

        /**
         * Informa ao jogador que não existem mais cartas a serem compradas no baralho.
        */
        if(!compra){
            printf("Baralho não possui mais cartas para serem compradas.\n");
        }

        printf("Aguarde a Próxima Jogada.\n"); 


        /**
         * Passa a vez para o próximo jogador.
        */
        jogo->player = (jogo->player + 1) % NUM_JOGADORES;                  
        jogo->tipo = PASSAVEZ;
    }
}

void defineJogada(Game *jogo, Mao * hand, int posicao){
    CartaMao *atual = hand->cartas->proxima, *anterior = hand->cartas;

    for(int i = 0; i < posicao; i++){
        anterior = atual;
        atual = atual->proxima;  
    }
    anterior->proxima = atual->proxima;
    jogo->jogada = atual->carta;
    free(atual);
    hand->qnt_cartas --;
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

    int player = atoi(argv[5]), escolha_carta;
    int file_desc_server;
    struct hostent h;  
    struct sockaddr_in server; 
    int status_send;
    int status_receive;

    

    struct sockaddr_in sockaddr_in_client;
    struct hostent h_client;
    int file_desc_client;

    

    inicia_server(&file_desc_server,&h,&server,argv[1],argv[2]);
    inicia_cliente(&file_desc_client,&h_client,&sockaddr_in_client,argv[3],argv[4]);
        
    socklen_t len = sizeof server;

    Mao hand;

    // Cabeça da lista (Serve apenas para guardar inicio da lista ligada da mão).
    hand.cartas = malloc(sizeof(CartaMao)); 
    hand.cartas->proxima = NULL;
    hand.qnt_cartas = 0;

    Game jogo; // inicia o jogo para o player 0  

    if(player == 0){
        printf("dentro do if \n");

        GeraBaralho(jogo.baralho);
	    printf("depois de criar o baralho\n");

        jogo.tipo = DISTRIBUINDO;
        jogo.player = 0;
        jogo.qnt_cartas = 56;
        jogo.jogada;   
        jogo.efeito = 0;


        hand.qnt_cartas = 0;
        compraCartas(&hand, 7, &jogo);

        imprimirCartas(&hand);

        /**
         * Espera por entrada de usuário para que jogo prossiga.
        */
        printf("Precione ENTER para iniciar a distribuição de cartas.\n");
        getchar();      
        
        status_send = sendto(file_desc_client, &jogo, sizeof(Game), 0, (struct sockaddr * ) &sockaddr_in_client,sizeof(sockaddr_in_client));  
        if(status_send < 0){
            perror("Passando Baralho Jogador 0");
            exit(-1);
        }

    }

    while(1){      
        status_receive = recvfrom(file_desc_server, &jogo, sizeof(Game), 0, (struct sockaddr *) &server, &len);
        if(status_receive < 0){
            perror("Recebimento de mesagem");
            exit(-1);
        }
        switch(jogo.tipo){
            case DISTRIBUINDO:

                if(player){
                    compraCartas(&hand, 7, &jogo);
                    imprimirCartas(&hand);
                    printf("Aguarde a Primeira Jogada.\n");
                }
                else{
                    imprimirCartas(&hand);
                    printf("Informe a posição da carta de sua jogada dentre as opções acima:\n");
                    scanf("%d", &escolha_carta); 
                    
                    /**
                    * Entra em laço caso usuário tenha digitado algum valor inválido.
                    */
                    while(escolha_carta < 0 || escolha_carta > hand.qnt_cartas){
                        imprimirCartas(&hand);                        
                        printf("Valor inválido, escolha apenas entre as opções acima:\n");
                        scanf("%d", &escolha_carta);                         
                    }
                    
                    defineJogada(&jogo, &hand, escolha_carta);
                    
                    jogo.tipo = JOGADA;
                } 

            break;
            /**
             * - Jogador atual lê e imprime a jogada de outro jogador.
             * - Jogador atual recebe sua própria jogada e passa a vez para próximo jogador.
            */
            case JOGADA:
                imprimirCartas(&hand);
                recebe_jogada(&jogo, player);
            break;
            case PASSAVEZ:
                realiza_jogada(&hand, &jogo);                
                
            break;
            case UNO:
                recebe_uno(&jogo,player);
                /**
                 * Jogador atual recebe a mensagem de que jogador contido em jogo.player ira ficar com aṕenas uma carta.
                */
            break;
            case FIM:
                recebe_fim(&jogo,player);
                /**
                * Jogador atual recebe mensagem de que o jogo acabou, informando o vencedor.
                */            
            break;
            case EMPATE:
                status_send = sendto(file_desc_client, &jogo, sizeof(Game), 0, (struct sockaddr * ) &sockaddr_in_client,sizeof(sockaddr_in_client)); 
                recebe_empate(&jogo,player);
            break;
        }
        status_send = sendto(file_desc_client, &jogo, sizeof(Game), 0, (struct sockaddr * ) &sockaddr_in_client,sizeof(sockaddr_in_client)); 
    }  
    return 0;
}
