#include "bitstream.h"
#include "windowset.h"
#include "widthset.h"
#include <stdlib.h>

WindowSet abr_extract(const Bitstream *S, const WidthSet *G)
{
    WindowSet ws;
    ws.count   = G->count;
    ws.windows = (Window *)calloc(ws.count, sizeof(Window));
    if (!ws.windows) {
        ws.count = 0;
        return ws;
    }

    size_t cursor = 0;

    for (size_t i = 0; i < G->count; ++i) {
        size_t w   = G->widths[i];
        size_t end = cursor + w;
        if (end > S->length) {
            end = S->length;
        }

        size_t len = (end > cursor) ? (end - cursor) : 0;

        ws.windows[i].width  = w;
        ws.windows[i].length = len;
        ws.windows[i].bits   = NULL;

        if (len > 0) {
            ws.windows[i].bits = (uint8_t *)malloc(len * sizeof(uint8_t));
            if (!ws.windows[i].bits) {
                /* on allocation failure, truncate remaining windows */
                ws.windows[i].length = 0;
                break;
            }
            for (size_t j = 0; j < len; ++j) {
                ws.windows[i].bits[j] = S->bits[cursor + j];
            }
        }

        cursor += w;
        if (cursor >= S->length) {
            cursor = S->length;
        }
    }

    return ws;
}
