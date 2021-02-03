#ifndef __PARSER_PARSER_REG_H__
#define __PARSER_PARSER_REG_H__

#include "parser.h"

typedef enum
{
  IDF_TYPE_INT,
  IDF_TYPE_STR,
  IDF_TYPE_FLT,
  IDF_TYPE_AUT
} identifier_type_t;

typedef struct
{
  int id;
  identifier_type_t type;
  char *value;
} identifier_t;

typedef struct
{
  int count; // Determines identity.
  identifier_t *identifiers[];
} parser_register_t;

extern void parser_register_add(parser_register_t *reg, identifier_t *idf);
extern void parser_register_remove(parser_regitser_t *reg, int idf_id);
extern void parser_register_update(parser_register_t *reg, int idf_id, char *newvalue);

#endif