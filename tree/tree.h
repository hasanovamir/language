#ifndef TREE_H
#define TREE_H

//————————————————————————————————————————————————————————————————————————————————

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "tree_types.h"
#include "list.h"
#include "general_io_file_func.h"


//————————————————————————————————————————————————————————————————————————————————

struct big_array_data_t {
    list_t list;
    int pos_in_cur_arr;
};

//————————————————————————————————————————————————————————————————————————————————

struct my_alloc_context_t
{
    int src_cap;
    int src_size;
    int cur_array;
    int next_array;
    int num_src_arr;
    big_array_data_t* big_array;
};

//————————————————————————————————————————————————————————————————————————————————

struct tree_context_t {
    int num_calls;
    int size;
    tree_node_t* head;
    my_alloc_context_t* my_alloc_context;
};

//————————————————————————————————————————————————————————————————————————————————

extern tree_context_t tree_context;

//————————————————————————————————————————————————————————————————————————————————
void         TreeDestroy         ();
void         MyFree              (tree_node_t* node);
void         TreeDeleteBranch    (tree_node_t* node);
void         FreeSideNodes       (tree_node_t* parent_node);
void         SkipSpace           (const char*  buffer, int* pos, int* cur_line);
void         DebugPrint          (const char*  buffer, int pos, int size);
tree_err_t   TreeInit            ();
tree_err_t   InitBigArray        ();
tree_err_t   ChangeSrcArray      ();
tree_node_t* MyAlloc             ();
int
TreeReadDataBase (const char* FileName, char** buffer);
tree_err_t   MakeNode            (tree_node_t** Node    );
tree_err_t   ReadBuffer          (char* buffer, const char* FileName, int size);
tree_err_t   TreeInsert          (tree_data_t val, tree_node_t* ParentNode, tree_data_t cond);
tree_node_t* 
NewNode (node_type_t type, tree_data_t data, tree_node_t* left, tree_node_t* right);

//————————————————————————————————————————————————————————————————————————————————

#endif // TREE_H