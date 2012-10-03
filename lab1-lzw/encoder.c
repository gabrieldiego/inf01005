/* Grupo 3: Rafael / Gabriel Teixeira */
#include <stdio.h>
#include <string.h>
#include "lzw.h"

//#define ENCODE_IN_ASCII

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

typedef struct write_to_file_ascii_s {
  char *output_file_name;
  FILE *output_file;
  uint8_t bits_encoded;
} write_to_file_ascii_t;

int init_write_to_file_ascii(write_to_file_ascii_t *ctx, char *output_file_name) {
  uint8_t group_number=3; /* Group 3 */

  ctx->output_file_name = output_file_name;
  ctx->output_file = fopen(ctx->output_file_name,"wb");
  if(!ctx->output_file) {
	fprintf(stderr,"Error while opening %s\n",ctx->output_file_name);
	return -1;
  }

  fputc(group_number, ctx->output_file);
  /*This should be the number of bits encoded, but this only will be known at
	the end of the file encoding */
  fputc(group_number, ctx->output_file);

  ctx->bits_encoded = 0;
  return 0;

}

void write_to_file_ascii(uint16_t codeword, void *ptr) {
  write_to_file_ascii_t *ctx = (write_to_file_ascii_t *)ptr;
  int i;

  if(ctx->bits_encoded > ctx->bits_encoded+12) {
	fprintf(stderr,"Number of bits encoded overflows.\n");
	fprintf(stderr,"Files can't have more than 256 bits according to spec.\n");

	/* Finish writing the file anyways */
	codeword = 0xFFFF;
  } else {
	if(codeword != 0xFFFF) {
	  /* If no overflow is expected, increase the number of bits */
	  ctx->bits_encoded += 12;
	}
  }

  if(codeword == 0xFFFF) {
	/* This codeword is to close the output file */
	fseek(ctx->output_file,1,SEEK_SET);

	/* Keep in mind that this overflows after 256 */
	fputc(ctx->bits_encoded, ctx->output_file);

	fclose(ctx->output_file);
	return;
  }

  /* Bit banging MSB */
  for(i=0;i<12;i++) {
	if(codeword&0x400) {
	  fputc('1', ctx->output_file);
	} else {
	  fputc('0', ctx->output_file);
	}
	codeword <<=1; /* Shift the data to get the next bit */
  }

}

void generate_report(
  FILE *outfile,
  lzw_enc_t *encoder_ctx, dictionary_t *dict, char * input_file
) {
  fprintf(outfile,"Report of the encoding:\n");
  fprintf(outfile,"\n");
  fprintf(outfile,"Input file: %s\n",input_file);
  fprintf(outfile,"\n");
  fprintf(outfile,"Input bytes: %d\n",encoder_ctx->input_count);
  fprintf(outfile,"Output bits: %d\n",encoder_ctx->output_count*12);
  fprintf(outfile,"\n");
  fprintf(outfile,"Compression rate %0.2f %%\n",
	(1200.0*encoder_ctx->output_count)/(encoder_ctx->input_count*8.0));
  fprintf(outfile,"Final dictionary:\n");
  write_dict_to_file(stdout, dict);
}

int main(int argc, char **argv) {
  FILE *input_file;
  dictionary_t dict;
  lzw_enc_t encoder;
  char char_input;
  char *output_file_name;
  size_t output_file_name_len;
#ifdef ENCODE_IN_ASCII
  write_to_file_ascii_t write_to_file_ascii_ctx;
#else /* ENCODE_IN_ASCII */
  write_to_file_t write_to_file_ctx;
#endif /* ENCODE_IN_ASCII */

  if(argc < 2) {
    fprintf(stderr,"Usage.\n");
    fprintf(stderr,"  %s <input file> [<dictionary file>] > [report file]\n",argv[0]);
    exit(-1);
  }

  input_file = fopen(argv[1],"r");
  if (input_file == NULL) {
    fprintf(stderr,"Error opening input file %s.\n",argv[1]);
	return -1;
  }

  output_file_name_len = strlen(argv[1])+5;
  output_file_name = malloc(output_file_name_len);

  memcpy(output_file_name, argv[1], strchr(argv[1],'.')-argv[1]+1);
  strcpy(output_file_name+(strchr(argv[1],'.')-argv[1]+1),"dout");

#ifdef ENCODE_IN_ASCII
  if(init_write_to_file_ascii(&write_to_file_ascii_ctx, output_file_name)) {
	return -1;
  }
#else /* ENCODE_IN_ASCII */
  if(init_write_to_file(&write_to_file_ctx, output_file_name)) {
	return -1;
  }
#endif /* ENCODE_IN_ASCII */

  if(argc >= 3) {
	init_dict(&dict,argv[2]);
  } else {
	init_dict(&dict,NULL);
  }

  char_input = fgetc(input_file);
  if(char_input != EOF) {
#ifdef ENCODE_IN_ASCII
	init_lzw_encoder(&encoder,char_input,write_to_file_ascii,&dict,&write_to_file_ascii_ctx);
#else /* ENCODE_IN_ASCII */
	init_lzw_encoder(&encoder,char_input,write_to_file,&dict,&write_to_file_ctx);
#endif /* ENCODE_IN_ASCII */
  }

  char_input = fgetc(input_file);

  while(char_input != EOF) {
	insert_char(&encoder,&dict,char_input);
	char_input = fgetc(input_file);
  }

  output_last_codeword(&encoder,&dict);

#ifdef ENCODE_IN_ASCII
  write_to_file_ascii(0xFFFF,&write_to_file_ascii_ctx);
#else /* ENCODE_IN_ASCII */
  write_to_file(0xFFFF,&write_to_file_ctx);
#endif /* ENCODE_IN_ASCII */

  generate_report(stdout, &encoder, &dict, argv[1]);

  return 0;
}
