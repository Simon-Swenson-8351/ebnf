#include "ebnf_priv.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "ebnf_err_priv.h"
#include "ebnf_expr_priv.h"
#include "ebnf_prod_rule_priv.h"

ebnf_err_t ebnf_create(ebnf_t **given_out_created)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!given_out_created) return EBNF_ERR_NULL_ARG;
#endif
    *given_out_created = malloc(sizeof(ebnf_t));
#ifdef EBNF_CHECK_ALLOC
    if(!(*given_out_created)) return EBNF_ERR_ALLOC;
#endif
    return ebnf_init(*given_out_created);
}

ebnf_err_t ebnf_init(ebnf_t *borrowed_to_init)
{
#define INITIAL_CAP 2
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!borrowed_to_init) return EBNF_ERR_NULL_ARG;
#endif
    borrowed_to_init->state = EBNF_STATE_BUILDING;
    borrowed_to_init->prod_rules = malloc(sizeof(ebnf_prod_rule_t) * INITIAL_CAP);
#ifdef EBNF_CHECK_ALLOC
    if(!borrowed_to_init->prod_rules) return EBNF_ERR_ALLOC;
#endif
    borrowed_to_init->cap = INITIAL_CAP;
    borrowed_to_init->count = 0;
    return EBNF_ERR_SUCCESS;
#undef INITIAL_CAP
}

ebnf_err_t ebnf_add_prod_rule(ebnf_t *borrowed_ebnf, ebnf_prod_rule_t *given_prod_rule)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!borrowed_ebnf || !given_prod_rule) return EBNF_ERR_NULL_ARG;
#endif
    if(borrowed_ebnf->count == borrowed_ebnf->cap)
    {
        if(borrowed_ebnf->cap == 0)
        {
            borrowed_ebnf->cap = 1;
            borrowed_ebnf->prod_rules = malloc(sizeof(ebnf_prod_rule_t));
        }
        else
        {
            borrowed_ebnf->cap *= 2;
            borrowed_ebnf->prod_rules = realloc(borrowed_ebnf->prod_rules, sizeof(ebnf_prod_rule_t) * borrowed_ebnf->cap);
        }
#ifdef EBNF_CHECK_ALLOC
        if(!borrowed_ebnf->prod_rules) return EBNF_ERR_ALLOC;
#endif
    }
    borrowed_ebnf->prod_rules[borrowed_ebnf->count] = *given_prod_rule;
    borrowed_ebnf->count++;
    return EBNF_ERR_SUCCESS;
}

ebnf_validation_result_t ebnf_validate(ebnf_t *borrowed_ebnf)
{
    ebnf_validation_result_t result;
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!borrowed_ebnf)
    {
        result.type = EBNF_ERR_NULL_ARG;
        result.borrowed_duplicated_rule_name = NULL;
        result.borrowed_invalid_rule_name_reference = NULL;
        goto end;
    }
#endif
    /*
    1)  Simultaneously ensure there are no duplicated rule names and collect the rule names.
    2)  Use the collected rule names to verify that no expressions reference an invalid rule name.
     */
    char **rule_names = malloc(sizeof(char *) * borrowed_ebnf->count + 1);
#ifdef EBNF_CHECK_ALLOC
    if(!rule_names)
    {
        result.type = EBNF_ERR_ALLOC;
        result.borrowed_duplicated_rule_name = NULL;
        result.borrowed_invalid_rule_name_ref = NULL;
        goto end;
    }
#endif
    for(size_t i = 0; i < borrowed_ebnf->count; i++)
    {
        for(size_t j = 0; j < i; j++)
        {
            if(strcmp(rule_names[j], borrowed_ebnf->prod_rules[i].name) == 0)
            {
                result.type = EBNF_ERR_DUPE_PROD_RULE_NAME;
                result.borrowed_duplicated_rule_name = rule_names[j];
                result.borrowed_invalid_rule_name_ref = NULL;
                goto cleanup_rule_names;
            }
        }
        rule_names[i] = borrowed_ebnf->prod_rules[i].name;
    }
    rule_names[borrowed_ebnf->count] = NULL;

    for(size_t i = 0; i < borrowed_ebnf->count; i++)
    {
        ebnf_expr_validation_result_t expr_result = ebnf_expr_validate_rule_references(&borrowed_ebnf->prod_rules[i].expression, rule_names);
        if(expr_result.type != EBNF_ERR_SUCCESS)
        {
            result.type = expr_result.type;
            if(expr_result.type == EBNF_ERR_INVALID_PROD_RULE_NAME_REF)
            {
                result.borrowed_invalid_rule_name_ref = expr_result.borrowed_invalid_rule_name_ref;
            }
            else
            {
                result.borrowed_invalid_rule_name_ref = NULL;
            }
            result.borrowed_duplicated_rule_name = NULL;
            goto cleanup_rule_names;
        }
    }
    borrowed_ebnf->state = EBNF_STATE_VALIDATED;
    result.type = EBNF_ERR_SUCCESS;
    result.borrowed_duplicated_rule_name = NULL;
    result.borrowed_invalid_rule_name_ref = NULL;
cleanup_rule_names:
    free(rule_names);
end:
    return result;
}

void ebnf_clear(ebnf_t *borrowed_to_clear)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!borrowed_to_clear) return;
#endif
    for(size_t i = 0; i < borrowed_to_clear->count; i++)
    {
        ebnf_prod_rule_clear(&borrowed_to_clear->prod_rules[i]);
    }
    free(borrowed_to_clear->prod_rules);
}

void ebnf_destroy(ebnf_t *given_to_destroy)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!given_in_to_destroy) return;
#endif
    ebnf_clear(given_to_destroy);
    free(given_to_destroy);
}
