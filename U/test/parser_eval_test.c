#include "../include/parser/parser_node.h"
#include "../include/parser/parser_eval.h"
#include "../include/lexer/lex.h"
#include "../../Unity/src/unity.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void expr1()
{
  node_binary_t *rt = malloc(sizeof(node_binary_t));
  lexer_t lexer = lex_create_from_string("2-(4+3)*(8*(6-3+(4-1)+4)-4*(2-3)^2)");
  tok_list_t tokens = lex_make_toks(&lexer);
  parser_t parser = parser_create(&tokens);
  node_binary_tree(0, parser.tok_list->count - 1, &parser, rt);
  TEST_ASSERT_EQUAL(-530, (int)node_binary_tree_eval(rt).result);
  tok_list_delete(&tokens);
  lex_destroy(&lexer);
  parser_destroy(&parser);
  node_binary_tree_delete(rt);
}

void expr2()
{
  node_binary_t *rt = malloc(sizeof(node_binary_t));
  lexer_t lexer = lex_create_from_string("2^2-3+4*(2-9)");
  tok_list_t tokens = lex_make_toks(&lexer);
  parser_t parser = parser_create(&tokens);
  node_binary_tree(0, parser.tok_list->count - 1, &parser, rt);
  TEST_ASSERT_EQUAL(-27, (int)node_binary_tree_eval(rt).result);
  tok_list_delete(&tokens);
  lex_destroy(&lexer);
  parser_destroy(&parser);
  node_binary_tree_delete(rt);
}

void expr3()
{
  node_binary_t *rt = malloc(sizeof(node_binary_t));
  lexer_t lexer = lex_create_from_string("5-5/5-5");
  tok_list_t tokens = lex_make_toks(&lexer);
  parser_t parser = parser_create(&tokens);
  node_binary_tree(0, parser.tok_list->count - 1, &parser, rt);
  TEST_ASSERT_EQUAL(-1, (int)node_binary_tree_eval(rt).result);
  tok_list_delete(&tokens);
  lex_destroy(&lexer);
  parser_destroy(&parser);
  node_binary_tree_delete(rt);
}

void expr4()
{
  node_binary_t *rt = malloc(sizeof(node_binary_t));
  lexer_t lexer = lex_create_from_string("0-(-2)");
  tok_list_t tokens = lex_make_toks(&lexer);
  parser_t parser = parser_create(&tokens);
  node_binary_tree(0, parser.tok_list->count - 1, &parser, rt);
  TEST_ASSERT_EQUAL(-2, (int)node_binary_tree_eval(rt).result);
  tok_list_delete(&tokens);
  lex_destroy(&lexer);
  parser_destroy(&parser);
  node_binary_tree_delete(rt);
}

void expr5()
{
  node_binary_t *rt = malloc(sizeof(node_binary_t));
  lexer_t lexer = lex_create_from_string("1");
  tok_list_t tokens = lex_make_toks(&lexer);
  parser_t parser = parser_create(&tokens);
  node_binary_tree(0, parser.tok_list->count - 1, &parser, rt);
  TEST_ASSERT_EQUAL(1, (int)node_binary_tree_eval(rt).result);
  tok_list_delete(&tokens);
  lex_destroy(&lexer);
  parser_destroy(&parser);
  node_binary_tree_delete(rt);
}

int main()
{
  UNITY_BEGIN();
  RUN_TEST(expr1);
  RUN_TEST(expr2);
  RUN_TEST(expr3);
  RUN_TEST(expr4);
  RUN_TEST(expr5);
  return UNITY_END();
}