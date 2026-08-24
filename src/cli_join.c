#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abr_stream.h"
#include "window_extract.h"
#include "reconstruct.h"
#include "bigint.h"
#include "bigint_list.h"
#include "abr_config.h"

/*
 * CLI Join (Phoenix Edition)
 *
 * This tool:
 *   - reads windows from input
 *   - reconstructs the original BigInt
 *   - prints the result
 *
 * Math / Engineering view:
 *   Given windows W_i:
 *
 *       x = Σ W_i * 2^(i*w)
 *
 *   This file is a thin wrapper around the reconstruction engine.
 */

/* -----------------------------------------------------------
 * Parse hex string into BigInt
 * -----------------------------------------------------------
 */
static int parse_hex(BigInt *out, const char *hex)
{
    size_t len = strlen(hex);

    /* Each pair of hex digits = 1 byte */
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
 * CLI join entrypoint
 * -----------------------------------------------------------
 */
int cli_join_main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <window_bits> <hex_window_0> [hex_window_1 ...]\n", argv[0]);
        return 1;
    }

    size_t window_bits = (size_t)atoi(argv[1]);

    AbrConfig cfg;
    cfg.bitwidth.window_bits = window_bits;
    cfg.bitwidth.max_bits = 1 << 20; /* 1M bits default */
    cfg.max_windows = 1 << 16;       /* 65536 windows */

    ReconstructAcc acc;
    reconstruct_init(&acc, &cfg);

    /* Consume each window */
    for (int i = 2; i < argc; i++) {
        BitWindow w;
        bitwindow_init(&w, window_bits);

        if (parse_hex(&w.value, argv[i]) != 0) {
            fprintf(stderr, "Invalid hex window: %s\n", argv[i]);
            bitwindow_free(&w);
            reconstruct_free(&acc);
            return 2;
        }

        if (reconstruct_consume(&acc, &w, (size_t)(i - 2)) != 0) {
            fprintf(stderr, "Reconstruction failed.\n");
            bitwindow_free(&w);
            reconstruct_free(&acc);
            return 3;
        }

        bitwindow_free(&w);
    }

    /* Finalize */
    BigInt result;
    bigint_init(&result);

    if (reconstruct_finalize(&acc, &result) != 0) {
        fprintf(stderr, "Finalize failed.\n");
        reconstruct_free(&acc);
        return 4;
    }

    /* Print result as hex */
    printf("Result: ");
    for (size_t i = result.byte_len; i-- > 0;) {
        printf("%02x", result.bytes[i]);
    }
    printf("\n");

    bigint_free(&result);
    reconstruct_free(&acc);
    return 0;
}
