#include "tree.h"

//--------------------------------------------------------------------------------
static tree_node_t* GetNode           (const char* buffer, int size, int* pos);
static tree_node_t* GetNum            (const char* buffer, int size, int* pos);
static tree_node_t* GetVar            (const char* buffer, int size, int* pos);
static tree_node_t* GetBasicWord      (const char* buffer, int size, int* pos);
static tree_node_t* MakeTree          (const char* buffer, int size, int* pos);
static int          CompareBasicword  (node_type_t   type    , const char* buffer, int size, int* pos,
                                       const char**  word_arr, int word_arr_size  );
static int          GetWordLen        (node_type_t   type    , int word_num       );
static int          GetBasicWordNum   (const char* buffer, int size, int* pos, node_type_t* type);

//--------------------------------------------------------------------------------

tree_node_t*
InputTree (const char* file_name, char** buffer)
{
    DEBUG_ASSERT (file_name != nullptr);

    int size = TreeReadDataBase (file_name, buffer);

    if (size == 0) return nullptr;

    int pos = 0;

    tree_node_t* node = MakeTree (*buffer, size, &pos);

    return node;
}

static tree_node_t*
MakeTree (const char* buffer, int size, int* pos)
{
    DEBUG_ASSERT (buffer != nullptr);

    int tmp = 0;

    tree_node_t* node  = nullptr;
    tree_node_t* left  = nullptr;
    tree_node_t* right = nullptr;

    SkipSpace (buffer, pos, &tmp);

    if (buffer[*pos] == '{') {
        *pos += 1;
        left = MakeTree (buffer, size, pos);

        if (buffer[*pos] != '}') {
            fprintf (stderr, "No close-figure bracket pos = %d c = |%c|\n", *pos, buffer[*pos]);
            return nullptr;
        }
        *pos += 1;

        SkipSpace (buffer, pos, &tmp);
    }

    node = GetNode (buffer, size, pos);

    if (node == nullptr) return nullptr;

    if (buffer[*pos] == '{') {
        *pos += 1;
        right = MakeTree (buffer, size, pos);

        if (buffer[*pos] != '}') {
            fprintf (stderr, "No close-figure bracket pos = %d c = |%c|\n", *pos, buffer[*pos]);
            return nullptr;
        }
        *pos += 1;

        SkipSpace (buffer, pos, &tmp);
    }

    node->left_node  = left;
    node->right_node = right;

    return node;
}

//--------------------------------------------------------------------------------

static tree_node_t*
GetNode (const char* buffer, int size, int* pos)
{
    DEBUG_ASSERT (buffer != nullptr);
    DEBUG_ASSERT (pos    != nullptr);

    int tmp = 0;

    SkipSpace (buffer, pos, &tmp);

    tree_node_t* node = nullptr;

    if (isdigit (buffer[*pos])) {
        node = GetNum (buffer, size, pos);
    }

    if (node == nullptr) {
        node = GetBasicWord (buffer, size, pos);
    }

    if (node == nullptr) {
        node = GetVar (buffer, size, pos);
    }
    
    if (node == nullptr) {
        fprintf (stderr, "Unknown data type |%c|\n", buffer[*pos]);
    }

    SkipSpace (buffer, pos, &tmp);

    return node;
}

//--------------------------------------------------------------------------------

// int
// MakeNodeBuffer (const char* buffer, int size)
// {
//     int pos = 0;

//     while (pos < size) {
//         if (parser_ctx.node_num == parser_ctx.capacity) {
//             if (UpsizeTokenBuffer (&parser_ctx) == language_err_t::AlocationErr) return 0;
//         }

//         language_err_t st = language_err_t::SyntaxErr;

//         if (isdigit (buffer[pos])) {
            
//             st = GetNum (&parser_ctx);

//             if (st == language_err_t::Success     ) continue;
//             if (st == language_err_t::AlocationErr) return -1;
//         }

//         if ((st = GetBasicWord (&parser_ctx)) != language_err_t::AnotherNodeType) {
//             if (st == language_err_t::Success     ) continue;
//             if (st == language_err_t::AlocationErr) return -1;    
//         }

//         if ((st = GetVar (&parser_ctx)) != language_err_t::AnotherNodeType) {
//             if (st == language_err_t::Success     ) continue;
//             if (st == language_err_t::AlocationErr) return -1;    
//         }
//     }

//     LexicalDump (*tokens, parser_ctx.node_num);

//     return parser_ctx.node_num;
// }

//--------------------------------------------------------------------------------

