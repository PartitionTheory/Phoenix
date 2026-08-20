#include "bitstream.h"
#include "window.h"
#include <stdlib.h>

Window abr_slice(const Bitstream *S, size_t pos, size_t width)
{
    Window W;
    W.width  = width;
    W.length = 0;
    W.bits   = NULL;

    if (pos >= S->length) {
        return W;
    }

    size_t end = pos + width;
    if (end > S->length) {
        end = S->length;
    }

    size_t len = end - pos;
    W.length   = len;

    if (len > 0) {
        W.bits = (uint8_t *)malloc(len * sizeof(uint8_t));
        if (!W.bits) {
            W.length = 0;
            return W;
        }
        for (size_t i = 0; i < len; ++i) {
            W.bits[i] = S->bits[pos + i];
        }
    }

    return W;
}
