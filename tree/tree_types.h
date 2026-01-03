#ifndef TREE_TYPES_H
#define TREE_TYPES_H

//————————————————————————————————————————————————————————————————————————————————


//————————————————————————————————————————————————————————————————————————————————

struct math_oper_cont_t {
    const char* oper_name;
    const char* oper;
};

//————————————————————————————————————————————————————————————————————————————————

extern const char* kMathOperations[];
extern const char* kMathFunctions[] ;
extern const char* kKeywords[]      ;

//————————————————————————————————————————————————————————————————————————————————

extern const int kNumOfMathOper;
extern const int kNumOfMathFunc;
extern const int kNumOfKeyWords;

//————————————————————————————————————————————————————————————————————————————————

enum class math_oper_t {
    Addition       = 0,
    Difference     = 1,
    Multiplication = 2,
    Division       = 3,
    Exponentiation = 4,
    Assignment     = 5,
    Equality       = 6,
    NotEquality    = 7,
    Above          = 8,
    AboveEqual     = 9,
    Below          = 10,
    BelowEqual     = 11,
    And            = 12,
    Or             = 13,
};

//————————————————————————————————————————————————————————————————————————————————

enum class math_func_t {
    root      = 0,
    logarithm = 1,
    sin       = 2,
    cos       = 3,
    tg        = 4,
    ctg       = 5,
    arcsin    = 6,
    arccos    = 7,
    arctg     = 8,
    arcctg    = 9,
    sh        = 10,
    ch        = 11,
};

//————————————————————————————————————————————————————————————————————————————————

enum class keywords_t 
{
    if_kw             = 0,
    else_kw           = 1,
    while_kw          = 2,
    semicolon         = 3,
    comma             = 4,
    open_bracket      = 5,
    close_bracket     = 6,
    fig_open_bracket  = 7,
    fig_close_bracket = 8,
    announce          = 9,
};

//————————————————————————————————————————————————————————————————————————————————

#define CHECK_POS if (*pos >= node_num) {return nullptr;}

//————————————————————————————————————————————————————————————————————————————————

struct variable_t {
    char*  name;
    int    number;
};

//————————————————————————————————————————————————————————————————————————————————

typedef union {
    int         var_number;
    double      immediate;
    variable_t  var_ptr;
    math_oper_t math_oper;
    math_func_t math_func;
    keywords_t  keyword;
} tree_data_t;

//————————————————————————————————————————————————————————————————————————————————

enum class tree_err_t
{
    Success,
    OpenFileErr,
    SysCallErr,
    AllocErr,
    InvalidFileSize,
    UnknownDataType,
};

//————————————————————————————————————————————————————————————————————————————————

enum class node_type_t 
{
    Unknown    = -1,
    Keyword    =  0,
    MathOper   =  1,
    MathFunc   =  2,
    CustomFunc =  3,
    Constant   =  4,
    Variable   =  5,
    Connection =  6,
};

//————————————————————————————————————————————————————————————————————————————————

struct tree_node_t {
    int            idx;
    node_type_t    type;
    tree_data_t    node_data;
    tree_node_t*   prev_node;
    tree_node_t*   left_node;
    tree_node_t*   right_node;
};

//————————————————————————————————————————————————————————————————————————————————

#endif //TREE_TYPES_H