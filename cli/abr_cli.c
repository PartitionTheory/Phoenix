#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core_extract.h"
#include "abr_plugin.h"
#include "abr_plugin_registry.h"

/*
A: Mathematical description
   CLI pipeline:
       1. Load bytes B from file.
       2. Compute initial WindowSet W = core_extract(B).
       3. Apply plugin transform T:
            - If plugin is non-branching:
                  W' = T.process_set(W)
            - If plugin is branching:
                  W' = T.process_branch(W)
       4. Print resulting windows.

B: Engineering description
   - Minimal CLI for testing ABR pipeline.
   - Loads plugin by name.
   - Reads file into memory.
   - Calls core_extract().
   - Dispatches to correct plugin function based on is_branching flag.
   - Prints windows in binary.
*/

static void print_window(const Window *w)
{
    for (size_t i = 0; i < w->length; i++) {
        printf("%u", w->bits[i]);
    }
    printf("\n");
}

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

    /* Load plugin */
    Plugin *plugin = abr_load_plugin(plugin_name);
    if (!plugin) {
        fprintf(stderr, "Error: plugin '%s' not found.\n", plugin_name);
        return 1;
    }

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
    fread(bytes, 1, count, fp);
    fclose(fp);

    /* Extract initial windows */
    WindowSet ws = core_extract(bytes, count);
    free(bytes);

    /* Apply plugin */
    if (plugin->is_branching) {
        size_t out_count = 0;
        WindowSet *out = plugin->process_branch(plugin, &ws, NULL, NULL, &out_count);

        for (size_t i = 0; i < out_count; i++) {
            printf("window[%zu]: ", i);
            print_window(&out->windows[i]);
        }

        /* Caller frees branching output */
        for (size_t i = 0; i < out_count; i++) {
            free(out->windows[i].bits);
        }
        free(out->windows);
        free(out);

    } else {
        WindowSet out = plugin->process_set(plugin, &ws, NULL, NULL);

        for (size_t i = 0; i < out.count; i++) {
            printf("window[%zu]: ", i);
            print_window(&out.windows[i]);
        }
    }

    /* Free initial windows */
    for (size_t i = 0; i < ws.count; i++) {
        free(ws.windows[i].bits);
    }
    free(ws.windows);

    return 0;
}

