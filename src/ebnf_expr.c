#include "ebnf_expr_priv.h"

#include <stdlib.h>
#include <string.h>

#include "ebnf_err_priv.h"

const char *WHITESPACE_CHARACTERS[] = {" ", "\f", "\n", "\r", "\t", "\v", NULL};
const char *LOWERCASE_CHARACTERS[] = 
{
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", 
    "x", "y", "z", NULL
};
const char *UPPERCASE_CHARACTERS[] = 
{
    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", 
    "X", "Y", "Z", NULL
};
const char *DIGIT_CHARACTERS[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", NULL};

static ebnf_expr_validation_result_t validate_list(ebnf_expr_list_t *borrowed_expr_list, char **borrowed_valid_rule_names);

ebnf_err_t ebnf_expr_create_literal(char *given_literal, ebnf_expr_t **given_out_created)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!given_literal || !given_out_created) return EBNF_ERR_NULL_ARG;
#endif
    *given_out_created = malloc(sizeof(ebnf_expr_t));
#ifdef EBNF_CHECK_ALLOC
    if(!(*given_out_created)) return EBNF_ERR_ALLOC;
#endif
    ebnf_err_t result = ebnf_expr_init_literal(*given_out_created, given_literal);
    if(result) free(*given_out_created);
    return result;
}

ebnf_err_t ebnf_expr_create_rule_name(char *given_rule_name, ebnf_expr_t **given_out_created)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!given_literal || !given_rule_name) return EBNF_ERR_NULL_ARG;
#endif
    *given_out_created = malloc(sizeof(ebnf_expr_t));
#ifdef EBNF_CHECK_ALLOC
    if(!(*given_out_created)) return EBNF_ERR_ALLOC;
#endif
    ebnf_err_t result = ebnf_expr_init_rule_name(*given_out_created, given_rule_name);
    if(result) free(*given_out_created);
    return result;
}

ebnf_err_t ebnf_expr_create_special(char *given_special_text, ebnf_expr_t **given_out_created)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!given_special_text || !given_out_created) return EBNF_ERR_NULL_ARG;
#endif
    *given_out_created = malloc(sizeof(ebnf_expr_t));
#ifdef EBNF_CHECK_ALLOC
    if(!(*given_out_created)) return EBNF_ERR_ALLOC;
#endif
    ebnf_err_t result = ebnf_expr_init_special(*given_out_created, given_special_text);
    if(result) free(*given_out_created);
    return result;
}

ebnf_err_t ebnf_expr_create_concatenation(ebnf_expr_list_t *given_list, ebnf_expr_t **given_out_created)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!given_list || !given_out_created) return EBNF_ERR_NULL_ARG;
#endif
    *given_out_created = malloc(sizeof(ebnf_expr_t));
#ifdef EBNF_CHECK_ALLOC
    if(!(*given_out_created)) return EBNF_ERR_ALLOC;
#endif
    ebnf_err_t result = ebnf_expr_init_concatenation(*given_out_created, given_list);
    if(result) free(*given_out_created);
    return result;
}

ebnf_err_t ebnf_expr_create_alternation(ebnf_expr_list_t *given_list, ebnf_expr_t **given_out_created)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!given_list || !given_out_created) return EBNF_ERR_NULL_ARG;
#endif
    *given_out_created = malloc(sizeof(ebnf_expr_t));
#ifdef EBNF_CHECK_ALLOC
    if(!(*given_out_created)) return EBNF_ERR_ALLOC;
#endif
    ebnf_err_t result = ebnf_expr_init_alternation(*given_out_created, given_list);
    if(result) free(*given_out_created);
    return result;
}

ebnf_err_t ebnf_expr_create_exception(ebnf_expr_t *given_left_expr, ebnf_expr_t *given_right_expr, ebnf_expr_t **given_out_created)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!given_left_expr || !given_right_expr || !given_out_created) return EBNF_ERR_NULL_ARG;
#endif
    *given_out_created = malloc(sizeof(ebnf_expr_t));
