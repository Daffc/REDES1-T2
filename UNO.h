
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

    typedef struct Mao{
        Carta *cartas;
        int qnt_cartas;
    }Mao;

    /**
    * Gera baralho, embaralha e devolve ponteiro para este baralho (56 cartas).
    */
    Carta * GeraBaralho();

#endif 