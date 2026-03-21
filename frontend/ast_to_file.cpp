#include "frontend.h"

//--------------------------------------------------------------------------------

static frontend_err_t PrintASTNodetoFile (FILE* file, const tree_node_t* node, int deep     );
static frontend_err_t PrintASTNodeByType (FILE* file, const tree_node_t* node, int tab_count);

//--------------------------------------------------------------------------------

frontend_err_t
SaveASTtoFile (const char* file_name, const tree_node_t* head)
{
    DEBUG_ASSERT (head != nullptr);

    FILE* AST = fopen (file_name, "w");

    if (AST == nullptr) {
        fprintf (stderr, "FopenErr. Can not open file for AST\n");
        return frontend_err_t::FopenErr;
    }

    if (PrintASTNodetoFile (AST, head, 0) != frontend_err_t::Success) {
        fclose (AST);
        return frontend_err_t::UnknownDataType;
    }

    fclose (AST);

    return frontend_err_t::Success;
}

//--------------------------------------------------------------------------------

static frontend_err_t
PrintASTNodetoFile (FILE* file, const tree_node_t* node, int deep)
{
    DEBUG_ASSERT (file != nullptr);
    DEBUG_ASSERT (node != nullptr);

    if (node->left_node) {
        for (int i = 0; i < deep; i++) {
            fprintf (file, "\t");
        }

        fprintf (file, "{\n");

        PrintASTNodetoFile (file, node->left_node, deep + 1);

        for (int i = 0; i < deep; i++) {
            fprintf (file, "\t");
        }

        fprintf (file, "}\n");
    }

    if (PrintASTNodeByType (file, node, deep) != frontend_err_t::Success) {
        return frontend_err_t::UnknownDataType;
    }

    if (node->right_node) {
        for (int i = 0; i < deep; i++) {
        fprintf (file, "\t");
        }

        fprintf (file, "{\n");

        PrintASTNodetoFile (file, node->right_node, deep + 1);

        for (int i = 0; i < deep; i++) {
        fprintf (file, "\t");
        }

        fprintf (file, "}\n");
    }

    return frontend_err_t::Success;
}

//--------------------------------------------------------------------------------

static frontend_err_t
PrintASTNodeByType (FILE* file, const tree_node_t* node, int tab_count)
{
    DEBUG_ASSERT (file != nullptr);
    DEBUG_ASSERT (node != nullptr);

    while (tab_count--) {
        fprintf (file, "\t");
    }

    switch (node->type) {
        case node_type_t::Keyword :
            fprintf (file, "%s\n", kKeywords[(int)node->node_data.keyword]);
            break;
        case node_type_t::MathOper :
            fprintf (file, "%s\n", kMathOperations[(int)node->node_data.math_oper]);
            break;
        case node_type_t::Constant :
            fprintf (file, "%d\n", node->node_data.immediate);
            break;
        case node_type_t::Variable :
            fprintf (file, "%.*s\n", node->node_data.variable.len, node->node_data.variable.name);
            break;
        case node_type_t::Connection :
            fprintf (file, "connector\n");
            break;
        default :
            fprintf (stderr, "Unknown data type\n");
            return frontend_err_t::UnknownDataType;
    }

    return frontend_err_t::Success;
}

//--------------------------------------------------------------------------------