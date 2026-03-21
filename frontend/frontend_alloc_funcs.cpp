#include "frontend.h"

//--------------------------------------------------------------------------------

frontend_err_t
InitParserCtx (parser_ctx_t* parser, const char* buffer, int size)
{
    DEBUG_ASSERT (buffer != nullptr);

    parser->tokens = (token_t*) calloc (CommonStackSize, sizeof (token_t));

    if (parser->tokens == nullptr) {
        PRINTERR (frontend_err_t::AlocationErr);
        return frontend_err_t::AlocationErr;
    }

    parser->capacity = CommonStackSize;
    parser->buffer   = buffer;
    parser->size     = size;

    return frontend_err_t::Success;
}

//--------------------------------------------------------------------------------

frontend_err_t
UpsizeTokenBuffer (parser_ctx_t* parser)
{
    DEBUG_ASSERT (parser != nullptr);

    token_t* new_arr = (token_t*) realloc (parser->tokens, 
                        parser->capacity * sizeof (token_t) * 2);

    if (new_arr == nullptr) {
        PRINTERR (frontend_err_t::AlocationErr);
        return frontend_err_t::AlocationErr;
    }

    parser->tokens = new_arr;

    parser->capacity *= 2;

    return frontend_err_t::Success;
}

//--------------------------------------------------------------------------------