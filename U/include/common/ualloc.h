#ifndef __COMMON_UALLOC_H__
#define __COMMON_UALLOC_H__

#include "error.h"

extern void ufree(void *ptr);                                       // Just like free() but sets ptr to NULL.
extern void *ualloc(error_pos_t *pos, int size);                    // Just like malloc() but raises on memory allocation fail.
extern void *urealloc(error_pos_t *pos, void *origin, int newsize); // Just like realloc() ... u get the point

#endif