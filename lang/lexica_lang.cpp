#include "lang.h"

//--------------------------------------------------------------------------------

const char* kKeywords[] = 
{
    "if"      ,
    "else"    ,
    "while"   ,
    ";"       ,
    ","       ,
    "("       ,
    ")"       ,
    "{"       ,
    "}"       ,
    "announce",
    "def"     ,
};

const int kNumOfKeywords = sizeof (kKeywords) / sizeof (*kKeywords);

//--------------------------------------------------------------------------------

static language_err_t GetNum            (parser_ctx_t* parser_ctx);
static language_err_t GetVar            (parser_ctx_t* parser_ctx);
static language_err_t GetBasicWord      (parser_ctx_t* parser_ctx);
static language_err_t UpsizeTokenBuffer (token_t**     tokens  , int* cur_tokens_cap);
static int            CompareBasicword  (node_type_t   type    , parser_ctx_t* parser_ctx,
                                         const char**  word_arr, int word_arr_size  );
static int            GetWordLen        (node_type_t   type    , int word_num       );
static void           LexicalDump       (token_t*      tokens  , int count          );
static int            GetBasicWordNum   (parser_ctx_t* parser_ctx, node_type_t* type);

//--------------------------------------------------------------------------------

int
MakeNodeBuffer (const char* buffer, int size, token_t** tokens)
{
    *tokens = (token_t*) calloc (100, sizeof (token_t));

    parser_ctx_t parser_ctx = {};

    parser_ctx.size   =  size;
    parser_ctx.buffer =  buffer;
    parser_ctx.tokens = *tokens;
    
    while (parser_ctx.pos < size) {
        int pos = parser_ctx.pos;

        language_err_t st = language_err_t::SyntaxErr;

        if (isdigit (buffer[pos])) {
            
            st = GetNum (&parser_ctx);

            if (st == language_err_t::Success     ) continue;
            if (st == language_err_t::AlocationErr) return -1;
        }

        if ((st = GetBasicWord (&parser_ctx)) != language_err_t::AnotherNodeType) {
            if (st == language_err_t::Success     ) continue;
            if (st == language_err_t::AlocationErr) return -1;    
        }

        if ((st = GetVar (&parser_ctx)) != language_err_t::AnotherNodeType) {
            if (st == language_err_t::Success     ) continue;
            if (st == language_err_t::AlocationErr) return -1;    
        }
    }

    LexicalDump (*tokens, parser_ctx.node_num);

    return parser_ctx.node_num;
}

//--------------------------------------------------------------------------------

static language_err_t
GetNum (parser_ctx_t* parser_ctx)
{
    DEBUG_ASSERT (parser_ctx != nullptr);

    int  pos      = parser_ctx->pos;
    int  cur_line = parser_ctx->cur_line;
    int  node_num = parser_ctx->node_num;

    char*       last_s = nullptr;
    const char* buffer = parser_ctx->buffer;

    tree_node_t* node   = nullptr;
    token_t*     tokens = parser_ctx->tokens;
    
    double value  = strtod (buffer + pos, &last_s);

    if (last_s < buffer + parser_ctx->size &&
       (isalpha (*last_s) || *last_s == '_')) {

        return language_err_t::AnotherNodeType;
    }

    parser_ctx->pos = last_s - buffer;

    node = NewNode (node_type_t::Constant, MakeDigitData (value), nullptr, nullptr);
    
    if (node == nullptr) {
        return language_err_t::AlocationErr;
    }
    
    tokens[node_num  ].line = cur_line;
    tokens[node_num++].node = node;

    pos = parser_ctx->pos;

    SkipSpace (buffer, &pos, &cur_line);

    parser_ctx->pos      = pos;
    parser_ctx->cur_line = cur_line;
    parser_ctx->node_num = node_num;

    return language_err_t::Success;
}

//--------------------------------------------------------------------------------

static language_err_t
GetBasicWord (parser_ctx_t* parser_ctx)
{
    DEBUG_ASSERT (parser_ctx != nullptr);

    int cur_line = parser_ctx->cur_line;
    int node_num = parser_ctx->node_num;

    const char* buffer = parser_ctx->buffer;

    token_t* tokens = parser_ctx->tokens;

    node_type_t type = node_type_t::Keyword;

    int word_num = GetBasicWordNum (parser_ctx, &type);

    if (word_num == -1) {
        return language_err_t::AnotherNodeType;
    }

    tree_node_t* node = NewNode (type   , MakeKeywordData ((keywords_t) word_num), 
                                 nullptr, nullptr);

    if (node == nullptr) {
        return language_err_t::AlocationErr;
    }

    tokens[node_num  ].node = node;
    tokens[node_num++].line = cur_line;

    SkipSpace (buffer, &(parser_ctx->pos), &cur_line);

    parser_ctx->cur_line = cur_line;
    parser_ctx->node_num = node_num;

    return language_err_t::Success;
}

//--------------------------------------------------------------------------------

