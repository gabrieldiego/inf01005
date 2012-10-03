/* Grupo 3: Rafael / Gabriel Teixeira */

#ifndef __LZW_H__
#define __LZW_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "dict.h"

typedef struct lzw_enc_s {
  uint16_t current_prefix;
  void (*output_func)(uint16_t, void*);
  void *output_func_ctx;
  uint32_t input_count;
  uint32_t output_count;
} lzw_enc_t;

typedef struct lzw_dec_s {
  uint16_t previous_codeword;
  char last_char;
  uint16_t (*input_func)(void);
  void (*output_func)(uint8_t);
} lzw_dec_t;

void init_lzw_encoder(
  lzw_enc_t *encoder_ctx,
  char first_char,
  void (*output_func)(uint16_t, void*),
  dictionary_t *dict,
  void *output_func_ctx
);

void insert_char(lzw_enc_t *encoder_ctx, dictionary_t *dict, char input_char);

void output_last_codeword(lzw_enc_t *encoder_ctx, dictionary_t *dict);

char decode_char(lzw_enc_t *encoder_ctx, dictionary_t *dict);


void init_lzw_decoder(
  lzw_dec_t *decoder_ctx,
  uint16_t (*input_func)(void),
  void (*output_func)(uint8_t)
);

size_t decode_file(lzw_dec_t *encoder_ctx, dictionary_t *dict);

#endif /* __LZW_H__ */
