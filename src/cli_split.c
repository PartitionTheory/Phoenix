#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "window_extract.h"
#include "bigint.h"
#include "bigint_list.h"
#include "abr_config.h"

/*
 * CLI Split (Phoenix Edition)
 *
 * This tool:
 *   - reads a hex BigInt from input
 *   - extracts windows
 *   - prints each window as hex
 *
 * Math / Engineering view:
 *   Given BigInt x with bitstring B:
 *
 *       W_i = B[i*w : (i+1)*w]
 *
 *   This file is a thin wrapper around window_extract_all().
 */

/* -----------------------------------------------------------
 * Parse hex string into BigInt
 * -----------------------------------------------------------
 */
static int parse_hex(BigInt *out, const char *hex)
{
    size_t len = strlen(hex);

    if (len % 2 != 0)
        return -1;

    size_t bytes = len / 2;
    if (bigint_alloc(out, bytes) != 0)
        return -2;

    for (size_t i = 0; i < bytes; i++) {
        unsigned int v;
        if (sscanf(hex + 2*i, "%2x", &v) != 1)
            return -3;
        out->bytes[bytes - 1 - i] = (uint8_t)v; /* big-endian input → little-endian storage */
    }

    /* Compute bit_len */
    size_t i = bytes;
    while (i > 0 && out->bytes[i - 1] == 0)
        i--;

    if (i == 0) {
        out->bit_len = 0;
        out->byte_len = 0;
        return 0;
    }

    out->byte_len = i;

    uint8_t last = out->bytes[i - 1];
    size_t bits = 0;
    while (last > 0) {
        last >>= 1;
        bits++;
    }

    out->bit_len = (i - 1) * 8 + bits;
    return 0;
}

/* -----------------------------------------------------------
 * CLI split entrypoint
 * -----------------------------------------------------------
 */
int cli_split_main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <window_bits> <hex_bigint>\n", argv[0]);
        return 1;
    }

    size_t window_bits = (size_t)atoi(argv[1]);

    AbrConfig cfg;
    cfg.bitwidth.window_bits = window_bits;
    cfg.bitwidth.max_bits = 1 << 20; /* 1M bits */
    cfg.max_windows = 1 << 16;       /* 65536 windows */

    BigInt input;
    bigint_init(&input);

    if (parse_hex(&input, argv[2]) != 0) {
        fprintf(stderr, "Invalid hex BigInt.\n");
        return 2;
    }

    BigIntList windows;
    bigint_list_init(&windows);

    if (window_extract_all(&input, &cfg, &windows) != 0) {
        fprintf(stderr, "Window extraction failed.\n");
        bigint_free(&input);
        bigint_list_free(&windows);
        return 3;
    }

    /* Print windows */
    for (size_t i = 0; i < windows.count; i++) {
        BigInt *w = &windows.items[i];

        printf("W[%zu] = ", i);
        for (size_t j = w->byte_len; j-- > 0;) {
            printf("%02x", w->bytes[j]);
        }
        printf("\n");
    }

    bigint_free(&input);
    bigint_list_free(&windows);
    return 0;
}
