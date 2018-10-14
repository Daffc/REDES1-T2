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
#define ANSI_COLOR_BACKGOURND   "\x1b[47m"
#define ANSI_COLOR_RED          "\x1b[31m"
#define ANSI_COLOR_GREEN        "\x1b[32m"
#define ANSI_COLOR_YELLOW       "\x1b[33m"
#define ANSI_COLOR_BLUE         "\x1b[34m"
#define ANSI_COLOR_RESET        "\x1b[0m"


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

void limparEntrada(){
    /**
     * Limpa Buffer de entrada padrão para que pausas possam ocorrer durante o programa.
    */
    int ch;

    do
        ch = fgetc ( stdin ); 
    while ( ch != EOF && ch != '\n' ); 

    clearerr ( stdin );
}
void imprimeCarta(Carta carta){
    /**
     * Define a cor impressa de acordo com a cor da carta informada.
    */
    switch(carta.cor){
        case VERDE:
            printf(ANSI_COLOR_GREEN ANSI_COLOR_BACKGOURND);
        break;
        case VERMELHO:
            printf(ANSI_COLOR_RED ANSI_COLOR_BACKGOURND);
        break;
        case AZUL:
            printf(ANSI_COLOR_BLUE ANSI_COLOR_BACKGOURND);
        break;
        case AMARELO:
            printf(ANSI_COLOR_YELLOW ANSI_COLOR_BACKGOURND);
        break;                
    }

    /**
     * Define o valor a ser impresso e desabilita estilo após impressão.
    */
    if(carta.valor <= NOVE)
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

void impressaoPadrao(Mao *hand, Game *jogo){
    imprimirCartas(hand);
    printf("\n\n\n\n");            
    printf("Carta em jogo é:\n");        
    imprimeCarta(jogo->jogada);
    printf("\n\n\n");     
}

void passaVez(Game *jogo){
    /**
     * Passa a vez para o próximo jogador.
    */
    jogo->player = (jogo->player + 1) % NUM_JOGADORES;                  
    jogo->tipo = PASSAVEZ;   
}

void recebe_uno(struct Game *jogo, Mao * hand, int player){
    impressaoPadrao(hand, jogo);

    if(jogo->player == player){
        /**
         * Pausa programa para que todos os jogadores saibam do anuncio do UNO.
        */
        printf(ANSI_COLOR_GREEN  "UNO foi anunciao !!!\n" ANSI_COLOR_RESET);
        printf("Precione [ENTER] para proseguir o jogo:\n");
        limparEntrada();

        fflush (stdout);
        getchar();  

        impressaoPadrao(hand, jogo);
        passaVez(jogo);
    }else{
        printf(ANSI_COLOR_RED "O player %d declarou UNO !!!" ANSI_COLOR_RESET, jogo->player);        
        printf("Bora jogar uma para ele comprar\n");        
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
    impressaoPadrao(hand, jogo);                       
    printf(ANSI_COLOR_GREEN "SUA VEZ !!!\n" ANSI_COLOR_RESET);
    printf("Informe a posição da carta de sua jogada dentre as opções acima ou -1 para comprar uma carta:\n");
    scanf("%d", &escolha_carta); 

    /**
    * Entra em laço caso usuário tenha digitado algum valor inválido (menor que -1 ou maior ou igual que a quantidade 
    * de cartas na mão).
    */
    while(escolha_carta < -1 || escolha_carta >= hand->qnt_cartas){
        impressaoPadrao(hand, jogo);        
        printf(ANSI_COLOR_GREEN "SUA VEZ !!!\n" ANSI_COLOR_RESET);        
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

        impressaoPadrao(hand, jogo);

        /**
         * Informa ao jogador que não existem mais cartas a serem compradas no baralho.
        */
        if(!compra){
            printf("Baralho não possui mais cartas para serem compradas.\n");
        }
        printf("Aguarde a Próxima Jogada.\n"); 
        passaVez(jogo);
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
         * Define a primeira jogada do jogo.
        */
        jogo.jogada = jogo.baralho[jogo.qnt_cartas - 1];
        jogo.qnt_cartas --;

        /**
         * Espera por entrada de usuário para que jogo prossiga.
        */
        printf("Precione [Enter] para iniciar a distribuição de cartas.\n");
        fflush (stdout);
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

            /**
             * - Mensagem passa pelos jogadores distribuindo cartas.
             * - Ao chegar no jogador '0' esse deve realizar a primeira jogada.
            */
            case DISTRIBUINDO:

                if(player){
                    compraCartas(&hand, 7, &jogo);
                    impressaoPadrao(&hand, &jogo);      
                    printf("Aguarde a Próxima jogada.\n");
                }
                else{
                    realiza_jogada(&hand, &jogo);      
                } 

            break;

            /**
             * - Jogador atual lê e imprime a jogada de outro jogador.
             * - Jogador atual recebe sua própria jogada e passa a vez para próximo jogador.
            */
            case JOGADA:
                impressaoPadrao(&hand, &jogo);
                
                // Caso jogador que realizou a jogada seja o que recebeu a menságem, bastão é passado.
                if(jogo.player == player){
                    if(hand.qnt_cartas != 1)
                        passaVez(&jogo);
                    else    
                        jogo.tipo = UNO;
                }
            break;

            /**
            * - Jogador atual possui o bastão.
            * - Jogador debe verificar carta de "jogada" e reagir a ela caso seja uma carta de efeito (MAIS2 ou PULAR), ou realizar uma jogada.
            */
            case PASSAVEZ:
                /**
                 * Caso carta de efetito:
                */
                if(jogo.efeito){   

                    /**
                     * Se +2, compra cartas e passa a vez 
                    */                 
                    if(jogo.jogada.valor == MAIS2){

                        int compra = compraCartas(&hand, 2, &jogo);

                        impressaoPadrao(&hand, &jogo);
                        printf("Jogaram um '+2' para você =(.\n"); 

                        /**
                         * Informa ao jogador que não existem mais cartas a serem compradas no baralho.
                        */
                        if(!compra){
                            printf("Baralho não possui mais cartas para serem compradas.\n");
                        }

                        passaVez(&jogo);

                        // Desabilita efeito da carta.
                        jogo.efeito = 0;
                    }

                    if(jogo.jogada.valor == PULAR){
                        printf("Você foi pulado =/.\n"); 
                        passaVez(&jogo);
                        
                        // Desabilita efeito da carta.
                        jogo.efeito = 0;
                    }          
                }
                /**
                 * Caso efeito esteja desabilitado, carta de jogada deve ser tratada como uma carta comum.
                */
                else
                    realiza_jogada(&hand, &jogo);                      
            break;

            /**
             * - Jogador atual recebe menságem que que jogador indicaro em jogo.player possui apenas uma carta.
             * - Se jogador = Jogador.player, passar a vez para próximo jogador.
            */
            case UNO:
                recebe_uno(&jogo, &hand, player);
            break;

            /**
             * - Anuncia fim de jogo e quem foi o campeão a todos os jogadores.
            */
            case FIM:
                recebe_fim(&jogo,player);
                /**
                * Jogador atual recebe mensagem de que o jogo acabou, informando o vencedor.
                */            
            break;

            /**
             * - Anuncia fim de jogo e que houve empate ( não é mais possivel efetuar jogadas ).
            */
            case EMPATE:
                status_send = sendto(file_desc_client, &jogo, sizeof(Game), 0, (struct sockaddr * ) &sockaddr_in_client,sizeof(sockaddr_in_client)); 
                recebe_empate(&jogo,player);
            break;
        }
        status_send = sendto(file_desc_client, &jogo, sizeof(Game), 0, (struct sockaddr * ) &sockaddr_in_client,sizeof(sockaddr_in_client)); 
    }  
    return 0;
}
