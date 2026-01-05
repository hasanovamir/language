#include "lang.h"

//--------------------------------------------------------------------------------

static language_err_t VarArrCtor  (variable_ctx** var_ctx);
static void           VarArrDtor  (variable_ctx*  var_ctx);
static bool           CheckForVar (variable_t var);

static tree_node_t* GetP          (const token_t* tokens, int* pos, int node_num);
static tree_node_t* GetMul        (const token_t* tokens, int* pos, int node_num);
static tree_node_t* GetAdd        (const token_t* tokens, int* pos, int node_num);
static tree_node_t* GetN          (const token_t* tokens, int* pos, int node_num);
static tree_node_t* GetG          (const token_t* tokens, int* pos, int node_num);
static tree_node_t* GetV          (const token_t* tokens, int* pos, int node_num);
static tree_node_t* GetExp        (const token_t* tokens, int* pos, int node_num);
static tree_node_t* GetEqual      (const token_t* tokens, int* pos, int node_num);
static tree_node_t* GetAss        (const token_t* tokens, int* pos, int node_num);
static tree_node_t* GetAnd        (const token_t* tokens, int* pos, int node_num);
static tree_node_t* GetFigBracket (const token_t* tokens, int* pos, int node_num);
static tree_node_t* GetIfWhile    (const token_t* tokens, int* pos, int node_num);
static tree_node_t* GetElse       (const token_t* tokens, int* pos, int node_num);
static tree_node_t* GetStatement  (const token_t* tokens, int* pos, int node_num);
static tree_node_t* GetAnnounce   (const token_t* tokens, int* pos, int node_num);

//--------------------------------------------------------------------------------

static stack_t stk = {};

//--------------------------------------------------------------------------------

tree_node_t*
ReadTree (const token_t* tokens, int node_num)
{
    DEBUG_ASSERT (tokens != nullptr);

    stack_init (&stk, 10);

    int pos = 0;

    return GetG (tokens, &pos, node_num);
}

//--------------------------------------------------------------------------------

static tree_node_t*
GetP (const token_t* tokens, int* pos, int node_num)
{
    DEBUG_ASSERT (tokens != nullptr);

    CHECK_POS;

    tree_node_t* cur_node = tokens[*pos].node;

    node_type_t  type    = cur_node->type;
    keywords_t   keyword = cur_node->node_data.keyword;

    if (type    == node_type_t::Keyword && 
        keyword == keywords_t ::open_bracket) {

        *pos += 1;

        if (*pos >= node_num) {
            DebugPrint ("Nothing after bracket", tokens[*pos - 1].line);
            return nullptr;
        }

        tree_node_t* node = GetAnd (tokens, pos, node_num);

        if (*pos >= node_num) {
            DebugPrint ("No close bracket", tokens[*pos - 1].line);
            return nullptr;
        }

        cur_node = tokens[*pos].node;
        type     = cur_node->type;
        keyword  = cur_node->node_data.keyword;

        if (type != node_type_t::Keyword || keyword != keywords_t::close_bracket) {
            DebugPrint ("No close bracket", tokens[*pos - 1].line);
            
            return nullptr;
        }

        *pos += 1;
    
        return node;
    }
    else if (type == node_type_t::Constant) {
        return GetN (tokens, pos, node_num);
    }
    else if (type == node_type_t::Variable) {
        return GetV (tokens, pos, node_num);
    }

    fprintf (stderr, "TI EBLAN (GetP)\n");

    return nullptr;
}

//--------------------------------------------------------------------------------

static tree_node_t*
GetMul (const token_t* tokens, int* pos, int node_num)
{
    DEBUG_ASSERT (tokens != nullptr);

    CHECK_POS;

    tree_node_t* node_1  = GetExp (tokens, pos, node_num);

    if (node_1 == nullptr) {
        return nullptr;
    }

    if (*pos >= node_num - 1) {
        return node_1;
    }

    tree_node_t* cur_node = tokens[*pos].node;
    math_oper_t  m_oper   = cur_node->node_data.math_oper;

    while (cur_node->type == node_type_t::MathOper && *pos < node_num &&
          (m_oper == math_oper_t::Multiplication || m_oper == math_oper_t::Division)) {

        (*pos)++;

        if (*pos > node_num - 1) {
            DebugPrint ("too few arguments to math operator1", tokens[*pos - 1].line);
            return nullptr;
        }
        
        tree_node_t* node_2 = GetExp (tokens, pos, node_num);

        if (node_2 == nullptr) {
            DebugPrint ("too few arguments to math operator2", tokens[*pos].line);
            return nullptr;
        }

        cur_node->left_node  = node_1;
        cur_node->right_node = node_2;

        node_1 = cur_node;

        if (*pos < node_num) {
            cur_node = tokens[*pos].node;
            m_oper   = cur_node->node_data.math_oper;
        }
        else {
            return node_1;
        }
    }

    return node_1;
}

