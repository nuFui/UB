#ifndef __PARSER_PARSER_REG_H__
#define __PARSER_PARSER_REG_H__

#include "../common/ualloc.h"
#include "../common/error.h"

#include "../lexer/tok.h"

#include <string.h> // for memmove()?

typedef struct
{
  int id;
  tok_type_t type;
  char *name;
  char *value;
} identifier_t;

typedef struct
{
  int count; // Determines identity.
  identifier_t *identifiers[];
} parser_register_t;

extern parser_register_t parser_regitser_create();
extern void parser_register_destroy(parser_register_t *reg);

extern void parser_register_add(parser_register_t **reg, identifier_t *idf);
extern void parser_register_remove(parser_register_t *reg, int idf_id);
extern void parser_register_update(parser_register_t *reg, int idf_id, char *newvalue);


#endif