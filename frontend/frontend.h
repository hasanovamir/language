#ifndef CALCULATOR_H
#define CALCULATOR_H

//————————————————————————————————————————————————————————————————————————————————

#include "frontend_types.h"
#include "common_dump.h"
#include "stack.h"

//————————————————————————————————————————————————————————————————————————————————

const int StartVarCap = 10;

//————————————————————————————POSHEL_NAHUI————————————————————————————————————————————————————

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
tree_node_t* MakeTree                   (const token_t* tokens, int node_num);
frontend_err_t
InitParserCtx (parser_ctx_t* parser, const char* buffer, int size);
frontend_err_t
UpsizeTokenBuffer (parser_ctx_t* parser);
frontend_err_t
SaveASTtoFile (const char* file_name, const tree_node_t* head);

//————————————————————————————————————————————————————————————————————————————————

#endif //CALCULATOR_H