//--------------------------------------------------------------------------------

static tree_node_t*
GetAdd (const token_t* tokens, int* pos, int node_num)
{
    DEBUG_ASSERT (tokens != nullptr);

    CHECK_POS;

    tree_node_t* node_1 = GetMul (tokens, pos, node_num);

    if (node_1 == nullptr) {
        return nullptr;
    }

    if (*pos >= node_num - 1) {
        return node_1;
    }

    tree_node_t* cur_node = tokens[*pos].node;

    math_oper_t  m_oper   = cur_node->node_data.math_oper;

    while (cur_node->type == node_type_t::MathOper && *pos < node_num &&
          (m_oper == math_oper_t::Addition || m_oper == math_oper_t::Difference)) {

        (*pos)++;

        if (*pos > node_num - 1) {
            DebugPrint ("too few arguments to math operator3", tokens[*pos - 1].line);
            return nullptr;
        }

        tree_node_t* node_2 = GetMul (tokens, pos, node_num);

        if (node_2 == nullptr) {
            DebugPrint ("too few arguments to math operator4", tokens[*pos].line);
            return nullptr;
        }

        cur_node->left_node  = node_1;
        cur_node->right_node = node_2;

        node_1 = cur_node;

        if (*pos < node_num) {
            cur_node = tokens[*pos].node;
            m_oper   = cur_node->node_data.math_oper;
        }
        else {
            return node_1;
        }
    }

    return node_1;
}

//--------------------------------------------------------------------------------

static tree_node_t*
GetN (const token_t* tokens, int* pos, int node_num)
{
    DEBUG_ASSERT (tokens != nullptr);

    CHECK_POS;

    tree_node_t* cur_node = tokens[*pos].node;

    if (cur_node->type != node_type_t::Constant) {
        return nullptr;
    }

    *pos += 1;

    return cur_node;
}

//--------------------------------------------------------------------------------

static tree_node_t*
GetG (const token_t* tokens, int* pos, int node_num)
{
    tree_node_t* node = GetStatement (tokens, pos, node_num);

    if (node == nullptr) fprintf (stderr, "nullptr in getG\n");
    
    return node;
}

//--------------------------------------------------------------------------------

static tree_node_t*
GetV (const token_t* tokens, int* pos, int node_num)
{
    DEBUG_ASSERT (tokens != nullptr);

    CHECK_POS;

    tree_node_t* cur_node  = tokens[*pos].node;

    if (cur_node->type != node_type_t::Variable) {
        return nullptr;
    }

    if (CheckForVar (cur_node->node_data.variable) == false) {
        DebugPrint ("Unkown variable", tokens[*pos].line);
        return nullptr;
    }

    *pos += 1;

    return cur_node;
}

//--------------------------------------------------------------------------------

static tree_node_t*
GetExp (const token_t* tokens, int* pos, int node_num)
{
    DEBUG_ASSERT (tokens != nullptr);

    tree_node_t* node_1 = GetP (tokens, pos, node_num);

    if (*pos == node_num - 1) {
        return node_1;
    }

    if (node_1 == nullptr) {
        return nullptr;
    }

    tree_node_t* cur_node = tokens[*pos].node;

    while (cur_node->type == node_type_t::MathOper && cur_node->node_data.math_oper == math_oper_t::Exponentiation) {
        (*pos)++;

        if (*pos > node_num - 1) {
            DebugPrint ("too few arguments to math operator5", tokens[*pos].line);
            return nullptr;
        }

        tree_node_t* node_2 = GetP (tokens, pos, node_num);

        if (node_2 == nullptr) {
            DebugPrint ("too few arguments to math operator6", tokens[*pos].line);
            return nullptr;
        }

        cur_node->left_node  = node_1;
        cur_node->right_node = node_2;

        node_1 = cur_node;

        if (*pos < node_num - 1) {
            cur_node = tokens[*pos].node;
        }
        else {
            break;
        }
    }

    return node_1;
}

