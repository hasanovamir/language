#include "lang.h"
#include "general_io_file_func.h"

//--------------------------------------------------------------------------------

int
TreeReadDataBase (const char* FileName, char** buffer)
{
    DEBUG_ASSERT (FileName    != NULL);

    int size = get_file_size (FileName);

    if (size == -1) {
        PRINTERR (TREE_SYS_FUNC_ERR);
        return -1;
    }

    *buffer = (char*) calloc (size, sizeof (char));
    
    if (*buffer == nullptr) {
        PRINTERR (tree_err_t::AllocErr);
        return -1;
    }

    if (ReadBuffer (*buffer, FileName, size) != tree_err_t::Success) {
        return -1;
    }

    return size;
}

//--------------------------------------------------------------------------------

tree_err_t 
ReadBuffer (char* buffer, const char* FileName, int size)
{
    DEBUG_ASSERT (buffer   != NULL);
    DEBUG_ASSERT (FileName != NULL);

    FILE* file = fopen (FileName, "r");

    if (file == NULL) {
        PRINTERR (tree_err_t::OpenFileErr);
        return    tree_err_t::OpenFileErr;
    }

    int read_size = ( int ) fread ( buffer, sizeof ( char ), size, file );

    if ( read_size != size ) {
        PRINTERR (tree_err_t::InvalidFileSize);
        return tree_err_t::InvalidFileSize;
    }

    fclose (file);

    return tree_err_t::Success;
}

//--------------------------------------------------------------------------------

void 
SkipSpace (const char* buffer, int* pos, int* cur_line)
{
    DEBUG_ASSERT (buffer != nullptr);

    while (buffer[*pos] == ' ' || buffer[*pos] == '\r' || buffer[*pos] == '\n') {
        if (buffer[*pos] == '\n') *cur_line += 1;
        (*pos)++;
    }
}

//--------------------------------------------------------------------------------

void 
DebugPrint (const char* buffer, int pos, int size)
{
    for (int i = 0; i < pos; i++) {
        fprintf (stderr, COLOR_GREEN "%c" COLOR_RESET, buffer[i]);
    }

    fprintf (stderr, COLOR_RED "|%c|", buffer[pos]);

    for (int i = pos + 1; i < size; i++) {
        fprintf (stderr, COLOR_YELLOW "%c" COLOR_RESET, buffer[i]);
    }

    fprintf (stderr, "\n\n");
}

//--------------------------------------------------------------------------------