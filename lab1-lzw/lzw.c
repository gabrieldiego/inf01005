/* Grupo 3: Rafael / Gabriel Teixeira */

#define VEC_SIZE 256

static char *alloc_vector = NULL;
static size_t alloc_vector_pos;

char *alloc_bytes(size_t count) {
  char return_vector;

  if(alloc_vector == NULL || (VEC_SIZE - alloc_vector_pos < count)) {
    alloc_vector = malloc(VEC_SIZE);
    if(alloc_vector == NULL) {
      exit(-1);
    }
    alloc_vector_pos = 0;
  }

  if(count > VEC_SIZE) {

    return_vector = alloc_vector + alloc_vector_pos;

    alloc_vector_pos += count;

    return return_vector;
  }
}

/* Clear dictionary once it is full */

