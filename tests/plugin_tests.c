#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "core_extract.h"
#include "abr_plugin_registry.h"
#include "windowset.h"
#include "flags.h"
#include "context.h"

static void print_window(const Window *w)
{
    for (size_t i = 0; i < w->width; i++) {
        printf("%u", w->bits[i]);
    }
    printf("\n");
}

int main(void)
{
    uint8_t data[3] = { 0xDE, 0xAD, 0xBE };
    WindowSet ws = core_extract(data, 3);
    //debug only delete
    printf("ws.count = %zu\n", ws.count);
    for (size_t i = 0; i < ws.count; i++) {
        printf("ws[%zu].length = %zu, width = %zu\n",
               i, ws.windows[i].length, ws.windows[i].width);
    }

    //end of debug
    Plugin *p = abr_load_plugin("branch_merge_split");
    if (!p) {
        fprintf(stderr, "Plugin not found.\n");
        return 1;
    }

    Context ctx = {0};
    Flags flags = {0};

    if (p->is_branching) {
        size_t out_count = 0;

        // Correct ABI order: Flags*, Context*
        WindowSet *out = p->process_branch(p, &ws, &flags, &ctx, &out_count);

        for (size_t i = 0; i < out_count; i++) {
            printf("window[%zu]: ", i);
            print_window(&out->windows[i]);
        }

        for (size_t i = 0; i < out_count; i++) {
            free(out->windows[i].bits);
        }
        free(out->windows);
        free(out);

    } else {
        // Correct ABI order: Flags*, Context*
        WindowSet out = p->process_set(p, &ws, &flags, &ctx);

        for (size_t i = 0; i < out.count; i++) {
            printf("window[%zu]: ", i);
            print_window(&out.windows[i]);
        }

        for (size_t i = 0; i < out.count; i++) {
            free(out.windows[i].bits);
        }
        free(out.windows);
    }

    for (size_t i = 0; i < ws.count; i++) {
        free(ws.windows[i].bits);
    }
    free(ws.windows);

    return 0;
}

