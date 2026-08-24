#include "flip_stretch.h"
#include "bitwindow.h"
#include "context.h"

/*
 * Phoenix ABI implementation: flip bits, then stretch window
 */

void flip_stretch_plugin_apply(struct abr_context *ctx, BitWindow *window)
{
    if (!window) return;

    /* Flip */
    for (size_t i = 0; i < window->length; ++i) {
        window->bits[i] ^= 1;
    }

    /* Stretch: duplicate each bit once */
    size_t new_len = window->length * 2;
    uint8_t *new_bits = malloc(new_len);

    for (size_t i = 0; i < window->length; ++i) {
        new_bits[2*i]     = window->bits[i];
        new_bits[2*i + 1] = window->bits[i];
    }

    free(window->bits);
    window->bits   = new_bits;
    window->length = new_len;
}
