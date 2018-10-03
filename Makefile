# DOUGLAS AFFONSO CLEMENTINO
# GRR20175877

CC     = gcc -std=c11 -g

EXECS  = UNO jogador

# arquivos-objeto
objects = UNO.o jogador.o ConexaoDgramSocket.o

all:  $(EXECS)

UNO: UNO.o ConexaoDgramSocket.o 
	$(CC) UNO.o ConexaoDgramSocket.o -o UNO 

jogador: jogador.o ConexaoDgramSocket.o 
	$(CC) jogador.o ConexaoDgramSocket.o -o jogador 


UNO.o : UNO.c ConexaoDgramSocket.h Estruturas.h
	$(CC) -c UNO.c 


jogador.o : jogador.c ConexaoDgramSocket.h Estruturas.h
	$(CC) -c jogador.c 

ConexaoDgramSocket.o : ConexaoDgramSocket.c ConexaoDgramSocket.h
	$(CC) -c ConexaoDgramSocket.c 


clean:
	-rm $(objects)
	-rm *~

purge: clean
	-rm $(EXECS)