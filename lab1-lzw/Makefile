all: encoder

encoder:encoder.o lzw.o
	gcc -o encoder encoder.o lzw.o

encoder.o: lzw.h
	gcc -c encoder.c

lzw.o: lzw.h
	gcc -c lzw.c

