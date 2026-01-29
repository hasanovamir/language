#include "lang.h"

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

tree_node_t* 
NewNode (node_type_t type, tree_data_t data, tree_node_t* left, tree_node_t* right)
{
    tree_node_t* new_node = nullptr;

    MakeNode (&new_node);

    new_node->type = type;

    new_node->node_data = data;

    new_node->left_node  = left;
    new_node->right_node = right;

    return new_node;
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