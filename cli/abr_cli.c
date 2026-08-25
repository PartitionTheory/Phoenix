#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#include "core_extract.h"
#include "abr_plugin.h"
#include "abr_plugin_registry.h"
#include "bigint.h"
#include "bitwindow.h"

int main(int argc, char **argv)
{
    if (argc < 4) {
        fprintf(stderr,
            "Usage: %s <file> --plugin <name> --widths <comma-separated>\n",
            argv[0]);
        return 1;
    }

    /* Parse arguments */
    const char *file_path = argv[1];
    const char *plugin_name = NULL;

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--plugin") == 0 && i + 1 < argc) {
            plugin_name = argv[i + 1];
        }
    }

    if (!plugin_name) {
        fprintf(stderr, "Error: --plugin <name> required.\n");
        return 1;
    }

    /* Load plugin (Phoenix ABI) */
    const abr_plugin_entry *entry = abr_find_plugin(plugin_name);
    if (!entry) {
        fprintf(stderr, "Error: plugin '%s' not found.\n", plugin_name);
        return 1;
    }

    const abr_plugin_descriptor *plugin = entry->desc;

    /* Load file */
    FILE *fp = fopen(file_path, "rb");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    size_t count = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    uint8_t *bytes = malloc(count);
    if (!bytes) {
        fclose(fp);
        fprintf(stderr, "Error: malloc failed.\n");
        return 1;
    }

    fread(bytes, 1, count, fp);
    fclose(fp);

    /* Extract initial windows (legacy WindowSet) */
    WindowSet ws = core_extract(bytes, count);
    free(bytes);

    /* Phoenix ABI context */
    abr_context ctx = {0};

    for (size_t i = 0; i < ws.count; i++) {

        /* Convert legacy Window → BigInt */
        BigInt tmp;
        bigint_init(&tmp);

        size_t bit_len = ws.windows[i].length;
        size_t byte_len = (bit_len + 7) / 8;

        uint8_t *buf = calloc(byte_len, 1);
        if (!buf) {
            fprintf(stderr, "Error: calloc failed.\n");
            return 1;
        }

        for (size_t bit = 0; bit < bit_len; bit++) {
            if (ws.windows[i].bits[bit]) {
                buf[bit >> 3] |= (1u << (bit & 7));
            }
        }

        bigint_set(&tmp, buf, byte_len);
        free(buf);

        /* Convert BigInt → Phoenix BitWindow */
        BitWindow bw;
        bitwindow_init(&bw, bit_len);
        bitwindow_set_from_bigint(&bw, &tmp, 0);

        bigint_free(&tmp);

        /* Apply plugin */
        plugin->apply(&ctx, &bw);

        /* Print Phoenix BitWindow using legacy bits */
        printf("window[%zu]: ", i);
        for (size_t bit = 0; bit < ws.windows[i].length; bit++) {
            printf("%u", ws.windows[i].bits[bit]);
        }
        printf("\n");


        bitwindow_free(&bw);
    }

    /* Free legacy windows */
    for (size_t i = 0; i < ws.count; i++) {
        free(ws.windows[i].bits);
    }
    free(ws.windows);

    return 0;
}
