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
    fprintf (stderr, "before node_buffer\n");
    if ((node_num = MakeNodeBuffer (buffer, size, &tokens)) == -1) return 1;
    fprintf (stderr, "before Read\n");
    tree_node_t* node = ReadTree (tokens, node_num);
    fprintf (stderr, "before dump\n");
    TreeDump (node);

    return 0;
}

//--------------------------------------------------------------------------------