static tree_node_t*
GetNum (const char* buffer, int size, int* pos)
{
    DEBUG_ASSERT (buffer != nullptr);
    DEBUG_ASSERT (pos    != nullptr);

    tree_node_t* node  = nullptr;

    char* last_s = nullptr; 
    
    int value  = (int) strtol (buffer + *pos, &last_s, 10);

    if (last_s < buffer + size &&
       (isalpha (*last_s) || *last_s == '_')) {

        return nullptr;
    }

    *pos = last_s - buffer;

    node = NewNode (node_type_t::Constant, (tree_data_t) {.immediate = value}, nullptr, nullptr);
    
    if (node == nullptr) return nullptr;

    int tmp = 0;

    SkipSpace (buffer, pos, &tmp);

    return node;
}

//--------------------------------------------------------------------------------

static tree_node_t*
GetBasicWord (const char* buffer, int size, int* pos)
{
    DEBUG_ASSERT (buffer != nullptr);
    DEBUG_ASSERT (pos    != nullptr);

    node_type_t type = node_type_t::Keyword;

    int word_num = GetBasicWordNum (buffer, size, pos, &type);

    if (word_num == -1) {
        return nullptr;
    }

    tree_node_t* node = NewNode (type   , (tree_data_t) {.keyword = ((keywords_t) word_num)}, 
                                 nullptr, nullptr);

    if (node == nullptr) return nullptr;

    int tmp = 0;

    SkipSpace (buffer, pos, &tmp);

    return node;
}

//--------------------------------------------------------------------------------

static tree_node_t*
GetVar (const char* buffer, int size, int* pos)
{
    DEBUG_ASSERT (buffer != nullptr);
    DEBUG_ASSERT (pos    != nullptr);

    int tmp = 0;

    SkipSpace (buffer, pos, &tmp);

    char var_name[CommonStringSize] = "";

    int i = 0;

    while (*pos < size && (isalpha (buffer[*pos]) || isdigit (buffer[*pos]) || buffer[*pos] == '_')) {
        var_name[i++] = buffer[(*pos)++];
    }

    if (i == 0) return nullptr;

    variable_t var = {};

    var.name = const_cast <char*> (buffer) + *pos - i;
    var.len  = i;

    tree_data_t data = {};

    data.variable = var;

    tree_node_t* node = NewNode (node_type_t::Variable, data, nullptr, nullptr);

    if (node == nullptr) return nullptr;

    SkipSpace (buffer, pos, &tmp);

    return node;
}

//--------------------------------------------------------------------------------

static int
CompareBasicword (node_type_t type, const char* buffer, int size, int* pos, const char** word_arr, int word_arr_size)
{
    DEBUG_ASSERT (buffer   != nullptr);
    DEBUG_ASSERT (pos      != nullptr);
    DEBUG_ASSERT (word_arr != nullptr);

    int word_num = -1;

    int         len_word = 0;
    const char* cur_word = nullptr;

    for (int i = 0; i < word_arr_size; i++) {
        cur_word = word_arr[i];
        len_word = strlen (cur_word);

        if (strncmp (buffer + *pos, cur_word, len_word) == 0) {
            word_num = i;
            break;
        }
    }

    if ((type == node_type_t::Keyword && 
        (word_num == (int) keywords_t::if_kw     ||
         word_num == (int) keywords_t::while_kw  || 
         word_num == (int) keywords_t::else_kw)) ||
         type == node_type_t::MathFunc) {
        if (*pos + len_word < size && (isdigit (buffer[*pos + len_word]) ||
                           isalpha (buffer[*pos + len_word]) ||
                           buffer[*pos + len_word] == '_')) {
                return -1;
        }
    }

    if (type == node_type_t::MathOper && word_num == (int) math_oper_t::Assignment) {
        if (buffer[*pos + 1] == '=') {
            return (int) math_oper_t::Equality;
        }
    }

    return word_num;
}

//--------------------------------------------------------------------------------

static int
GetWordLen (node_type_t type, int word_num)
{
    if (word_num == -1) {
        return 0;
    }

    switch (type) {
    case node_type_t::Keyword :
        return strlen (kKeywords[word_num]);
    case node_type_t::MathOper :
        return strlen (kMathOperations[word_num]);
    case node_type_t::MathFunc :
        return strlen (kMathFunctions[word_num]);
    default:
        return 0;
    }
}

//--------------------------------------------------------------------------------

static int
GetBasicWordNum (const char* buffer, int size, int* pos, node_type_t* type)
{
    DEBUG_ASSERT (buffer != nullptr);
    DEBUG_ASSERT (pos    != nullptr);

    *type = node_type_t::Keyword;

    int word_num = CompareBasicword (node_type_t::Keyword, buffer, size, pos,
                                    kKeywords, kNumOfKeyWords);
                                    
    int word_len = GetWordLen (node_type_t::Keyword, word_num);

    if (word_num == -1) {
        *type     = node_type_t::MathOper;
        word_num = CompareBasicword (node_type_t::MathOper, buffer, size, pos, 
                                    kMathOperations, kNumOfMathOper);

        word_len = GetWordLen       (node_type_t::MathOper, word_num);
    }
    
    *pos += word_len;

    return word_num;
}

//--------------------------------------------------------------------------------