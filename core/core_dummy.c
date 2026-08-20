#include "core_extract.h"
#include "bitstream.h"
#include "windowset.h"
#include "widthset.h"
#include <stdlib.h>

Bitstream abr_dummy_stream(size_t length)
{
    Bitstream S;
    S.length = length;
    S.bits   = NULL;

    if (length > 0) {
        S.bits = (uint8_t *)calloc(length, sizeof(uint8_t));
        if (!S.bits) {
            S.length = 0;
        }
    }

    return S;
}

WindowSet abr_dummy_windows(const WidthSet *G, size_t length)
{
    Bitstream S = abr_dummy_stream(length);
    WindowSet ws = abr_extract(&S, G);
    return ws;
}
