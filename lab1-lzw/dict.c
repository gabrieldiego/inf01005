#include "dict.h"

void init_dict(dictionary_t *dict) {
  int i;

  for(i=0; i<256; i++) {
    dict->entry[i].append = i;
    dict->entry[i].prefix = 0;
    dict->entry[i].len = 1;
  }

  dict->size = 256;
}

void insert_in_dictionary(
  uint16_t prefix,
  uint8_t append,
  dictionary_t *dict
) {
  uint16_t dict_pos = dict->size;

  if(dict->size == DICT_MAX_SIZE) {
    printf("Dictionary reseted due to oversize.");
    init_dict(dict);
  } else {
	dict->entry[dict_pos].append = append;
	dict->entry[dict_pos].prefix = prefix;
	dict->entry[dict_pos].len = dict->entry[prefix].len+1;
	/* The resulting lenght if 1 more of the prefix */
	dict->size++;
  }
}

int search_in_dictionary(
  uint16_t prefix,
  uint8_t append,
  dictionary_t *dict
) {
  uint16_t i;

  /* Remember that a prefix entry must always come before the appended one
      Ex: AAB is always before AABB */

  if(prefix+1 == dict->size) return 0;
  /* If the prefix is the last entry in the dictionary, there are no possible
      further entries (see comment above) */

  for(i=prefix+1; i<dict->size; i++) {
	/* Search starting after the position of the prefix */
    if((dict->prefix == prefix) && (dict->append == append)) {
	  /* If found a matching entry returns the position */
	  return i;
	}
  }

  /* If nothing is found returns 0 */
  /* If the position found was 0, returns 0 too, but this function is not
      supposed to search by a entry of length 1 (and in the position 0 ) */
  return 0;
}
