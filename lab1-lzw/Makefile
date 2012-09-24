CDFLAGS=-MM -MF $@ -MP -MT

SRC=$(shell ls *.c)
DEP=$(subst .c,.d,$(SRC))
OBJ=$(subst .c,.o,$(SRC))

all: encoder

encoder: $(OBJ)
	$(CC) -o encoder $^

-include $(DEP)

%.d: %.c
	$(CC) $(CDFLAGS) $(subst .c,.o,$<) $<

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(DEP) $(OBJ) encoder
