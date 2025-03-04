#pragma once

#include <stddef.h>

#include "ebnf_err.h"

typedef struct ebnf ebnf_t;

ebnf_err_t ebnf_create(ebnf_t **given_out_created);

ebnf_err_t ebnf_add_prod_rule(ebnf_t *borrowed_ebnf, ebnf_prod_rule_t *given_prod_rule);

typedef struct ebnf_validation_result
{
    ebnf_err_t type;
    char *borrowed_duplicated_rule_name;
    char *borrowed_invalid_rule_name_ref;
} ebnf_validation_result_t;
ebnf_validation_result_t ebnf_validate(ebnf_t *borrowed_ebnf);

void ebnf_destroy(ebnf_t *given_to_destroy);
