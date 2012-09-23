/* Grupo 3: Rafael / Gabriel Teixeira */
#include <stdint.h>

typedef struct dictionary_entry_s {
  size_t len;
  uint16_t prefix; // If len == 1, prefix is 0
  uint8_t append;
} dictionary_entry_t;

typedef struct dictionary_s {
  dictionary_entry_t entry[4096];
  uint16_t ditc_size;
} dictionary_t;
