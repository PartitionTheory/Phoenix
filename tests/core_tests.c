#include <stdio.h>
#include "bitstream.h"
#include "widthset.h"
#include "windowset.h"

/* forward decl from core */
WindowSet abr_extract(const Bitstream *S, const WidthSet *G);

int main(void)
{
    uint8_t bits[8] = {1,0,1,0,1,0,1,0};
    Bitstream S = { bits, 8 };

    size_t widths_arr[3] = {2, 3, 4};
    WidthSet G = { widths_arr, 3 };

    WindowSet ws = abr_extract(&S, &G);

    printf("core_tests: windows=%zu\n", ws.count);
    return 0;
}
