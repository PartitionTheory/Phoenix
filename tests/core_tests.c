#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "core_extract.h"

/*
A: Mathematical description
   Test that core_extract expands bytes → bits correctly.

B: Engineering description
   - Feed known bytes
   - Print extracted bits
   - Developer visually verifies correctness
*/

static void print_window(const Window *w)
{
    for (size_t i = 0; i < w->length; i++) {
        printf("%u", w->bits[i]);
    }
    printf("\n");
}

int main(void)
{
    uint8_t data[3] = { 0xDE, 0xAD, 0xBE };

    WindowSet ws = core_extract(data, 3);

    printf("Extracted %zu windows\n", ws.count);
    print_window(&ws.windows[0]);

    /* Free memory */
    free(ws.windows[0].bits);
    free(ws.windows);

    return 0;
}

