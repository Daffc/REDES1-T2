
#ifndef _UNO_
    #define _UNO_


    enum Cores{
        AMARELO,
        AZUL, 
        VERMELHO,
        VERDE
    }Cores;

    enum Valores{
        ZERO,
        UM,
        DOIS,
        TRES,
        QUATRO,
        CINCO,
        SEIS, 
        SETE,
        OITO,
        NOVE,
        MAIS2,
        PULAR,
    } Valores;

    enum Estados{
        DISTRIBUINDO,
        JOGADA,
        PASSAVEZ,
        UNO,
        FIM,
    }Estados;

    typedef struct Carta{
        enum Valores valor;
        enum Cores   cor;
    }Carta;

    typedef struct Game{
        enum Estados tipo;
        int player;
        int qnt_cartas;
        struct Carta jogada;
        int efeito;
        int deadlock;
        struct Carta baralho[56];
    }Game;

    typedef struct CartaMao{
        Carta       carta;
        struct CartaMao    *proxima;
    }CartaMao;

    typedef struct Mao{
        CartaMao *cartas;
        int qnt_cartas;
    }Mao;

    /**
    * Recebe ponteiro para buffer baralho previamente alocado e o devolve preenchido e embaralhado.
    */
    void GeraBaralho(Carta *);

    /**
    * Recebe ponteiros para mão, e baralho e a quantidade de cartas contidas no baralho, 
    * e finaliza tendo as novas cartas compradas adicionadas a mão indicada.
    */
    void compraCartas(Mao *hand,int qnt_compra, Game * jogo);


#endif 