//--------------------------------------------------------------------------------

static tree_node_t*
GetEqual (const token_t* tokens, int* pos, int node_num)
{
    DEBUG_ASSERT (tokens != nullptr);

    CHECK_POS;

    tree_node_t* node_1 = GetAdd (tokens, pos, node_num);

    if (node_1 == nullptr) {
        return nullptr;
    }

    if (*pos >= node_num - 1) {
        return node_1;
    }

    int          cur_line = tokens[*pos].line;
    tree_node_t* cur_node = tokens[*pos].node;

    math_oper_t m_oper = cur_node->node_data.math_oper;

    if ((cur_node->type == node_type_t::MathOper) &&
        (m_oper == math_oper_t::Equality || m_oper == math_oper_t::NotEquality ||
         m_oper == math_oper_t::Above    || m_oper == math_oper_t::AboveEqual  ||
         m_oper == math_oper_t::Below    || m_oper == math_oper_t::BelowEqual)) {

        *pos += 1;

        tree_node_t* node_2 = GetAdd (tokens, pos, node_num);

        if (node_2 == nullptr) {
            DebugPrint ("too few arguments to math operator7", cur_line);
            return nullptr;
        }

        cur_node->left_node  = node_1;
        cur_node->right_node = node_2;

        return cur_node;
    }
    else {
        return node_1;
    }
}

//--------------------------------------------------------------------------------

static tree_node_t* GetAss (const token_t* tokens, int* pos, int node_num)
{
    DEBUG_ASSERT (tokens != nullptr);

    CHECK_POS;

    if (tokens[*pos].node->type != node_type_t::Variable) {
        return nullptr;
    }

    tree_node_t* node_1 = GetV (tokens, pos, node_num);

    if (node_1 == nullptr) {
        return nullptr;
    }

    if (*pos >= node_num) {
        return node_1;
    }

    int          cur_line = tokens[*pos].line;
    tree_node_t* cur_node = tokens[*pos].node;

    if (cur_node->type                == node_type_t::MathOper &&
        cur_node->node_data.math_oper == math_oper_t::Assignment) {

        *pos += 1;

        tree_node_t* node_2 = GetAnd (tokens, pos, node_num);

        if (node_2 == nullptr) {
            DebugPrint ("too few arguments to math operator8", cur_line);
            return nullptr;
        }

        cur_node->left_node  = node_1;
        cur_node->right_node = node_2;

        tree_node_t* node_3 = tokens[*pos].node;

        if (node_3->type              != node_type_t::Keyword || 
            node_3->node_data.keyword != keywords_t::semicolon ) {
            DebugPrint ("forgotten semicolon", tokens[*pos].line);
            return nullptr;
        }

        *pos += 1;

        node_3->left_node = cur_node;

        return node_3;
    }

    return node_1;
}

//--------------------------------------------------------------------------------

static tree_node_t*
GetAnd (const token_t* tokens, int* pos, int node_num)
{
    DEBUG_ASSERT (tokens != nullptr);

    CHECK_POS;

    tree_node_t* node_1 = GetEqual (tokens, pos, node_num);

    if (node_1 == nullptr) {
        return nullptr;
    }

    if (*pos >= node_num) {
        return node_1;
    }

    int          cur_line = tokens[*pos].line;
    tree_node_t* cur_node = tokens[*pos].node;

    math_oper_t m_oper = cur_node->node_data.math_oper;

    while ((cur_node->type == node_type_t::MathOper) &&
        (m_oper == math_oper_t::And || m_oper == math_oper_t::Or)) {

        *pos += 1;

        tree_node_t* node_2 = GetEqual (tokens, pos, node_num);

        if (node_2 == nullptr) {
            DebugPrint ("too few arguments to math operator9", cur_line);
            return nullptr;
        }

        cur_node->left_node  = node_1;
        cur_node->right_node = node_2;

        node_1 = cur_node;

        
        if (*pos < node_num) {
            cur_node = tokens[*pos].node;
            cur_line = tokens[*pos].line;
            m_oper   = cur_node->node_data.math_oper;
        }
        else {
            return node_1;
        }
    }

    return node_1;
}

