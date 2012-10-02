/* Grupo 3: Rafael / Gabriel Teixeira */
#include <stdio.h>
#include "lzw.h"

void print_output_codeword(uint16_t codeword) {
  printf("Output codeword: 0x%04X\n",codeword);
}

typedef struct write_to_file_s {
  char *output_file_name;
  FILE *output_file;
  uint8_t nibble;
  uint8_t nibble_avail;
} write_to_file_t;

int init_write_to_file(write_to_file_t *ctx, char *output_file_name) {
  ctx->output_file_name = output_file_name;
  ctx->output_file = fopen(ctx->output_file_name,"wb");
  if(!ctx->output_file) {
	fprintf(stderr,"Error while opening %s\n",ctx->output_file_name);
	return -1;
  }

  ctx->nibble_avail = 0;

  return 0;

}

void write_to_file(uint16_t codeword, void *ptr) {
  write_to_file_t *ctx = (write_to_file_t *)ptr;

  if(ctx->nibble_avail) {
	if(codeword == 0xFFFF) {
	  /* This codeword is to close the output file */
	  fputc(((ctx->nibble&0xF) << 4), ctx->output_file);
	  fclose(ctx->output_file);
	  return;
	}

	fputc(((ctx->nibble&0xF) << 4) | (codeword>>8), ctx->output_file);
	fputc(codeword&0xFF, ctx->output_file);

	ctx->nibble_avail = 0;

  } else {
	if(codeword == 0xFFFF) {
	  fclose(ctx->output_file);
	  return;
	}
	fputc(codeword>>4, ctx->output_file);
	ctx->nibble = codeword&0xF;
	ctx->nibble_avail = 1;
  }
}

int main(int argc, char **argv) {
  FILE *input_file;
  dictionary_t dict;
  lzw_enc_t encoder;
  char char_input;
  char output_file_name[]="output.dout";
  write_to_file_t write_to_file_ctx;
  

  if(argc < 2) {
    printf("Usage.\n");
    printf("  %s <input file> [<dictionary file>]\n",argv[0]);
    exit(-1);
  }

  input_file = fopen(argv[1],"r");
  if (input_file == NULL) {
    printf("Error opening input file %s.\n",argv[1]);
  }

  if(init_write_to_file(&write_to_file_ctx, output_file_name)) {
	return -1;
  }


  if(argc >= 3) {
	init_dict(&dict,argv[2]);
  } else {
	init_dict(&dict,NULL);
  }

  char_input = fgetc(input_file);
  if(char_input != EOF) {
	init_lzw_encoder(&encoder,char_input,write_to_file,&write_to_file_ctx);
  }

  char_input = fgetc(input_file);

  while(char_input != EOF) {
	insert_char(&encoder,&dict,char_input);
	char_input = fgetc(input_file);
  }

  output_last_codeword(&encoder);

  write_to_file(0xFFFF,&write_to_file_ctx);
  return 0;
}
