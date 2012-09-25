/* Grupo 3: Rafael / Gabriel Teixeira */

#ifndef __LZW_H__
#define __LZW_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "dict.h"

typedef struct lzw_enc_s {
  uint16_t current_prefix;
} lzw_enc_t;

void init_lzw_encoder(lzw_enc_t *encoder_ctx, char first_char);

void insert_char(lzw_enc_t *encoder_ctx, dictionary_t *dict, char input_char);

#endif /* __LZW_H__ */