static language_err_t
GetVar (parser_ctx_t* parser_ctx)
{
    DEBUG_ASSERT (parser_ctx != nullptr);

    int pos      = parser_ctx->pos;
    int size     = parser_ctx->size;
    int cur_line = parser_ctx->cur_line;
    int node_num = parser_ctx->node_num;

    char var_name[CommonStringSize] = "";

    const char* buffer = parser_ctx->buffer;

    token_t* tokens = parser_ctx->tokens;

    int i = 0;

    while (pos < size && (isalpha (buffer[pos]) || isdigit (buffer[pos]) || buffer[pos] == '_')) {
        var_name[i++] = buffer[pos++];
    }

    if (i == 0) {
        return language_err_t::SyntaxErr;
    }

    variable_t var = {};

    var.name   = const_cast <char*> (buffer) + pos - i;
    var.len = i;

    tree_data_t data = {};

    data.variable = var;

    tree_node_t* node = NewNode (node_type_t::Variable, data, nullptr, nullptr);

    if (node == nullptr) {
        return language_err_t::AlocationErr;
    }

    tokens[node_num  ].node = node;
    tokens[node_num++].line = cur_line;

    SkipSpace (buffer, &pos, &cur_line);

    parser_ctx->pos      = pos;
    parser_ctx->cur_line = cur_line;
    parser_ctx->node_num = node_num;

    return language_err_t::Success;
}

//--------------------------------------------------------------------------------

static void
LexicalDump (token_t* tokens, int count)
{
    DEBUG_ASSERT (tokens != nullptr);

    FILE* dump = fopen ("dump/lexical.txt", "w");

    if (dump == nullptr) {
        PRINTERR (language_err_t::FopenErr);
        return ;
    }

    for (int i = 0; i < count; i++) {
        tree_node_t* cur_node = tokens[i].node;
        int          cur_line = tokens[i].line;
        fprintf (dump, "(#%d | line = %d | type = ", i, cur_line);
        switch (cur_node->type) {
            case node_type_t::Keyword :
                fprintf (dump, "  keyword   | data = \"%s\")\n", kKeywords[(int)(cur_node->node_data.keyword)]);
                break;
            case node_type_t::MathOper :
                fprintf (dump, " math  oper | data = \'%s\')\n", kMathOperations[(int)(cur_node->node_data.math_oper)]);
                break;
            case node_type_t::MathFunc :
                fprintf (dump, " math  func | data = %s)\n", kMathFunctions[(int)(cur_node->node_data.math_func)]);
                break;
            case node_type_t::CustomFunc :
                fprintf (dump, "custom func | data = %s)\n", 
                    cur_node->node_data.variable.name);
            case node_type_t::Constant :
                fprintf (dump, "  Constant  | data = %lg)\n", cur_node->node_data.immediate);
                break;
            case node_type_t::Variable :
                fprintf (dump, "  variable  | data = \"%.*s\")\n", 
                    cur_node->node_data.variable.len, cur_node->node_data.variable.name);
                break;
            default:
                break;
        }
    }

    fclose (dump);
}

//--------------------------------------------------------------------------------

static language_err_t
UpsizeTokenBuffer (token_t** tokens, int* cur_tokens_cap)
{
    DEBUG_ASSERT (tokens != nullptr);

    token_t* new_arr = (token_t*) realloc (*tokens, 
                        *cur_tokens_cap * sizeof (token_t) * 2);

    if (new_arr == nullptr) {
        PRINTERR (language_err_t::AlocationErr);
        return language_err_t::AlocationErr;
    }

    *tokens = new_arr;

    *cur_tokens_cap *= 2;

    return language_err_t::Success;
}

//--------------------------------------------------------------------------------

static int
CompareBasicword (node_type_t type, parser_ctx_t* parser_ctx, const char** word_arr, int word_arr_size)
{
    DEBUG_ASSERT (parser_ctx != nullptr);
    DEBUG_ASSERT (word_arr   != nullptr);

    const char* buffer = parser_ctx->buffer;
    int         size   = parser_ctx->size;
    int         pos    = parser_ctx->pos;

    int word_num = -1;

    int         len_word = 0;
    const char* cur_word = nullptr;

    for (int i = 0; i < word_arr_size; i++) {
        cur_word = word_arr[i];
        len_word = strlen (cur_word);

        if (strncmp (buffer + pos, cur_word, len_word) == 0) {
            word_num = i;
            break;
        }
    }

    if ((type == node_type_t::Keyword && 
            (word_num == (int) keywords_t::if_kw     ||
             word_num == (int) keywords_t::while_kw  || 
             word_num == (int) keywords_t::else_kw)) ||
         type == node_type_t::MathFunc) {
        if (pos + len_word < size && (isdigit (buffer[pos + len_word]) ||
                           isalpha (buffer[pos + len_word]) ||
                           buffer[pos + len_word] == '_')) {
                return -1;
        }
    }

    if (type == node_type_t::MathOper && word_num == (int) math_oper_t::Assignment) {
        if (buffer[pos + 1] == '=') {
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
GetBasicWordNum (parser_ctx_t* parser_ctx, node_type_t* type)
{
    DEBUG_ASSERT (parser_ctx != nullptr);

    *type = node_type_t::Keyword;

    int word_num = CompareBasicword (node_type_t::Keyword, parser_ctx, 
                                    kKeywords, kNumOfKeywords);

    int word_len = GetWordLen       (node_type_t::Keyword, word_num);
    if (word_num == -1) {
        *type     = node_type_t::MathOper;
        word_num = CompareBasicword (node_type_t::MathOper, parser_ctx, 
                                    kMathOperations, kNumOfMathOper);

        word_len = GetWordLen       (node_type_t::MathOper, word_num);
    }

    if (word_num == -1) {
        *type     = node_type_t::MathFunc;
        word_num = CompareBasicword (node_type_t::MathFunc, parser_ctx, 
                                     kMathFunctions, kNumOfMathFunc);

        word_len = GetWordLen       (node_type_t::MathFunc, word_num);
    }
    
    parser_ctx->pos += word_len;

    return word_num;
}

//--------------------------------------------------------------------------------