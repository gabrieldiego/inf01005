CDFLAGS=-MM -MF $@ -MP -MT

SRC_ENC=dict.c encoder.c lzw.c
SRC_DEC=decoder.c dict.c lzw.c
DEP=$(subst .c,.d,$(SRC_ENC) $(SRC_DEC))
OBJ_ENC=$(subst .c,.o,$(SRC_ENC))
OBJ_DEC=$(subst .c,.o,$(SRC_DEC))

all: encoder decoder

encoder: $(OBJ_ENC)
	$(CC) -o encoder $^ -lm

decoder: $(OBJ_DEC)
	$(CC) -o decoder $^

-include $(DEP)

%.d: %.c
	$(CC) $(CDFLAGS) $(subst .c,.o,$<) $<

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(DEP) $(OBJ_ENC) $(OBJ_DEC) encoder decoder
