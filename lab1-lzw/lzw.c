/* Grupo 3: Rafael / Gabriel Teixeira */
#include "lzw.h"

void init_lzw_encoder(
  lzw_enc_t *encoder_ctx,
  char first_char,
  void (*output_func)(uint16_t)
) {
  encoder_ctx->current_prefix = first_char;
  encoder_ctx->output_func = output_func;
}

void insert_char(lzw_enc_t *encoder_ctx, dictionary_t *dict, char input_char) {
  uint16_t codeword;

  /* Search the dict for the current prefix + input_char */
  codeword = search_in_dictionary(encoder_ctx->current_prefix,input_char,dict);

  if(codeword != DICT_NOT_FOUND) {
	/* If found in dictionary, save the current prefix and continues */
	encoder_ctx->current_prefix = codeword;
  } else {
	/* If not found, add it to the dictionary. Remember that at this point the
	    dictionary may be resetted, but this changes nothing */
	insert_in_dictionary(encoder_ctx->current_prefix,input_char,dict);
	printf("Output codeword: 0x%04X\n",encoder_ctx->current_prefix);
	encoder_ctx->current_prefix = input_char;
  }
}

void output_last_codeword(lzw_enc_t *encoder_ctx) {
  printf("Output codeword: 0x%04X\n",encoder_ctx->current_prefix);
}

/* Clear dictionary once it is full */
