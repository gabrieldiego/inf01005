#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#define S_CHAR 1
#define S_COLON 2
#define S_NUMBER 3
#define DICT_MAX_SIZE 4096

char *filename_dictionary;
int have_dictionary;


typedef struct entry_s {
  char word[100];
} entry_t;

typedef struct dictionary_s {
  entry_t entry[DICT_MAX_SIZE];
  int size;
} dictionary_t;


void init_dict(dictionary_t *dict);


void insert_in_dictionary(
  char word[100],
  dictionary_t *dict
);

void print_dictionary(
  dictionary_t *dict
);


char *search_in_dictionary(
  int code,
  dictionary_t *dict
);

int size_dictionary(
  dictionary_t *dict
);

void init_dictionary_from_file(
  char *filename, 
  dictionary_t *dict
);

void init_dictionary_with_ASCII_table(
  dictionary_t *dict
);

