#pragma once

#include <stddef.h>

#include "ebnf_err.h"

typedef struct ebnf_expr ebnf_expr_t;
typedef struct ebnf_expr_list ebnf_expr_list_t;

ebnf_err_t ebnf_expr_create_literal(char *given_literal, ebnf_expr_t **given_out_created);
ebnf_err_t ebnf_expr_create_rule_name(char *given_rule_name, ebnf_expr_t **given_out_created);
ebnf_err_t ebnf_expr_create_special(char *given_special_text, ebnf_expr_t **given_out_created);
ebnf_err_t ebnf_expr_create_concatenation(ebnf_expr_list_t *given_list, ebnf_expr_t **given_out_created);
ebnf_err_t ebnf_expr_create_alternation(ebnf_expr_list_t *given_list, ebnf_expr_t **given_out_created);
ebnf_err_t ebnf_expr_create_exception(ebnf_expr_t *given_left_expr, ebnf_expr_t *given_right_expr, ebnf_expr_t **given_out_created);
ebnf_err_t ebnf_expr_create_count_spec(ebnf_expr_t *given_inner_expr, unsigned int minimum_count_incl, unsigned int maximum_count_excl, ebnf_expr_t **given_out_created);

ebnf_err_t ebnf_expr_create_alternation_from_string_array(char **given_string_array_of_given_strings, ebnf_expr_t **given_out_created);

ebnf_err_t ebnf_expr_create_whitespace(ebnf_expr_t **given_out_created);
ebnf_err_t ebnf_expr_create_alpha_lower(ebnf_expr_t **given_out_created);
ebnf_err_t ebnf_expr_create_alpha_upper(ebnf_expr_t **given_out_created);
ebnf_err_t ebnf_expr_create_alpha_uncased(ebnf_expr_t **given_out_created);
ebnf_err_t ebnf_expr_create_digit(ebnf_expr_t **given_out_created);
ebnf_err_t ebnf_expr_create_alphanum(ebnf_expr_t **given_out_created);
ebnf_err_t ebnf_expr_create_identifier(ebnf_expr_t **given_out_created);
ebnf_err_t ebnf_expr_create_unsigned(ebnf_expr_t **given_out_created);
ebnf_err_t ebnf_expr_create_int(ebnf_expr_t **given_out_created);
ebnf_err_t ebnf_expr_create_float(ebnf_expr_t **given_out_created);

void ebnf_expr_destroy(ebnf_expr_t *given_to_destroy);



ebnf_err_t ebnf_expr_list_create(unsigned int initial_capacity, ebnf_expr_list_t **given_out_to_create);

ebnf_err_t ebnf_expr_list_append(ebnf_expr_list_t *borrowed_list, ebnf_expr_t *given_to_append);

void ebnf_expr_list_destroy(ebnf_expr_list_t *given_to_destroy);
