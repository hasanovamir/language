#include "backend.h"

//--------------------------------------------------------------------------------

static FILE* file = nullptr;
static int labels_count = 0;

//--------------------------------------------------------------------------------

static void
TranslateNum (tree_node_t* node)
{
    DEBUG_ASSERT (node != nullptr);

    if (node->type != node_type_t::Connection) return;

    fprintf (file, "PUSH %d\n", node->node_data.immediate);

    return;
}

//--------------------------------------------------------------------------------

static void
TranslateMul (tree_node_t* node)
{
    DEBUG_ASSERT (node != nullptr);

    if (node->type                != node_type_t::MathOper       &&
       (node->node_data.math_oper != math_oper_t::Multiplication &&
        node->node_data.math_oper != math_oper_t::Division )) return;

    TranslateNum (node->left_node );
    TranslateNum (node->right_node);

    if (node->node_data.math_oper == math_oper_t::Multiplication) {
        fprintf (file, "MUL\n");
    }
    else {
        fprintf (file, "DIV\n");
    }

    return;
}

//--------------------------------------------------------------------------------

static void
TranslateAdd (tree_node_t* node)
{
    DEBUG_ASSERT (node != nullptr);

    if (node->type                != node_type_t::MathOper &&
       (node->node_data.math_oper != math_oper_t::Addition &&
        node->node_data.math_oper != math_oper_t::Difference )) return;

    TranslateNum (node->left_node );
    TranslateNum (node->right_node);

    if (node->node_data.math_oper == math_oper_t::Addition) {
        fprintf (file, "ADD\n");
    }
    else {
        fprintf (file, "DIF\n");
    }

    return;
}

//--------------------------------------------------------------------------------