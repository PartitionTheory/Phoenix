#include "bitstream.h"
#include "windowset.h"
#include "widthset.h"
#include <stdlib.h>

/* allocate a dummy bitstream of given length, all zeros */
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

/* build dummy windows by running core extract on a dummy stream */
WindowSet abr_dummy_windows(const WidthSet *G, size_t length)
{
    Bitstream S = abr_dummy_stream(length);
    WindowSet ws = abr_extract(&S, G);

    /* caller is responsible for freeing S.bits and ws contents */
    return ws;
}
