#include "dict.h"

int init_dict(dictionary_t *dict, char *input_dict) {
  int i;

  dict->file_name = input_dict;

  dict->size = 0;

  if(input_dict != NULL) {
	FILE *input_dict_file;
	char atribute;
	int value;
	input_dict_file = fopen(input_dict,"r");

	if(input_dict_file == NULL) {
	  fprintf(stderr,"Could not open dictionary file %s.\n",input_dict);
	  return -1;
	}

	while (!feof(input_dict_file)) {
	  if (fscanf(input_dict_file,"%c:%d\n",&atribute,&value) != 2) continue;
	  if(value>dict->size) {
		for(i=dict->size;i<value;i++) {
		  /* It is adding a character past the dict size, reset entries before */
		  dict->entry[i].append = 0;
		  dict->entry[i].prefix = 0;
		  dict->entry[i].len = 1;
		  dict->size = value+1;
		}
	  }
	  dict->entry[value].append = atribute;
	  dict->entry[value].prefix = 0;
	  dict->entry[value].len = 1;
	  dict->size = dict->size>value ? dict->size : value+1;
	}

  } else {
	/* If there is no input dictionary, use ASCII table as input */
	for(i=0; i<256; i++) {
	  dict->entry[i].append = i;
	  dict->entry[i].prefix = 0;
	  dict->entry[i].len = 1;
	}
	dict->size = 256;
  }

  return 0;
}

void insert_in_dictionary(
  uint16_t prefix,
  uint8_t append,
  dictionary_t *dict
) {
  uint16_t dict_pos = dict->size;

  if(dict->size == DICT_MAX_SIZE) {
    printf("Dictionary reseted due to oversize.\n");
    init_dict(dict,dict->file_name);
  } else {
	dict->entry[dict_pos].append = append;
	dict->entry[dict_pos].prefix = prefix;
	dict->entry[dict_pos].len = dict->entry[prefix].len+1;
	/* The resulting lenght if 1 more of the prefix */
	dict->size++;
  }
}

uint16_t search_in_dictionary(
  uint16_t prefix,
  uint8_t append,
  dictionary_t *dict
) {
  uint16_t i;

  /* Remember that a prefix entry must always come before the appended one
      Ex: AAB is always before AABB */

  if(prefix+1 == dict->size) return DICT_NOT_FOUND;
  /* If the prefix is the last entry in the dictionary, there are no possible
      further entries (see comment above) */

  for(i=prefix+1; i<dict->size; i++) {
	/* Search starting after the position of the prefix */
    if((dict->entry[i].prefix == prefix) && (dict->entry[i].append == append)) {
	  /* If found a matching entry returns the position */
	  return i;
	}
  }

  /* If nothing is found returns DICT_NOT_FOUND */
  return DICT_NOT_FOUND;
}