//--------------------------------------------------------------------------------

static tree_node_t*
GetIfWhile (const token_t* tokens, int* pos, int node_num)
{
    DEBUG_ASSERT (tokens != nullptr);

    CHECK_POS;

    int          cur_line = tokens[*pos].line;
    tree_node_t* cur_node = tokens[*pos].node;

    if (cur_node->type == node_type_t::Keyword &&
            (cur_node->node_data.keyword == keywords_t::if_kw ||
             cur_node->node_data.keyword == keywords_t::while_kw)) {
        *pos += 1;

        tree_node_t* node_1 = GetP (tokens, pos, node_num);

        if (node_1 == nullptr || *pos >= node_num) {
            DebugPrint ("too few arguments to if-operator1", cur_line);
            return nullptr;
        }

        tree_node_t* node_2 = GetFigBracket (tokens, pos, node_num);

        if (node_2 == nullptr) {
            DebugPrint ("too few arguments to if-operator2", tokens[*pos].line);
            return nullptr;
        }
        
        cur_node->left_node  = node_1;
        cur_node->right_node = node_2;

        tree_node_t* node_3 = GetElse (tokens, pos, node_num);

        if (node_3 != nullptr) {
            node_3  ->left_node  = node_2;
            cur_node->right_node = node_3;
        }

        return cur_node;
    }

    return nullptr;
}

//--------------------------------------------------------------------------------

static tree_node_t*
GetFigBracket (const token_t* tokens, int* pos, int node_num)
{
    DEBUG_ASSERT (tokens != nullptr);

    CHECK_POS;

    int          cur_line = tokens[*pos].line;
    tree_node_t* cur_node = tokens[*pos].node;

    if (cur_node->type              == node_type_t::Keyword &&
        cur_node->node_data.keyword == keywords_t ::fig_open_bracket) {
        *pos += 1;

        MyFree (cur_node);

        variable_ctx* v_ctx = nullptr;

        if (VarArrCtor (&v_ctx) == language_err_t::AlocationErr) {
            return nullptr;
        }

        stack_push (&stk, (void*) v_ctx);

        tree_node_t* node_1 = GetStatement (tokens ,pos, node_num);
        
        if (node_1 == nullptr) {
            return nullptr;
        }

        if (*pos >= node_num) {
            DebugPrint ("No close figure bracket", tokens[*pos].line);
            return nullptr;
        }

        tree_node_t* node_2 = tokens[*pos].node;

        if (node_2->type              != node_type_t::Keyword || 
            node_2->node_data.keyword != keywords_t ::fig_close_bracket) {
            DebugPrint ("No close figure bracket", tokens[*pos].line);
            return nullptr;
        }

        *pos += 1;

        MyFree (node_2);

        void* tmp = nullptr;

        stack_pop (&stk, &tmp);

        VarArrDtor ((variable_ctx*) tmp);

        return cur_node;
    }

    return nullptr;
}

//--------------------------------------------------------------------------------

static tree_node_t*
GetElse (const token_t* tokens, int* pos, int node_num)
{
    DEBUG_ASSERT (tokens != nullptr);

    CHECK_POS;

    int          cur_line = tokens[*pos].line; 
    tree_node_t* cur_node = tokens[*pos].node;

    if (cur_node->type              == node_type_t::Keyword &&
        cur_node->node_data.keyword == keywords_t ::else_kw   ) {
        *pos += 1;

        tree_node_t* node_1 = GetFigBracket (tokens, pos, node_num);

        if (node_1 == nullptr) {
            DebugPrint ("too few arguments to else-operator", tokens[*pos].line);
            return nullptr;
        }

        cur_node->right_node = node_1;

        return cur_node;
    }

    return nullptr;
}

//--------------------------------------------------------------------------------

