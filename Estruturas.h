
#ifndef _ESTRUTURAS_

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

    typedef struct Carta{
        enum Valores valor;
        enum Cores   cor;
    }Carta;

    typedef struct Mao{
        Carta *cartas;
        int quantidade_cartas;
    }Mao;

#endif 