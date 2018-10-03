# DOUGLAS AFFONSO CLEMENTINO
# GRR20175877

CC     = gcc -std=c11 -g

EXECS  = UNO 

# arquivos-objeto
objects = UNO.o  ConexaoRawSocket.o

all:  $(EXECS)

UNO: UNO.o ConexaoRawSocket.o 
	$(CC) UNO.o ConexaoRawSocket.o -o UNO 

UNO.o : UNO.c ConexaoRawSocket.h Estruturas.h
	$(CC) -c UNO.c 

ConexaoRawSocket.o : ConexaoRawSocket.c ConexaoRawSocket.h
	$(CC) -c ConexaoRawSocket.c 


clean:
	-rm $(objects)
	-rm *~

purge: clean
	-rm $(EXECS)