#ifdef EBNF_CHECK_ALLOC
    if(!(*given_out_created)) return EBNF_ERR_ALLOC;
#endif
    ebnf_err_t result = ebnf_expr_init_exception(*given_out_created, given_left_expr, given_right_expr);
    if(result) free(*given_out_created);
    return result;
}

ebnf_err_t ebnf_expr_create_count_spec(ebnf_expr_t *given_inner_expr, unsigned int minimum_count_incl, unsigned int maximum_count_excl, ebnf_expr_t **given_out_created)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!given_inner_expr || !given_out_created) return EBNF_ERR_NULL_ARG;
#endif
    *given_out_created = malloc(sizeof(ebnf_expr_t));
#ifdef EBNF_CHECK_ALLOC
    if(!(*given_out_created)) return EBNF_ERR_ALLOC;
#endif
    ebnf_err_t result = ebnf_expr_init_count_spec(*given_out_created, minimum_count_incl, maximum_count_excl);
    if(result) free(*given_out_created);
    return result;
}

ebnf_err_t ebnf_expr_init_literal(ebnf_expr_t *borrowed_to_init, char *given_literal)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!borrowed_to_init || !given_literal) return EBNF_ERR_NULL_ARG;
#endif
    borrowed_to_init->type = EBNF_EXPR_TYPE_LITERAL;
    borrowed_to_init->data.literal = given_literal;
    return EBNF_ERR_SUCCESS;
}

ebnf_err_t ebnf_expr_init_rule_name(ebnf_expr_t *borrowed_to_init, char *given_rule_name)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!borrowed_to_init || !given_rule_name) return EBNF_ERR_NULL_ARG;
#endif
    borrowed_to_init->type = EBNF_EXPR_TYPE_RULE_NAME;
    borrowed_to_init->data.rule_name = given_rule_name;
    return EBNF_ERR_SUCCESS;
}

ebnf_err_t ebnf_expr_init_special(ebnf_expr_t *borrowed_to_init, char *given_special_text)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!borrowed_to_init || !given_special_text) return EBNF_ERR_NULL_ARG;
#endif
    borrowed_to_init->type = EBNF_EXPR_TYPE_SPECIAL;
    borrowed_to_init->data.special = given_special_text;
    return EBNF_ERR_SUCCESS;
}

ebnf_err_t ebnf_expr_init_concatenation(ebnf_expr_t *borrowed_to_init, ebnf_expr_list_t *given_list)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!borrowed_to_init || !given_list) return EBNF_ERR_NULL_ARG;
#endif
    borrowed_to_init->type = EBNF_EXPR_TYPE_CONCATENATION;
    borrowed_to_init->data.concatenation = *given_list;
    return EBNF_ERR_SUCCESS;
}

ebnf_err_t ebnf_expr_init_alternation(ebnf_expr_t *borrowed_to_init, ebnf_expr_list_t *given_list)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!borrowed_to_init || !given_list) return EBNF_ERR_NULL_ARG;
#endif
    borrowed_to_init->type = EBNF_EXPR_TYPE_ALTERNATION;
    borrowed_to_init->data.alternation = *given_list;
    return EBNF_ERR_SUCCESS;
}

ebnf_err_t ebnf_expr_init_exception(ebnf_expr_t *borrowed_to_init, ebnf_expr_t *given_left_expr, ebnf_expr_t *given_right_expr)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!borrowed_to_init || !given_left_expr || !given_right_expr) return EBNF_ERR_NULL_ARG;
#endif
    borrowed_to_init->type = EBNF_EXPR_TYPE_EXCEPTION;
    borrowed_to_init->data.exception.left = given_left_expr;
    borrowed_to_init->data.exception.right = given_right_expr;
    return EBNF_ERR_SUCCESS;
}

