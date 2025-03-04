#pragma once

#include <stddef.h>

#include "ebnf_err.h"
#include "ebnf.h"

typedef struct ebnf_parse_tree ebnf_parse_tree_t;

size_t ebnf_parse_tree_size(void);

ebnf_err_t ebnf_parse_tree_init_by_parsing(ebnf_parse_tree_t *borrowed_to_init, ebnf_t *borrowed_grammar, char *borrowed_to_parse);
ebnf_err_t ebnf_parse_tree_init_from_json(ebnf_parse_tree_t *borrowed_to_init, char *borrowed_json_string);

ebnf_err_t ebnf_parse_tree_get_prod_rule_name_size(ebnf_parse_tree_t *borrowed_to_query, size_t *borrowed_out);
ebnf_err_t ebnf_parse_tree_get_prod_rule_name(ebnf_parse_tree_t *borrowed_to_query, char *borrowed_out_buf, size_t out_buf_size);
ebnf_err_t ebnf_parse_tree_get_text_size(ebnf_parse_tree_t *borrowed_to_query, size_t *borrowed_out);
ebnf_err_t ebnf_parse_tree_get_text(ebnf_parse_tree_t *borrowed_to_query, char *borrowed_out_buf, size_t out_buf_size);
ebnf_err_t ebnf_parse_tree_get_child_count(ebnf_parse_tree_t *borrowed_to_query, size_t *borrowed_out);
ebnf_err_t ebnf_parse_tree_get_child(ebnf_parse_tree_t *borrowed_to_query, size_t child_idx, ebnf_parse_tree_t *borrowed_out);

void ebnf_parse_tree_clear(ebnf_parse_tree_t *borrowed_to_clear);
