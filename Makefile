# DOUGLAS AFFONSO CLEMENTINO
# GRR20175877

CC     = gcc -std=c11 -g

EXECS  = main 

# arquivos-objeto
objects = UNO.o   

all:  $(EXECS)

main: UNO.o main.o
	$(CC) UNO.o   main.o -o main 


main.o : main.c UNO.h
	$(CC) -c main.c 

UNO.o : UNO.c UNO.h
	$(CC) -c UNO.c 


clean:
	-rm $(objects)
	-rm *~

purge: clean
	-rm $(EXECS)