ebnf_err_t ebnf_expr_init_count_spec(ebnf_expr_t *borrowed_to_init, ebnf_expr_t *given_inner_expr, unsigned int minimum_count_incl, unsigned int maximum_count_excl)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!borrowed_to_init || !given_inner_expr) return EBNF_ERR_NULL_ARG;
    if(minimum_count_incl >= maximum_count_excl) return EBNF_ERR_BAD_ARGS;
#endif
    borrowed_to_init->type = EBNF_EXPR_TYPE_COUNT_SPEC;
    borrowed_to_init->data.count_spec.minimum_count = minimum_count_incl;
    borrowed_to_init->data.count_spec.maximum_count = maximum_count_excl;
    return EBNF_ERR_SUCCESS;
}

void ebnf_expr_clear(ebnf_expr_t *borrowed_to_clear)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!borrowed_to_clear) return;
#endif
    switch(borrowed_to_clear->type)
    {
    case EBNF_EXPR_TYPE_LITERAL:
        free(borrowed_to_clear->data.literal);
        break;
    case EBNF_EXPR_TYPE_RULE_NAME:
        free(borrowed_to_clear->data.rule_name);
        break;
    case EBNF_EXPR_TYPE_CONCATENATION:
        ebnf_expr_list_clear(&borrowed_to_clear->data.concatenation);
        free(&borrowed_to_clear->data.concatenation);
        break;
    case EBNF_EXPR_TYPE_ALTERNATION:
        ebnf_expr_list_clear(&borrowed_to_clear->data.alternation);
        free(&borrowed_to_clear->data.alternation);
        break;
    case EBNF_EXPR_TYPE_EXCEPTION:
        ebnf_expr_clear(borrowed_to_clear->data.exception.left);
        free(borrowed_to_clear->data.exception.left);
        ebnf_expr_clear(borrowed_to_clear->data.exception.right);
        free(borrowed_to_clear->data.exception.right);
        break;
    case EBNF_EXPR_TYPE_COUNT_SPEC:
        ebnf_expr_clear(borrowed_to_clear->data.count_spec.expr);
        break;
    case EBNF_EXPR_TYPE_SPECIAL:
        free(borrowed_to_clear->data.special);
        break;
    }
}

void ebnf_expr_destroy(ebnf_expr_t *given_to_destroy);

ebnf_err_t ebnf_expr_list_create(unsigned int initial_capacity, ebnf_expr_list_t **given_out_to_create);

ebnf_err_t ebnf_expr_list_init(ebnf_expr_list_t *borrowed_to_init, unsigned int initial_capacity)
{
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!borrowed_to_init) return EBNF_ERR_NULL_ARG;
#endif
    borrowed_to_init->exprs = malloc(sizeof(ebnf_expr_t) * initial_capacity);
#ifdef EBNF_CHECK_ALLOC
    if(!borrowed_to_init->exprs) return EBNF_ERR_ALLOC;
#endif
    borrowed_to_init->count = 0;
    borrowed_to_init->cap = initial_capacity;
}

ebnf_err_t ebnf_expr_list_append(ebnf_expr_list_t *borrowed_list, ebnf_expr_t *given_to_append)
{
#ifndef EBNF_CHECK_PRECONDITIONS
    if(!borrowed_list || !given_to_append) return EBNF_ERR_NULL_ARG;
#endif
    if(borrowed_list->count == borrowed_list->cap)
    {
        if(borrowed_list->cap == 0)
        {
            borrowed_list->cap = 1;
            borrowed_list->exprs = malloc(sizeof(ebnf_expr_t));
        }
        else
        {
            borrowed_list->cap *= 2;
            borrowed_list->exprs = realloc(borrowed_list->exprs, sizeof(ebnf_expr_t) * borrowed_list->cap);
        }
#ifdef EBNF_CHECK_ALLOC
        if(!borrowed_list->exprs) return EBNF_ERR_ALLOC;
#endif
    }
    borrowed_list->exprs[borrowed_list->count] = *given_to_append;
    borrowed_list->count++;
    // We don't want to clear given_to_append here because its resources were moved into the list.
    free(given_to_append);
    return EBNF_ERR_SUCCESS;
}

