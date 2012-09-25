/* Grupo 3: Rafael / Gabriel Teixeira */
#include <stdio.h>
#include "lzw.h"

int main(int argc, char **argv) {
  FILE *input_file;
  dictionary_t dict;
  lzw_enc_t encoder;
  char char_input;

  if(argc < 2) {
    printf("Usage.\n");
    printf("  %s <input file> [<dictionary file>]\n",argv[0]);
    exit(-1);
  }

  input_file = fopen(argv[1],"r");
  if (input_file == NULL) {
    printf("Error opening input file %s.\n",argv[1]);
  }

  init_dict(&dict);

  char_input = fgetc(input_file);
  if(char_input != EOF) {
	init_lzw_encoder(&encoder,char_input);
  }

  char_input = fgetc(input_file);

  while(char_input != EOF) {
	insert_char(&encoder,&dict,char_input);
	char_input = fgetc(input_file);
  }

  return 0;
}
