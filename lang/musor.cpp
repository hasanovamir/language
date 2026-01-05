#include "lang.h"

//--------------------------------------------------------------------------------

static language_err_t CheckVarArr (variable_ctx* var_ctx);

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

int
AddVar (variable_ctx* var_ctx, variable_t var)
{
    CheckVarArr (var_ctx);

    var_ctx->variable_arr[var_ctx->count] = var;

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

    snprintf (err_str, CommonStringSize, "ERROR. Syntax Error in line:%d ", line);

    if (str != nullptr) {
        strcat (err_str, str);
    }

    strcat (err_str, "\n");

    RainbowPrint (err_str);
}

//--------------------------------------------------------------------------------

static language_err_t
CheckVarArr (variable_ctx* var_ctx)
{
    variable_t* var_arr = var_ctx->variable_arr;

    int var_cap = var_ctx->capacity;

    if (var_ctx->count == var_cap - 1) {
        variable_t* tmp = (variable_t*) realloc (var_arr, var_cap * 2);

        if (tmp == nullptr) {
            PRINTERR (language_err_t::AlocationErr);
            return language_err_t::AlocationErr;
        }

        var_ctx->variable_arr = tmp;
        var_ctx->capacity = var_cap * 2;
    }

    return language_err_t::Success;
}

//--------------------------------------------------------------------------------