#pragma once

#include "ebnf_err.h"

typedef struct ebnf_prod_rule ebnf_prod_rule_t;

ebnf_err_t ebnf_prod_rule_create(char *given_name, ebnf_expr_t *given_expr, ebnf_prod_rule_t **given_out_created);
void ebnf_prod_rule_destroy(ebnf_prod_rule_t *given_to_destroy);