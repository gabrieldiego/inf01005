/* Grupo 3: Rafael / Gabriel Teixeira */
#include "lzw.h"

void init_lzw_encoder(
  lzw_enc_t *encoder_ctx,
  char first_char,
  void (*output_func)(uint16_t, void*),
  void *output_func_ctx
) {
  encoder_ctx->current_prefix = first_char;
  encoder_ctx->output_func = output_func;
  encoder_ctx->output_func_ctx = (void *)output_func_ctx;
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
	encoder_ctx->output_func(
	  encoder_ctx->current_prefix,
	  encoder_ctx->output_func_ctx
	);
	encoder_ctx->current_prefix = input_char;
  }
}

void output_last_codeword(lzw_enc_t *encoder_ctx) {
  encoder_ctx->output_func(
    encoder_ctx->current_prefix,
    encoder_ctx->output_func_ctx
  );
}

/* Clear dictionary once it is full */

void init_lzw_decoder(
  lzw_dec_t *decoder_ctx,
  uint16_t (*input_func)(void),
  void (*output_func)(uint8_t)
) {
  decoder_ctx->input_func = input_func;
  decoder_ctx->output_func=output_func;

  decoder_ctx->previous_codeword = decoder_ctx->input_func();

  if(decoder_ctx->previous_codeword == 0xFFFF) {
	/* Probably means empty or inexisting file */
    return;
  }

  decoder_ctx->output_func(decoder_ctx->previous_codeword);

  decoder_ctx->last_char = decoder_ctx->previous_codeword;
}

size_t decode_file(lzw_dec_t *decoder_ctx, dictionary_t *dict) {
  uint16_t current_codeword;

  /* Take one codeword from file for analisys */
  current_codeword = decoder_ctx->input_func();

  while(current_codeword != 0xFFFF) {
	current_codeword = decoder_ctx->input_func();
  }

#if 0
  /* Search the dict for the current prefix + input_char */
  codeword = search_in_dictionary(encoder_ctx->current_prefix,input_char,dict);
  if(codeword != DICT_NOT_FOUND) {
	/* If found in dictionary, save the current prefix and continues */
	encoder_ctx->current_prefix = codeword;
  } else {
	/* If not found, add it to the dictionary. Remember that at this point the
	    dictionary may be resetted, but this changes nothing */
	insert_in_dictionary(encoder_ctx->current_prefix,input_char,dict);
	encoder_ctx->output_func(encoder_ctx->current_prefix);
	encoder_ctx->current_prefix = input_char;
  }
#endif
}

/* Outputs the codeword string recursivelly */
void output_cw_rec(lzw_dec_t *decoder_ctx, dictionary_t *dict, uint16_t codeword) {
  if(dict->entry[codeword].len>1) {
	output_cw_rec(decoder_ctx,dict,dict->entry[codeword].prefix);
  }
  decoder_ctx->output_func(dict->entry[codeword].append);
}
