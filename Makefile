#Gregory Macri
#Makefile for Monte program

CFILES= hw3.c  
OFILES=$(CFILES:.c=.o)
CC=gcc -g -pthread -lm -Wall

monte:	$(OFILES)
	$(CC) -o monte $(OFILES) 

clean::
	/bin/rm -f monte $(OFILES)
