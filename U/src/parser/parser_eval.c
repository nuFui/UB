#include "../../include/parser/parser_eval.h"

#define MAX_DIGITS 100

static tok_type_t determine_result_kind(eval_result_t *first, eval_result_t *second) {
  if (first->kind == second->kind) {
    return first->kind;
  }
  if (first->kind == TOK_TYPE_STR && second->kind == TOK_TYPE_INT ||
      first->kind == TOK_TYPE_INT && second->kind == TOK_TYPE_STR) {
    return TOK_TYPE_STR;
  }
  if (first->kind == TOK_TYPE_INT && second->kind == TOK_TYPE_FLT ||
      first->kind == TOK_TYPE_FLT && second->kind == TOK_TYPE_INT) {
    return TOK_TYPE_FLT;
  }
  return TOK_TYPE_DUMMY_MAX;
}

// Evaluates binary operation given left, right operand and the node->op->type.
static eval_result_t node_binary_eval(node_binary_t *node, eval_result_t left, eval_result_t right) {
  error_pos_t pos = {__FILE__, __func__, __LINE__};
  tok_type_t kind = determine_result_kind(&left, &right);
  eval_result_t res = {
      kind,
      (kind != TOK_TYPE_DUMMY_MAX ? EVAL_SUCCESS : EVAL_FAILURE),
      NULL};
  if (res.code != EVAL_FAILURE) {
    switch (node->op->type) {
      case TOK_TYPE_ADD:
        if (left.kind == TOK_TYPE_STR && right.kind == TOK_TYPE_STR) {
          int newlen = strlen(left.result) + strlen(right.result) + 1;
          res.result = ualloc(&pos, newlen);
          strcat(res.result, strcat(left.result, right.result));
          res.result[newlen] = '\0';
        } else {
          res.result = ualloc(&pos, MAX_DIGITS);
          float sum = atof(left.result) + atof(right.result);
          if (res.kind == TOK_TYPE_FLT) {
            snprintf(res.result, MAX_DIGITS, "%f", sum);
          } else {
            if (LLONG_MAX < sum) {
              strcat(res.result, "inf");
              break;
            }
            snprintf(res.result, MAX_DIGITS, "%lld", (long long)sum);
          }
        }
        break;
      case TOK_TYPE_SUB:
        res.result = ualloc(&pos, MAX_DIGITS);
        float sb = atof(left.result) - atof(right.result);
        if (res.kind == TOK_TYPE_FLT) {
          snprintf(res.result, MAX_DIGITS, "%f", sb);
        } else {
          if (LLONG_MAX < sb) {
            strcat(res.result, "inf");
            break;
          }
          snprintf(res.result, MAX_DIGITS, "%lld", (long long)sb);
        }
        break;
      case TOK_TYPE_MUL:
        if (left.kind == TOK_TYPE_STR && right.kind == TOK_TYPE_INT ||
            left.kind == TOK_TYPE_INT && right.kind == TOK_TYPE_STR) {
          int multiplier = left.kind == TOK_TYPE_INT ? atoi(left.result) : atoi(right.result);
          char *str = left.kind == TOK_TYPE_STR ? left.result : right.result;
          int newlen = strlen(str) * multiplier + 1;
          res.result = ualloc(&pos, newlen);
          for (int i = 0; i < multiplier; ++i) {
            strcat(res.result, str);
          }
          res.result[newlen] = '\0';
        } else {
          res.result = ualloc(&pos, MAX_DIGITS);
          float ml = atof(left.result) * atof(right.result);
          if (res.kind == TOK_TYPE_FLT) {
            snprintf(res.result, MAX_DIGITS, "%f", ml);
          } else {
            if (LLONG_MAX < ml) {
              strcat(res.result, "inf");
              break;
            }
            snprintf(res.result, MAX_DIGITS, "%lld", (long long)ml);
          }
        }
        break;
      case TOK_TYPE_DIV:
        if (atoi(right.result) == 0) {
          res.code = EVAL_DIVZERO;
          return res;
        }
        res.result = ualloc(&pos, MAX_DIGITS);
        float dv = atof(left.result) / atof(right.result);
        if (res.kind == TOK_TYPE_FLT) {
          snprintf(res.result, MAX_DIGITS, "%f", dv);
        } else {
          if (LLONG_MAX < dv) {
            strcat(res.result, "inf");
            break;
          }
          snprintf(res.result, MAX_DIGITS, "%lld", (long long)dv);
        }
        break;
      case TOK_TYPE_POW:
        res.result = ualloc(&pos, MAX_DIGITS);
        float pw = pow(atof(left.result), atof(right.result));
        if (res.kind == TOK_TYPE_FLT) {
          snprintf(res.result, MAX_DIGITS, "%f", pw);
        } else {
          if (LLONG_MAX < pw) {
            strcat(res.result, "inf");
            break;
          }
          snprintf(res.result, MAX_DIGITS, "%lld", (long long)pw);
        }
        break;
      case TOK_TYPE_MOD:
        res.result = ualloc(&pos, MAX_DIGITS);
        float mod = fmod(atof(left.result), atof(right.result));
        if (res.kind == TOK_TYPE_FLT) {
          snprintf(res.result, MAX_DIGITS, "%f", mod);
        } else {
          if (LLONG_MAX < mod) {
            strcat(res.result, "inf");
            break;
          }
          snprintf(res.result, MAX_DIGITS, "%lld", (long long)mod);
        }
    }
  }
  return res;
}

// Evaluates binary tree from leaves up.
eval_result_t node_binary_tree_eval(parser_register_t *reg, node_binary_t *mov) {
  if (mov->op->type == TOK_TYPE_ASGN) {
    if (mov->left->op->type != TOK_TYPE_IDF) {
      parser_err_base_t err = {
          mov->left->op,
          "InvalidSyntaxErr",
          "Expected identifier name got '%s'"};
      parser_register_err_raise(&err, reg, stringify_token_type(mov->left->op->type));
    }
    error_pos_t pos = {__FILE__, __func__, __LINE__};
    identifier_t *idf = ualloc(&pos, sizeof(identifier_t));
    eval_result_t res = node_binary_tree_eval(reg, mov->right);
    if (res.code != EVAL_SUCCESS) {
      node_binary_tree_root_deinit();
      exit(EXIT_FAILURE);
    }
    idf->type = res.kind;
    idf->name = mov->left->op->value;
    idf->value = res.result;
    parser_register_add(reg, idf);  // must be referneced
    eval_result_t asgn_ret = {TOK_TYPE_IDF, res.code, NULL};
    return asgn_ret;
  }
  if (mov->op->type == TOK_TYPE_INT || mov->op->type == TOK_TYPE_FLT || mov->op->type == TOK_TYPE_STR) {
    eval_result_t ret = {
        mov->op->type,
        EVAL_SUCCESS,
        mov->op->value};
    return ret;
  }
  eval_result_t left_subtree = node_binary_tree_eval(reg, mov->left);
  eval_result_t right_subtree = node_binary_tree_eval(reg, mov->right);
  eval_result_t s = node_binary_eval(mov, left_subtree, right_subtree);
  if (s.code != EVAL_SUCCESS) {
    node_binary_tree_root_deinit();
    exit(EXIT_FAILURE);
  }
  return s;
}