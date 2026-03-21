#ifndef LANG_TYPES_H
#define LANG_TYPES_H

//————————————————————————————————————————————————————————————————————————————————

#include "tree.h"

//————————————————————————————————————————————————————————————————————————————————

struct variable_ctx {
    variable_t* variable_arr;
    int         count;
    int         capacity;
};

//————————————————————————————————————————————————————————————————————————————————

struct token_t {
    int          line;
    tree_node_t* node;
};

//————————————————————————————————————————————————————————————————————————————————

struct parser_ctx_t {
    int         size;
    int         pos;
    int         cur_line;
    int         node_num;
    int         capacity;
    const char* buffer;
    token_t*    tokens;
};

//————————————————————————————————————————————————————————————————————————————————

enum class frontend_err_t {
    Success         = 0,
    SyntaxErr       = 1,
    AnotherNodeType = 2,
    AlocationErr    = 3,
    FopenErr        = 4,
    UnknownDataType = 5,
};

//————————————————————————————————————————————————————————————————————————————————

#endif // LANG_TYPES_H