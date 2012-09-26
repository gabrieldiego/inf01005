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
	if(!input_file) {
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

  printf("Read codeword 0x%3X\n",codeword);

  return codeword;

}

void output_to_file(uint8_t output_char) {
  static FILE *output_file=NULL;

  if(output_file == NULL) {
    output_file = fopen("output.txt","rb");
	if(!output_file) {
	  return;
	}
  }

  fputc(output_char,output_file);

}

int main(int argc, char **argv) {
  FILE *input_file;
  dictionary_t dict;
  lzw_dec_t decoder;

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

  init_lzw_decoder(&decoder,read_from_file,output_to_file);

  decode_file(&decoder,&dict);

  return 0;
}
