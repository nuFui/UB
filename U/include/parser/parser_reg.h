#ifndef __PARSER_PARSER_REG_H__
#define __PARSER_PARSER_REG_H__

#include <string.h>  // for memmove()

#include "../common/error.h"
#include "../common/ualloc.h"
#include "../lexer/tok.h"

#include "parser_err.h"

typedef struct {
  int id;
  tok_type_t type;
  char *name;
  char *value;
} identifier_t;

// Predefined in parser_err.
struct ParserRegister {
  int count;  // Determines identity.
  identifier_t *identifiers[];
};

// extern void parser_register_destroy(parser_register_t *reg);
extern void parser_register_add(parser_register_t **reg, identifier_t *idf);
extern void parser_register_remove(parser_register_t **reg, int idf_id);
extern void parser_register_update(parser_register_t **reg, int idf_id, char *newvalue);
extern int parser_register_contains(parser_register_t **reg, const char *idf);

#endif