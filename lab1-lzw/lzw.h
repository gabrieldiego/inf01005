/* Grupo 3: Rafael / Gabriel Teixeira */
typedef struct dictionary_entry_s {
  size_t len;
  uint16_t prefix; // If len == 1, prefix is 0
  uint8_t append;
} dictionary_entry;

