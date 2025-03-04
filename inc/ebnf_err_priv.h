#pragma once

#include "ebnf_err.h"

typedef enum ebnf_err_internal {
    EBNF_ERR_SUCCESS = 0,
    EBNF_ERR_NULL_ARG,
    EBNF_ERR_BAD_ARGS,
    EBNF_ERR_ALLOC,
    EBNF_ERR_DUPE_PROD_RULE_NAME,
    EBNF_ERR_INVALID_PROD_RULE_NAME_REF,
    EBNF_ERR_COUNT
} ebnf_err_internal_t;
