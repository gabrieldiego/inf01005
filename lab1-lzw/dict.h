#ifndef __DICT_H__
#define __DICT_H__

#include <stdint.h>
#include <stdlib.h>

#define DICT_MAX_SIZE 4096

typedef struct dictionary_entry_s {
  size_t len;
  uint16_t prefix; // If len == 1, prefix is void (single char)
  uint8_t append;
} dictionary_entry_t;

typedef struct dictionary_s {
  dictionary_entry_t entry[DICT_MAX_SIZE];
  uint16_t size;
} dictionary_t;

void init_dict(dictionary_t *dict);

void insert_in_dictionary(
  uint16_t prefix,
  uint8_t append,
  dictionary_t *dict
);

int search_in_dictionary(
  uint16_t prefix,
  uint8_t append,
  dictionary_t *dict
);

#endif /* __DICT_H__ */
