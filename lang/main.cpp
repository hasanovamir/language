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
    
    tree_node_t* node = ReadTree (tokens, node_num);
    
    TreeDump (node);

    return 0;
}

//--------------------------------------------------------------------------------