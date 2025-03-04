#include "ebnf_err_priv.h"

static const char *EBNF_ERR_STR[] = 
{
    "Success.",
    "A required argument was NULL.",
    "Given argument(s) did not match expected precondition.",
    "Dynamic allocation failed.",
    "Duplicate production rule name.",
    "Invalid production rule name reference in an expression."
};

char *ebnf_err_to_borrowed_string(ebnf_err_t err_code)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(err_code < 0 || err_code >= EBNF_ERR_COUNT) {
        return NULL;
    }
#endif
    return EBNF_ERR_STR[err_code];
}
