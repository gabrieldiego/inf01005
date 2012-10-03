#ifndef __DICT_H__
#define __DICT_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define DICT_MAX_SIZE 4096

#define DICT_NOT_FOUND 0xFFFF
#define DICT_NULL_PREFIX 0xFFFE

typedef struct dictionary_entry_s {
  size_t len;
  uint16_t prefix; // If len == 1, prefix is -1 (single char)
  uint8_t append;
} dictionary_entry_t;

typedef struct dictionary_s {
  dictionary_entry_t entry[DICT_MAX_SIZE];
  uint16_t size;
  char *file_name;
} dictionary_t;

int init_dict(dictionary_t *dict, char *input_dict);

void insert_in_dictionary(
  uint16_t prefix,
  uint8_t append,
  dictionary_t *dict
);

uint16_t search_in_dictionary(
  uint16_t prefix,
  uint8_t append,
  dictionary_t *dict
);

void write_codeword_to_file_rec(
  FILE *out_file,
  dictionary_t *dict,
  uint16_t entry
);

void write_dict_to_file(FILE *out_file, dictionary_t *dict);

#endif /* __DICT_H__ */