static tree_node_t*
GetStatement (const token_t* tokens, int* pos, int node_num)
{
    DEBUG_ASSERT (tokens != nullptr);

    CHECK_POS;

    tree_node_t* head = NewNode (node_type_t::Connection, MakeDigitData (0), 
                                 nullptr                , nullptr         );

    tree_node_t* cur_node = head;

    while (true) {
        if ((cur_node->right_node = GetAnnounce (tokens, pos, node_num))) {
            cur_node = cur_node->right_node;
        }
        else if (*pos < node_num && tokens[*pos].node->type == node_type_t::Variable) {
            cur_node->right_node = GetAss (tokens, pos, node_num);
            if (cur_node->right_node == nullptr) {
                return nullptr;
            }
            cur_node = cur_node->right_node;
        }
        else if ((cur_node->right_node = GetIfWhile (tokens, pos, node_num))) {
            tree_node_t* con = NewNode (node_type_t::Connection, MakeDigitData (0), 
                                        nullptr                , nullptr         );
            con->left_node = cur_node->right_node;

            cur_node->right_node = con;

            cur_node = con;
        }
        else {
            return head;
        }
    }

    return head;
}

//--------------------------------------------------------------------------------

static tree_node_t*
GetAnnounce (const token_t* tokens, int* pos, int node_num)
{
    DEBUG_ASSERT (tokens != nullptr);

    CHECK_POS;
 
    int          cur_line = tokens[*pos].line;
    tree_node_t* cur_node = tokens[*pos].node;

    if (cur_node->type              == node_type_t::Keyword &&
        cur_node->node_data.keyword == keywords_t ::announce ) {
        *pos += 1;

        if (*pos >= node_num || tokens[*pos].node->type != node_type_t::Variable) {
            DebugPrint ("Too few arguments to announce", cur_line);
            return nullptr;
        }

        void* cur_vbl = nullptr;

        stack_pop (&stk, &cur_vbl);

        variable_ctx* cur_variables = (variable_ctx*) cur_vbl;
        
        AddVar (cur_variables, tokens[*pos].node->node_data.variable);

        stack_push (&stk, cur_variables);

        cur_node->left_node = GetAss (tokens, pos, node_num);

        tree_node_t* left = cur_node->left_node;

        if (left->             type != node_type_t::Keyword || 
            left->node_data.keyword != keywords_t ::semicolon) {
            DebugPrint ("Initialization is required when declaring a variable", cur_line);
            return nullptr;
        }

        return cur_node;
    }

    return nullptr;
}

//--------------------------------------------------------------------------------

static bool
CheckForVar (variable_t var)
{
    int var_len = var.len;

    void* v_arr = nullptr;

    stack_pop (&stk, &v_arr);

    if (v_arr == nullptr) {
        return false;
    }

    variable_ctx* var_ctx = (variable_ctx*) v_arr;

    for (int i = 0; i < var_ctx->count; i++) {
        int         cur_len = var_ctx->variable_arr[i].len ;
        const char* cur_var = var_ctx->variable_arr[i].name;

        cur_len = (cur_len > var_len) ? var_len : cur_len;

        if (strncmp (var.name, cur_var, cur_len) == 0) {
            stack_push (&stk, v_arr);
            return true;
        }
    }

    if (CheckForVar (var)) {
        stack_push (&stk, v_arr);
        return true;
    }

    stack_push (&stk, v_arr);

    return false;
}

//--------------------------------------------------------------------------------

static language_err_t
VarArrCtor (variable_ctx** var_ctx)
{
    *var_ctx = (variable_ctx*) calloc (1, sizeof (variable_ctx));

    if (*var_ctx == nullptr) {
        PRINTERR (language_err_t::AlocationErr);
        return language_err_t::AlocationErr;
    }

    variable_t* var_arr = (variable_t*) calloc (CommonStackSize, sizeof (variable_t));

    if (var_arr == nullptr) {
        PRINTERR (language_err_t::AlocationErr);
        return language_err_t::AlocationErr;
    }

    (*var_ctx)->capacity = CommonStackSize;

    (*var_ctx)->variable_arr = var_arr;

    return language_err_t::Success;
}

//--------------------------------------------------------------------------------

static void
VarArrDtor (variable_ctx* var_ctx)
{
    DEBUG_ASSERT (var_ctx != nullptr);

    free (var_ctx->variable_arr);
    free (var_ctx);

    return ;
}

//--------------------------------------------------------------------------------