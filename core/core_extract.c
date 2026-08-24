#include "core_extract.h"
#include <stdlib.h>
#include <string.h>

/*
A: Mathematical description
   core_extract : {0,1}^N → WindowSet
   Given a byte array B of length N, produce a single window W:
       W.bits   = B expanded into bits
       W.length = 8 * N
       W.width  = W.length
   Output WindowSet has count = 1.

B: Engineering description
   - Allocate one Window
   - Expand bytes into bits (MSB-first)
   - Engine owns memory; caller must free WindowSet
*/

WindowSet core_extract(const uint8_t *bytes, size_t count)
{
    WindowSet out;
    out.count = 1;
    out.windows = calloc(1, sizeof(Window));

    Window *w = &out.windows[0];

    w->length = count * 8;
    w->width  = w->length;
    w->bits   = malloc(w->length);

    /* Expand bytes → bits */
    size_t k = 0;
    for (size_t i = 0; i < count; i++) {
        uint8_t b = bytes[i];
        for (int bit = 7; bit >= 0; bit--) {
            w->bits[k++] = (b >> bit) & 1;
        }
    }

    return out;
}

