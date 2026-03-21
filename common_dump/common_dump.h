#ifndef COMMON_DUMP_h
#define COMMON_DUMP_h

//————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

//————————————————————————————————————————————————————————————————————————————————

void        DotDumpTitle        (FILE* dot_file);
void        DotDumpNode         (FILE* dot_file, tree_node_t* node);
void        DotDumpEdge         (FILE* dot_file, tree_node_t* node);
tree_err_t  TreeDump            (tree_node_t* tree_node);
tree_err_t  FillDotFile         (tree_node_t* tree_node , const char* file_name);
tree_err_t  TreeFillHtml        (const char* file_name );
tree_err_t  TreeStartFillHtml   (void);
tree_err_t  TreeEndFillHtml     (void);

//————————————————————————————————————————————————————————————————————————————————

#define SET_NODE_PARAMS                                                                                                      \
fprintf (dot_file, " <ptr> ptr:%p | {<left_node> left_node=%p | <right_node> right_node=%p } }\", color=black, style=filled,", \
        node, node->left_node, node->right_node);

#define SET_NODE_COLOUR(colour) \
fprintf (dot_file, " fillcolor=%s];\n", colour); 

#endif //COMMON_DUMP