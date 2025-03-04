#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "ebnf_expr.h"

typedef enum ebnf_expr_type {
    EBNF_EXPR_TYPE_LITERAL,
    EBNF_EXPR_TYPE_RULE_NAME,
    EBNF_EXPR_TYPE_CONCATENATION,
    EBNF_EXPR_TYPE_ALTERNATION,
    EBNF_EXPR_TYPE_EXCEPTION,
    EBNF_EXPR_TYPE_COUNT_SPEC,
    EBNF_EXPR_TYPE_SPECIAL
} ebnf_expr_type_t;

typedef struct ebnf_expr_list {
    ebnf_expr_t *exprs;
    size_t count;
    size_t cap;
} ebnf_expr_list_t;

typedef struct ebnf_expr_data_exception {
    ebnf_expr_t *left;
    ebnf_expr_t *right;
} ebnf_expr_data_exception_t;

typedef struct ebnf_expr_data_count_spec {
    ebnf_expr_t *expr;
    unsigned int minimum_count;
    unsigned int maximum_count;
} ebnf_expr_data_count_spec_t;

typedef char                              *ebnf_expr_data_literal_t;
typedef char                              *ebnf_expr_data_rule_name_t;
typedef ebnf_expr_list_t                   ebnf_expr_data_concatenation_t;
typedef ebnf_expr_list_t                   ebnf_expr_data_alternation_t;
typedef struct ebnf_expr_data_exception    ebnf_expr_data_exception_t;
typedef struct ebnf_expr_data_count_spec   ebnf_expr_data_count_spec_t;
typedef char                              *ebnf_expr_data_special_t;

typedef struct ebnf_expr
{
    ebnf_expr_type_t type;
    union
    {
        ebnf_expr_data_literal_t       literal;
        ebnf_expr_data_rule_name_t     rule_name;
        ebnf_expr_data_concatenation_t concatenation;
        ebnf_expr_data_alternation_t   alternation;
        ebnf_expr_data_exception_t     exception;
        ebnf_expr_data_count_spec_t    count_spec;
        ebnf_expr_data_special_t       special;
    } data;
} ebnf_expr_t;

ebnf_err_t ebnf_expr_init_literal(ebnf_expr_t *borrowed_to_init, char *given_literal);
ebnf_err_t ebnf_expr_init_rule_name(ebnf_expr_t *borrowed_to_init, char *given_rule_name);
ebnf_err_t ebnf_expr_init_special(ebnf_expr_t *borrowed_to_init, char *given_special_text);
ebnf_err_t ebnf_expr_init_concatenation(ebnf_expr_t *borrowed_to_init, ebnf_expr_list_t *given_list);
ebnf_err_t ebnf_expr_init_alternation(ebnf_expr_t *borrowed_to_init, ebnf_expr_list_t *given_list);
ebnf_err_t ebnf_expr_init_exception(ebnf_expr_t *borrowed_to_init, ebnf_expr_t *given_left_expr, ebnf_expr_t *given_right_expr);
ebnf_err_t ebnf_expr_init_count_spec(ebnf_expr_t *borrowed_to_init, ebnf_expr_t *given_inner_expr, unsigned int minimum_count_incl, unsigned int maximum_count_excl);

void ebnf_expr_clear(ebnf_expr_t *borrowed_to_clear);

ebnf_err_t ebnf_expr_list_init(ebnf_expr_list_t *borrowed_to_init, unsigned int initial_capacity);

void ebnf_expr_list_clear(ebnf_expr_list_t *borrowed_to_clear);

typedef struct ebnf_expr_validation_result
{
    ebnf_err_t type;
    char *borrowed_invalid_rule_name_ref;
} ebnf_expr_validation_result_t;
ebnf_expr_validation_result_t ebnf_expr_validate_rule_references(ebnf_expr_t *borrowed_expr, char **borrowed_valid_rule_names);
