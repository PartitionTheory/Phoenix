#include "windowset.h"
#include <stdlib.h>
#include <string.h>

/*
A: Mathematical description
   slice(W, a, b) = W[a..b-1]
   Produces a new Window with width = length = (b - a).

B: Engineering description
   - Allocates a new Window
   - Copies bit range [a, b)
   - Caller owns memory
*/

Window slice_window(const Window *src, size_t a, size_t b)
{
    Window w;
    w.length = b - a;
    w.width  = w.length;
    w.bits   = malloc(w.length);

    memcpy(w.bits, src->bits + a, w.length);
    return w;
}

