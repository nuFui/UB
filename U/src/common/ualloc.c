#include "../../include/common/ualloc.h"

void ufree(void *ptr) {
  free(ptr);
  ptr = NULL;
}

void *ualloc(error_pos_t *pos, int size) {
  void *block;
  if (!(block = malloc(size))) {
    error_raise(error_memory, pos, "Could not allocate sufficient memory of %d bytes", size);
    exit(EXIT_FAILURE);
  }
  return block;
}

void *urealloc(error_pos_t *pos, void *origin, int newsize) {
  void *neworigin;
  if (!(neworigin = realloc(origin, newsize))) {
    error_raise(error_memory, pos, "Could not reallocate memory to %d bytes", newsize);
    exit(EXIT_FAILURE);
  }
  return neworigin;
}