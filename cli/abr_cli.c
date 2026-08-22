#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitstream.h"
#include "widthset.h"
#include "windowset.h"
#include "core_extract.h"


/* Load entire file as an unpacked bitstream: each byte is treated as a bit (0 or 1). */
static uint8_t *load_file_bits(const char *path, size_t *out_len) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "Cannot open file: %s\n", path);
        exit(1);
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        fprintf(stderr, "Failed to seek file: %s\n", path);
        fclose(f);
        exit(1);
    }

    long sz = ftell(f);
    if (sz < 0) {
        fprintf(stderr, "Failed to tell file size: %s\n", path);
        fclose(f);
        exit(1);
    }
    if (fseek(f, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Failed to rewind file: %s\n", path);
        fclose(f);
        exit(1);
    }

    uint8_t *buf = malloc((size_t)sz);
    if (!buf) {
        fprintf(stderr, "Out of memory.\n");
        fclose(f);
        exit(1);
    }

    size_t read = fread(buf, 1, (size_t)sz, f);
    fclose(f);

    if (read != (size_t)sz) {
        fprintf(stderr, "Short read on file: %s\n", path);
        free(buf);
        exit(1);
    }

    *out_len = (size_t)sz;
    return buf;
}

/* Parse comma-separated widths, e.g. "3,2,3" → {3,2,3}. */
static size_t *parse_widths(const char *arg, size_t *count) {
    char *tmp = strdup(arg);
    if (!tmp) {
        fprintf(stderr, "Out of memory.\n");
        exit(1);
    }

    size_t cap = 16;
    size_t *arr = malloc(cap * sizeof(size_t));
    if (!arr) {
        fprintf(stderr, "Out of memory.\n");
        free(tmp);
        exit(1);
    }

    *count = 0;

    char *tok = strtok(tmp, ",");
    while (tok) {
        if (*count == cap) {
            cap *= 2;
            size_t *new_arr = realloc(arr, cap * sizeof(size_t));
            if (!new_arr) {
                fprintf(stderr, "Out of memory.\n");
                free(arr);
                free(tmp);
                exit(1);
            }
            arr = new_arr;
        }
        arr[*count] = (size_t)atoi(tok);
        (*count)++;
        tok = strtok(NULL, ",");
    }

    free(tmp);
    return arr;
}

int main(int argc, char **argv) {
    if (argc < 4) {
        printf("Usage:\n");
        printf("  abr run <file> --widths 3,2,3\n");
        return 1;
    }

    const char *cmd = argv[1];
    if (strcmp(cmd, "run") != 0) {
        fprintf(stderr, "Unknown command: %s\n", cmd);
        return 1;
    }

    const char *file = argv[2];
    const char *widths_arg = NULL;

    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "--widths") == 0 && i + 1 < argc) {
            widths_arg = argv[++i];
        }
    }

    if (!widths_arg) {
        fprintf(stderr, "Missing --widths argument.\n");
        return 1;
    }

    /* Load bitstream */
    size_t bit_len = 0;
    uint8_t *bits = load_file_bits(file, &bit_len);
    Bitstream S = { bits, bit_len };

    /* Parse widths */
    size_t width_count = 0;
    size_t *widths = parse_widths(widths_arg, &width_count);
    WidthSet G = { widths, width_count };

    /* Core extraction */
    WindowSet out = abr_extract(&S, &G);

    /* Print */
    printf("\nFinal windows:\n");
    for (size_t i = 0; i < out.count; ++i) {
        Window *w = &out.windows[i];
        printf("  window[%zu] (width=%zu, length=%zu): ",
               i,
               w->width,
               w->length);

        for (size_t b = 0; b < w->length; ++b) {
            printf("%u", w->bits[b]);
        }
        printf("\n");
    }

    /* Cleanup */
    for (size_t i = 0; i < out.count; ++i) {
        free(out.windows[i].bits);
    }
    free(out.windows);

    free(bits);
    free(widths);

    return 0;
}
