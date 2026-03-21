#include "frontend.h"

//--------------------------------------------------------------------------------

tree_err_t 
LangInit (const char* file_name)
{
    ClearDump ();

    if (TreeInit () != tree_err_t::Success) {
        return tree_err_t::AllocErr;
    }

    return tree_err_t::Success;
}

//--------------------------------------------------------------------------------

void 
ClearDump (void)
{
    char command_1[CommonStringSize] = "";
    char command_2[CommonStringSize] = "";

    snprintf (command_1, CommonStringSize, "rm dump/svg/*.svg");
    snprintf (command_2, CommonStringSize, "rm dump/dot/*.dot");

    system (command_1);
    system (command_2);
}

//--------------------------------------------------------------------------------

tree_err_t
VariablesInit (variable_ctx* variables_ctx)
{
    variables_ctx->variable_arr = (variable_t*) calloc (StartVarCap, sizeof (variable_t));

    if (variables_ctx->variable_arr == nullptr) {
        PRINTERR (tree_err_t::AllocErr);
        return tree_err_t::AllocErr;
    }

    for (int i = 0; i < StartVarCap; i++) {
        variables_ctx->variable_arr[i].name = (char*) calloc (CommonStringSize, sizeof (char));

        if (variables_ctx->variable_arr[i].name == nullptr) {
            PRINTERR (tree_err_t::AllocErr);
            return tree_err_t::AllocErr;
        }
    }

    variables_ctx->capacity = StartVarCap;

    return tree_err_t::Success;
}

//--------------------------------------------------------------------------------

void
VariablesDtor (variable_ctx* var_ctx)
{
    DEBUG_ASSERT (var_ctx != nullptr);

    for (int i = 0; i < var_ctx->capacity;i ++) {
        free (var_ctx->variable_arr->name);
    }

    free (var_ctx->variable_arr);

    return ;
}

//--------------------------------------------------------------------------------