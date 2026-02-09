#include "lang.h"

//--------------------------------------------------------------------------------

int
main ()
{
    LangInit (nullptr);

    token_t* tokens = nullptr;

    char* buffer =  nullptr;
    int size = TreeReadDataBase ("govno.txt", &buffer);

    int node_num = -1;

    if ((node_num = MakeNodeBuffer (buffer, size, &tokens)) == -1) return 1;
    
    tree_node_t* node = MakeTree (tokens, node_num);

    if (node == nullptr) return 1;
    
    TreeDump (node);

    SaveASTtoFile ("pisun.txt", node);

    TreeDestroy ();

    return 0;
}

//--------------------------------------------------------------------------------