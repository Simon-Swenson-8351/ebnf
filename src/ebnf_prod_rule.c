#include "ebnf_prod_rule_priv.h"

#include "ebnf_err_priv.h"
#include "ebnf_expr_priv.h"

ebnf_err_t ebnf_prod_rule_create(char *given_name, ebnf_expr_t *given_expr, ebnf_prod_rule_t **given_out_created)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!given_name || !given_expr || *given_out_created) return EBNF_ERR_NULL_ARG;
#endif
    *given_out_created = malloc(sizeof(ebnf_prod_rule_t));
#ifdef EBNF_CHECK_ALLOC
    if(!(*given_out_created)) return EBNF_ERR_ALLOC;
#endif
    return ebnf_prod_rule_init(*given_out_created, given_name, given_expr);
}

ebnf_err_t ebnf_prod_rule_init(ebnf_prod_rule_t *borrowed_to_init, char *given_name, ebnf_expr_t *given_expr)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!borrowed_to_init || !given_name || !given_expr) return EBNF_ERR_NULL_ARG;
#endif
    borrowed_to_init->name = given_name;
    borrowed_to_init->expression = *given_expr;
    return EBNF_ERR_SUCCESS;
}

void ebnf_prod_rule_clear(ebnf_prod_rule_t *borrowed_to_clear)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!borrowed_to_clear) return;
#endif
    free(borrowed_to_clear->name);
    ebnf_expr_clear(&borrowed_to_clear->expression);
}

void ebnf_prod_rule_destroy(ebnf_prod_rule_t *given_to_destroy)
{
#ifdef EBNF_CHECK_PRECONDITIIONS
    if(!given_to_destroy) return;
#endif
    ebnf_prod_rule_clear(given_to_destroy);
    free(given_to_destroy);
}
