/* Grupo 3: Rafael / Gabriel Teixeira */
#include <stdio.h>
#include "lzw.h"

uint16_t read_from_file() {
  static uint8_t init = 0;
  static FILE *input_file;
  static uint8_t nibble,nibble_avail=0;
  uint16_t codeword;

  if(!init) {
    input_file = fopen("output.lzw","rb");
	if(!output_file) {
	  return;
	}
	init = 1;
  }

  if(nibble_avail) {
	uint8_t second_byte;
	second_byte = fgetc(input_file);
	if(feof(input_file)) {
	  fclose(input_file);
	  init=0;
	  nibble_avail=0;
	  return 0xFFFF;
	}

	codeword = ((nibble&0xF) << 8) | second_byte;

	nibble_avail = 0;
  } else {
	uint8_t first_byte,second_byte;
	first_byte = fgetc(input_file);
	if(feof(input_file)) {
	  fclose(input_file);
	  init=0;
	  nibble_avail=0;
	  return 0xFFFF;
	}

	second_byte = fgetc(input_file);
	if(feof(input_file)) {
	  fclose(input_file);
	  init=0;
	  nibble_avail=0;
	  return 0xFFFF;
	}

	nibble = codeword&0xF;

	codeword = (first_byte << 4) | (second_byte >> 4);

	nibble_avail = 1;
  }

  return codeword;

}

int main(int argc, char **argv) {
  FILE *input_file;
  dictionary_t dict;
  lzw_dec_t decoder;
  char char_output;

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
	init_lzw_decoder(&encoder,write_to_file);
  }

  char_input = fgetc(input_file);

  while(char_input != EOF) {
	insert_char(&encoder,&dict,char_input);
	char_input = fgetc(input_file);
  }

  output_last_codeword(&encoder);

  write_to_file(0xFFFF);
  return 0;
}
