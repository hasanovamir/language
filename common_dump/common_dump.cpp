#include "lang.h"

//--------------------------------------------------------------------------------

tree_err_t 
FillDotFile (tree_node_t* tree_node, const char* file_name)
{
    DEBUG_ASSERT (file_name != NULL);

    FILE* dot_file = fopen (file_name, "w");
    
    if (dot_file == NULL)
    {
        PRINTERR (tree_err_t::OpenFileErr);
        return   (tree_err_t::OpenFileErr);
    }

    DotDumpTitle (dot_file);
    DotDumpNode  (dot_file, tree_node);
    DotDumpEdge  (dot_file, tree_node);
    
    fprintf (dot_file, "}\n");

    fclose (dot_file);

    return tree_err_t::Success;
}

//--------------------------------------------------------------------------------

void 
DotDumpTitle (FILE* dot_file)
{
    DEBUG_ASSERT (dot_file != NULL);

    char title[] = 

R"(digraph ListDump {
    rankdir=TB;
    node [shape=Mrecord, color=black, fontname="Courier"];

    graph [splines=ortho]
    overlap=false;
    
)";

    fprintf (dot_file, "%s", title);
}

//--------------------------------------------------------------------------------

void
DotDumpNode (FILE* dot_file, tree_node_t* node)
{
    DEBUG_ASSERT (dot_file != NULL);
    DEBUG_ASSERT (node     != NULL);

    fprintf (dot_file, "\tnode%d [label=\"{<type> type = ", node->idx);

    switch (node->type) {
        case node_type_t::Constant :
            fprintf (dot_file, "NUM | VAL = %lg |", node->node_data.immediate);
            break;
        case node_type_t::Connection :
            fprintf (dot_file, "CON | VAL = NewOp |");
            break;
        case node_type_t::Keyword :
            fprintf (dot_file, "KeyWord | VAL = \\%s |", kKeywords[(int)node->node_data.keyword]);
            break;
        case node_type_t::MathFunc :
            fprintf (dot_file, "MATH_FUNC | VAL = %s  |", kMathFunctions[(int)node->node_data.math_func]);
            break;
        case node_type_t::MathOper :
            fprintf (dot_file, "MATH_OPER | VAL = \\%s |", kMathOperations[(int)node->node_data.math_oper]);
            break;
        case node_type_t::Variable :
            fprintf (dot_file, "VAR | VAL = %.*s  |", node->node_data.variable.len, node->node_data.variable.name);
            break;
        default :
            PRINTERR (Unknown data type);
            fprintf (stderr, "node type = %d\n", node->type);
            return ;
    }

    fprintf (dot_file, " <ptr> ptr:%p | {<left_node> left_node=%p | <right_node> right_node=%p } }\", color=black, style=filled, fillcolor=lime];\n", 
        node, node->left_node, node->right_node);

    if (node->left_node)
        DotDumpNode (dot_file, node->left_node);
    if (node->right_node)
        DotDumpNode (dot_file, node->right_node);
}

//--------------------------------------------------------------------------------

void 
DotDumpEdge (FILE* dot_file, tree_node_t* node)
{
    DEBUG_ASSERT (dot_file != NULL);

    if (node == NULL)
        return;

    if (node->left_node) {
        fprintf (dot_file, "\tnode%d -> node%d [color=green];\n", node->idx, node->left_node->idx);
        DotDumpEdge (dot_file, node->left_node);
    }

    if (node->right_node) {
        fprintf (dot_file, "\tnode%d -> node%d [color=green];\n", node->idx, node->right_node->idx);
        DotDumpEdge (dot_file, node->right_node);
    }
}

//--------------------------------------------------------------------------------

tree_err_t 
TreeDump (tree_node_t* tree_node)
{
    tree_context.num_calls++;

    char dot_file_name[MaxFileNameSize];
    char svg_file_name[MaxFileNameSize];
    char command[MaxCommandSize];
    
    snprintf (dot_file_name, MaxFileNameSize, "dump/dot/%s_%d.dot", "call", tree_context.num_calls);
    snprintf (svg_file_name, MaxFileNameSize, "dump/svg/%s_%d.svg", "call", tree_context.num_calls);
    
    if (FillDotFile (tree_node, dot_file_name) != tree_err_t::Success)
    {
        return tree_err_t::OpenFileErr;
    }

    printf ("\nGenerated DOT file:  %s\n", dot_file_name);
    
    snprintf (command, sizeof(command), "dot -Tsvg %s -o %s", dot_file_name, svg_file_name);
    
    if (system(command) == 0)
    {
        printf ("Generated SVG image: %s\n", svg_file_name);
    }
    else
    {
        PRINTERR (tree_err_t::SysCallErr);
        return   (tree_err_t::SysCallErr);
    }

    if (TreeFillHtml (svg_file_name) != tree_err_t::Success)
        return tree_err_t::OpenFileErr;

    return tree_err_t::Success;
}

//--------------------------------------------------------------------------------

tree_err_t 
TreeStartFillHtml (void)
{
    FILE* html_file = fopen ("dump/tree_dump.html", "w");

    if (html_file == NULL)
    {
        PRINTERR (tree_err_t::OpenFileErr);
        return   (tree_err_t::OpenFileErr);
    }

    fprintf (html_file, "<!DOCTYPE html>\n<html>\n<head>\n\t<title>LIST DUMP</title>\n</head>\n<body>\n");

    fclose (html_file);

    return tree_err_t::Success;
}

//--------------------------------------------------------------------------------

tree_err_t 
TreeFillHtml (const char* file_name)
{
    FILE* html_file = fopen ("dump/tree_dump.html", "a");

    if (html_file == NULL)
    {
        PRINTERR (tree_err_t::OpenFileErr);
        return   (tree_err_t::OpenFileErr);
    }

    char string[MaxStringSize] = "";

    snprintf (string, MaxStringSize, "\t<h2>CALL = #%d</h2>\n\t<p>HEAD = %p</p>\n\t<img src=\"%s\">\n\n", 
        tree_context.num_calls, tree_context.head, file_name);

    fprintf (html_file, "%s", string);

    fclose (html_file);

    return tree_err_t::Success;
}

//--------------------------------------------------------------------------------

tree_err_t 
TreeEndFillHtml (void)
{
    FILE* html_file = fopen ("dump/tree_dump.html", "a");

    if (html_file == NULL)
    {
        PRINTERR (tree_err_t::OpenFileErr);
        return   (tree_err_t::OpenFileErr);
    }

    fprintf (html_file, "</body>\n</html>");

    fclose (html_file);

    return tree_err_t::Success;
}

//--------------------------------------------------------------------------------