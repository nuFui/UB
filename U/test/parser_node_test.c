#include "../include/parser/parser_node.h"
#include "../include/lexer/lex.h"
#include "../../munit/munit.h"

static void *setup(const MunitParameter params[], void *user_data)
{
  lexer_t lexer = lex_create(params[0].value);
  tok_list_t list = lex_make_toks(&lexer);
  parser_t parser = parser_create(&list);
  parser_node_t *root = malloc(sizeof(parser_node_t));
  // TODO IMPLEMENT
  return root;
}

static void teardown(void *root)
{
  parser_node_kill_both((parser_node_t *)root);
}

static MunitResult check_nodes_deleted(const MunitParameter params[], void *fixture)
{

  return MUNIT_OK;
}

static MunitTest tests[] = {
    {"/check_nodes_deleted",
     check_nodes_deleted,
     setup,
     teardown,
     MUNIT_TEST_OPTION_NONE,
     NULL},
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static const MunitSuite suite = {
    "/parser_node_tests",
    tests,
    NULL,
    1,
    MUNIT_TEST_OPTION_NONE};

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
  return munit_suite_main(&suite, NULL, argc, argv);
}