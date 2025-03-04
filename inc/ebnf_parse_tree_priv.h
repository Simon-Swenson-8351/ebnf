#pragma once

#include "enbf_parse_tree.h"

typedef struct ebnf_parse_tree 
{
    char *prod_rule_name;
    char *text;
    ebnf_parse_tree_t *children;
    size_t children_count;
} ebnf_parse_tree_t;
