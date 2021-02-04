#ifndef __PARSER_PARSER_REG_H__
#define __PARSER_PARSER_REG_H__

#include <string.h>  // for memmove()

#include "../common/error.h"
#include "../common/ualloc.h"
#include "../lexer/tok.h"

typedef struct
{
  int id;
  tok_type_t type;
  char *name;
  char *value;
} identifier_t;

typedef struct
{
  int count;  // Determines identity.
  identifier_t *identifiers[];
} parser_register_t;

extern parser_register_t parser_register_create();
extern void parser_register_destroy(parser_register_t *reg);

extern void parser_register_add(parser_register_t *reg, identifier_t *idf);
extern void parser_register_remove(parser_register_t *reg, int idf_id);
extern void parser_register_update(parser_register_t *reg, int idf_id, char *newvalue);

#endif