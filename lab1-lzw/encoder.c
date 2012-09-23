/* Grupo 3: Rafael / Gabriel Teixeira */
#include <stdio.h>
#include "lzw.h"

int main(int argc, char **argv) {
  FILE *input_file;
  dictionary_t dict;

  if(argc < 2) {
    printf("Usage.\n");
    printf("  %s <input file> [<dictionary file>]",argv[0]);
    exit(-1);
  }

  input_file = fopen(argv[1],"r");
  if (input_file == NULL) {
    printf("Error opening input file %s.\n",argv[1]);
  }

  return 0;
}
