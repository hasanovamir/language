#include "lang.h"

//--------------------------------------------------------------------------------

tree_data_t
MakeMathOperData (math_oper_t oper)
{
    tree_data_t data;
    
    data.math_oper = oper;
    
    return data;
}

//--------------------------------------------------------------------------------

tree_data_t
MakeMathFuncData (math_func_t foo)
{
    tree_data_t data;
    
    data.math_func = foo;
    
    return data;
}

//--------------------------------------------------------------------------------

tree_data_t
MakeKeywordData (keywords_t oper)
{
    tree_data_t data;
    
    data.keyword = oper;
    
    return data;
}

//--------------------------------------------------------------------------------

tree_data_t
MakeDigitData (double value)
{
    tree_data_t data;

    data.immediate = value;

    return data;
}

//--------------------------------------------------------------------------------

tree_data_t
MakeVarData (int oper_num)
{
    tree_data_t data;
    
    data.var_number = oper_num;
    
    return data;
}

//--------------------------------------------------------------------------------

int 
CheckForVariables (tree_node_t* node)
{
    if (node->node_data.var_number != 0) {
        return 1;
    }

    if (node->left_node) {
        if (CheckForVariables (node->left_node)) {
            return 1;
        }
    }

    if (node->right_node) {
        if (CheckForVariables (node->right_node)) {
            return 1;
        }
    }

    return 0;
}

//--------------------------------------------------------------------------------

int
GetVarNumber (variable_ctx* variables_ctx, const char* name)
{
    for (int i = 0; i < variables_ctx->count; i++) {
        if (strcmp (name, variables_ctx->variable_arr[i].name) == 0) {
            return i;
        }
    }
  
    return -1;
}

//--------------------------------------------------------------------------------

int
AddVar (variable_ctx* var_ctx, const char* name)
{
    strcpy (var_ctx->variable_arr[var_ctx->count].name, name);

    var_ctx->count++;

    return var_ctx->count - 1;
}

//--------------------------------------------------------------------------------

int
CountPrevStrings (const char* buffer, int pos)
{
    DEBUG_ASSERT (buffer != nullptr);

    int count = 0;

    for (int i = 0; i < pos; i++) {
        if (buffer[i] == '\n') {
            count++;
        }
    }

    return count;
}

//--------------------------------------------------------------------------------

void
RainbowPrint (const char* buffer)
{
    DEBUG_ASSERT (buffer != nullptr);

    for (int i = 0; buffer[i] != '\0'; i++) {
             if (i % 6 == 0) fprintf (stderr, COLOR_RED     "%c" COLOR_RESET, buffer[i]);
        else if (i % 6 == 1) fprintf (stderr, COLOR_YELLOW  "%c" COLOR_RESET, buffer[i]);
        else if (i % 6 == 2) fprintf (stderr, COLOR_GREEN   "%c" COLOR_RESET, buffer[i]);
        else if (i % 6 == 3) fprintf (stderr, COLOR_CYAN    "%c" COLOR_RESET, buffer[i]);
        else if (i % 6 == 4) fprintf (stderr, COLOR_BLUE    "%c" COLOR_RESET, buffer[i]);
        else if (i % 6 == 5) fprintf (stderr, COLOR_MAGENTA "%c" COLOR_RESET, buffer[i]);
    }
}

//--------------------------------------------------------------------------------

void
DebugPrint (const char* str ,int line)
{
    char err_str[CommonStringSize] = "";

    snprintf (err_str, CommonStringSize, "Ti Dolboeb. Syntax Error in line:%d ", line);

    if (str != nullptr) {
        strcat (err_str, str);
    }

    strcat (err_str, "\n");

    RainbowPrint (err_str);
}

//--------------------------------------------------------------------------------