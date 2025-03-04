#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "ebnf.h"

typedef struct ebnf_prod_rule ebnf_prod_rule_t;

typedef enum ebnf_state
{
    EBNF_STATE_BUILDING,
    EBNF_STATE_VALIDATED
} ebnf_state_t;

typedef struct ebnf {
    ebnf_state_t state;
    ebnf_prod_rule_t *prod_rules;
    size_t count;
    size_t cap;
} ebnf_t;

ebnf_err_t ebnf_init(ebnf_t *borrowed_to_init);
void ebnf_clear(ebnf_t *borrowed_to_clear);
