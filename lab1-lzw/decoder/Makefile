all: cleandeco decoder clean
decoder: report.o dict.o decoder.o
	gcc -o decoder report.o dict.o decoder.o -lm
report.o: report.c
	gcc -o report.o -c report.c -lm
dict.o: dict.c
	gcc -o dict.o -c dict.c -lm
decoder.o: decoder.c
	gcc -o decoder.o -c decoder.c -lm
clean:
	rm -rf *.o
cleandeco:
	rm -rf *.o decode

