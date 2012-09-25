/* Grupo 3: Rafael / Gabriel Teixeira */
#include <stdio.h>
#include "lzw.h"

void print_output_codeword(uint16_t codeword) {
  printf("Output codeword: 0x%04X\n",codeword);
}

void write_to_file(uint16_t codeword) {
  static uint8_t init = 0;
  static FILE *output_file;
  static uint8_t nibble,nibble_avail=0;

  if(!init) {
    output_file = fopen("output.lzw","wb");
	if(!output_file) {
	  return;
	}
	init = 1;
  }

  if(nibble_avail) {
	if(codeword == 0xFFFF) {
	  fputc(((nibble&0xF) << 4), output_file);
	  fclose(output_file);
	  init=0;
	  return;
	}

	fputc(((nibble&0xF) << 4) | (codeword>>8), output_file);
	fputc(codeword&0xFF, output_file);

	nibble_avail = 0;
  } else {
	if(codeword == 0xFFFF) {
	  fclose(output_file);
	  init=0;
	  return;
	}
	fputc(codeword>>4, output_file);
	nibble = codeword&0xF;
	nibble_avail = 1;
  }

}

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
	init_lzw_encoder(&encoder,char_input,write_to_file);
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
