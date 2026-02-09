#include "backend.h"

//--------------------------------------------------------------------------------

int
main ()
{
    TreeInit ();

    char* tree_buffer = nullptr;

    tree_node_t* tree = InputTree ("pisun.txt", &tree_buffer);

    if (tree == nullptr) return 1;

    TreeDump (tree);

    TreeDestroy ();

    return 0;
}