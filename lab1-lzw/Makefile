all: encoder

encoder: encoder.o lzw.o dict.o
	gcc -o encoder $^

encoder.o: lzw.h

lzw.o: lzw.h dict.h

dict.o: dict.h

clean:
	rm -rf *.o encoder
