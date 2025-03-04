#pragma once

#include "ebnf_prod_rule.h"

#include "ebnf_expr_priv.h"

typedef struct ebnf_prod_rule {
    char *name; ///< owned
    ebnf_expr_t expression;
} ebnf_prod_rule_t;

ebnf_err_t ebnf_prod_rule_init(ebnf_prod_rule_t *borrowed_to_init, char *given_name, ebnf_expr_t *given_expr);
void ebnf_prod_rule_clear(ebnf_prod_rule_t *borrowed_to_clear);
