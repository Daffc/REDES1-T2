# DOUGLAS AFFONSO CLEMENTINO
# GRR20175877

CC     = gcc -std=c11 -g

EXECS  = main 

# arquivos-objeto
objects = UNO.o  ConexaoRawSocket.o

all:  $(EXECS)

main: UNO.o ConexaoRawSocket.o main.o
	$(CC) UNO.o ConexaoRawSocket.o main.o -o main 


main.o : main.c ConexaoRawSocket.h UNO.h
	$(CC) -c main.c 

UNO.o : UNO.c ConexaoRawSocket.h UNO.h
	$(CC) -c UNO.c 

ConexaoRawSocket.o : ConexaoRawSocket.c ConexaoRawSocket.h
	$(CC) -c ConexaoRawSocket.c 


clean:
	-rm $(objects)
	-rm *~

purge: clean
	-rm $(EXECS)