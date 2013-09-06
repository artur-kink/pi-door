.PHONY: all

all:
	gcc -c door.c
	gcc door.o -lbcm2835 -o door