void ebnf_expr_list_clear(ebnf_expr_list_t *borrowed_to_clear)
{
#ifndef EBNF_CHECK_PRECONDITIONS
    if(!borrowed_to_clear) return EBNF_ERR_NULL_ARG;
#endif
    for(size_t i = 0; i < borrowed_to_clear->count; i++)
    {
        ebnf_expr_clear(&borrowed_to_clear->exprs[i]);
    }
    free(borrowed_to_clear->exprs);
}

void ebnf_expr_list_destroy(ebnf_expr_list_t *given_to_destroy);

ebnf_expr_validation_result_t ebnf_expr_validate_rule_references(ebnf_expr_t *borrowed_expr, char **borrowed_valid_rule_names)
{
    ebnf_expr_validation_result_t result;
#ifndef EBNF_CHECK_PRECONDITIONS
    if(!borrowed_expr || !borrowed_valid_rule_names)
    {
        result.type = EBNF_ERR_NULL_ARG;
        result.borrowed_invalid_rule_name_ref = NULL;
        return result;
    }
#endif
    switch(borrowed_expr->type)
    {
    case EBNF_EXPR_TYPE_LITERAL:
        break;
    case EBNF_EXPR_TYPE_RULE_NAME:
        bool found = false;
        for(char **current_rule = borrowed_valid_rule_names; *current_rule != NULL; current_rule++)
        {
            if(strcmp(borrowed_expr->data.rule_name, *current_rule) == 0)
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            result.type = EBNF_ERR_INVALID_PROD_RULE_NAME_REF;
            result.borrowed_invalid_rule_name_ref = borrowed_expr->data.rule_name;
            return result;
        }
        break;
    case EBNF_EXPR_TYPE_CONCATENATION:
        return validate_list(&borrowed_expr->data.concatenation, borrowed_valid_rule_names);
        break;
    case EBNF_EXPR_TYPE_ALTERNATION:
        return validate_list(&borrowed_expr->data.alternation, borrowed_valid_rule_names);
        break;
    case EBNF_EXPR_TYPE_EXCEPTION:
        result = ebnf_expr_validate_rule_references(borrowed_expr->data.exception.left, borrowed_valid_rule_names);
        if(result.type != EBNF_ERR_SUCCESS)
        {
            return result;
        }
        result = ebnf_expr_validate_rule_references(borrowed_expr->data.exception.right, borrowed_valid_rule_names);
        if(result.type != EBNF_ERR_SUCCESS)
        {
            return result;
        }
        break;
    case EBNF_EXPR_TYPE_COUNT_SPEC:
        return ebnf_expr_validate_rule_references(borrowed_expr->data.count_spec.expr, borrowed_valid_rule_names);
        break;
    case EBNF_EXPR_TYPE_SPECIAL:
        break;
    }
    result.type = EBNF_ERR_SUCCESS;
    result.borrowed_invalid_rule_name_ref = NULL;
    return result;
}

static ebnf_expr_validation_result_t validate_list(ebnf_expr_list_t *borrowed_expr_list, char **borrowed_valid_rule_names)
{
    ebnf_expr_validation_result_t result;
#ifdef EBNF_CHECK_PRECONDITIONS
    if(!borrowed_expr_list || !borrowed_valid_rule_names)
    {
        result.result_type = EBNF_ERR_NULL_ARG;
        result.owned_invalid_rule_name_ref = NULL;
        return result;
    }
#endif
    for(size_t i = 0; i < borrowed_expr_list->count; i++)
    {
        result = ebnf_expr_validate_rule_references(&borrowed_expr_list->exprs[i], borrowed_valid_rule_names);
        if(result.type != EBNF_ERR_SUCCESS)
        {
            return result;
        }
    }
    result.type = EBNF_ERR_SUCCESS;
    result.borrowed_invalid_rule_name_ref = NULL;
    return result;
}
