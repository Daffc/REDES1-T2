// adicionar IFDEF

int ConexaoDgramSocket(char *device);

typedef struct Mensagem{
    
    unsigned char marcador_inicio;
    unsigned char tamanho;
    unsigned char sequencia;
    unsigned char tipo;
    char dados;
    unsigned char crc;
    
}Mensagem;

