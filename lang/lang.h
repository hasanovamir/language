#ifndef CALCULATOR_H
#define CALCULATOR_H

//————————————————————————————————————————————————————————————————————————————————

#include "lang_types.h"
#include "common_dump.h"
#include "stack.h"

//————————————————————————————————————————————————————————————————————————————————

const int StartVarCap = 10;

//————————————————————————————POSHEL_NAHUI————————————————————————————————————————————————————
tree_node_t* 
NewNode (node_type_t type, tree_data_t data, tree_node_t* left, tree_node_t* right);

void         ClearDump                  (void);
void         OptimizeTree               (tree_node_t* node);
void         OptimizeTreeIfBothConst    (tree_node_t* node);
void         OptimizeTreeIfRightConst   (tree_node_t* node);
void         OptimizeTreeIfLeftConst    (tree_node_t* node);
void         OptimizeFunctionOfConstant (tree_node_t* node);
void         RainbowPrint               (const char* buffer);
void         VariablesDtor              (variable_ctx* var_ctx);
void         CopyNodeData               (tree_node_t* destination_node, 
                                         tree_node_t* source_node    );
void         DebugPrint                 (const char*  str ,int line);
int          AddVar                     (variable_ctx* var_ctx, variable_t var, 
                                         variable_type_t type, int params_count);
int          CountPrevStrings           (const char* buffer, int pos);
int          MakeNodeBuffer             (const char* buffer, int size, token_t** tokens);
bool         CheckToEquality            (double value_1, double value_2);
tree_err_t   VariablesInit              (variable_ctx* variables_ctx);
tree_data_t  MakeDigitData              (double      value    );
tree_data_t  MakeKeywordData            (keywords_t  oper     );
tree_data_t  MakeMathOperData           (math_oper_t oper     );
tree_err_t   LangInit                   (const char* file_name);
tree_data_t  MakeMathFuncData           (math_func_t foo);
tree_node_t* ReadTree                   (const token_t* tokens, int node_num);

//——————————————————————————macros to Differentiate——————————————————————————————————————————————————————

#define d_(node, argument)  DifferentiateNode (node, argument)
#define c_(node)            CopyTree          (node)

#define ADD_(left, right)   NewNode (oper_t, MakeOperData (addition_op),       left, right)
#define DIF_(left, right)   NewNode (oper_t, MakeOperData (difference_op),     left, right)
#define MUL_(left, right)   NewNode (oper_t, MakeOperData (multiplication_op), left, right)
#define DIV_(left, right)   NewNode (oper_t, MakeOperData (division_op),       left, right)
#define EXP_(left, right)   NewNode (oper_t, MakeOperData (exponentiation_op), left, right)
#define ROOT_(left, right)  NewNode (oper_t, MakeOperData (root_op),           left, right)
#define LOG_(left, right)   NewNode (oper_t, MakeOperData (logarithm_op),      left, right)
#define SIN_(node)          NewNode (oper_t, MakeOperData (sin_op),    node, nullptr)
#define COS_(node)          NewNode (oper_t, MakeOperData (cos_op),    node, nullptr)
#define TG_(node)           NewNode (oper_t, MakeOperData (tg_op),     node, nullptr)
#define CTG_(node)          NewNode (oper_t, MakeOperData (ctg_op),    node, nullptr)
#define ARCSIN_(node)       NewNode (oper_t, MakeOperData (arcsin_op), node, nullptr)
#define ARCCOS_(node)       NewNode (oper_t, MakeOperData (arccos_op), node, nullptr)
#define ARCTG_(node)        NewNode (oper_t, MakeOperData (arctg_op),  node, nullptr)
#define ARCCTG_(node)       NewNode (oper_t, MakeOperData (arcctg_op), node, nullptr)
#define SH_(node)           NewNode (oper_t, MakeOperData (sh_op),     node, nullptr)
#define CH_(node)           NewNode (oper_t, MakeOperData (ch_op),     node, nullptr)

#define NEW_DATA_NODE(data)    NewNode (const_t, MakeDigitData (data), nullptr, nullptr)
#define NEW_VAR_NODE(var)      NewNode (var_t , MakeVarData   (var ), nullptr, nullptr)

//————————————————————————————————————————————————————————————————————————————————

#endif //CALCULATOR_H