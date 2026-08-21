#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abr_system.h"
#include "bitstream.h"
#include "widthset.h"

#include "increment_plugin.h"
#include "double_instruction.h"
#include "instruction_registry.h"


static uint8_t *load_file_bits(const char *path, size_t *out_len) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "Cannot open file: %s\n", path);
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);

    uint8_t *buf = malloc(sz);
    fread(buf, 1, sz, f);
    fclose(f);

    *out_len = sz;
    return buf;
}

static size_t *parse_widths(const char *arg, size_t *count) {
    char *tmp = strdup(arg);
    size_t cap = 16;
    size_t *arr = malloc(cap * sizeof(size_t));
    *count = 0;

    char *tok = strtok(tmp, ",");
    while (tok) {
        if (*count == cap) {
            cap *= 2;
            arr = realloc(arr, cap * sizeof(size_t));
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
        printf("  abr run <file> --widths 3,2,3 --pipeline inc --vm double\n");
        return 1;
    }

    const char *file = argv[2];
    const char *widths_arg = NULL;
    const char *pipeline_arg = NULL;
    const char *vm_arg = NULL;

    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "--widths") == 0) {
            widths_arg = argv[++i];
        } else if (strcmp(argv[i], "--pipeline") == 0) {
            pipeline_arg = argv[++i];
        } else if (strcmp(argv[i], "--vm") == 0) {
            vm_arg = argv[++i];
        }
    }

    if (!widths_arg || !pipeline_arg || !vm_arg) {
        fprintf(stderr, "Missing required arguments.\n");
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

    /* Build pipeline */
    Pipeline pl;
    pl.plugins = calloc(1, sizeof(Plugin *));
    pl.count = 1;

    if (strcmp(pipeline_arg, "inc") == 0) {
        pl.plugins[0] = make_increment_plugin();
    } else {
        fprintf(stderr, "Unknown pipeline plugin: %s\n", pipeline_arg);
        return 1;
    }



    /* ============================================================================
     *  VM Instruction Lookup (Registry-based)
     *
     *  A. Formal Mathematical Annotation
     *  ---------------------------------
     *  Let 𝕀 be the set of all VM instructions registered in the ABR VM.
     *  Each instruction is a morphism:
     *
     *      f : WindowSet → WindowSet
     *
     *  The registry is a partial function:
     *
     *      R : String → 𝕀
     *
     *  such that:
     *      R(name) = f        if f is registered under identifier `name`
     *      R(name) = ⊥        if no such instruction exists
     *
     *  Domain: ASCII strings (instruction names)
     *  Codomain: Instruction* (factory-produced morphisms)
     *
     *  Invariant:
     *      If R(name) ≠ ⊥, then f is total and pure:
     *          ∀W, f(W) is defined and depends only on W.
     *
     *  Engineering Notes
     *  -----------------
     *  - Registry lookup replaces manual strcmp-based instruction selection.
     *  - This enables dynamic VM program construction from CLI arguments.
     *  - Registry entries are defined in vm/instruction_registry.c.
     *  - Memory ownership: returned Instruction* belongs to the VM program.
     *  - Error handling: unknown instruction names produce a diagnostic list.
     *
     *  B. Semi-formal Math Summary
     *  ---------------------------
     *  We map a string to a pure function f: WindowSet → WindowSet.
     *  If the name is unknown, we reject the CLI input.
     * ============================================================================
     */

    VMProgram prog;
    prog.count = 1;
    prog.instructions = calloc(1, sizeof(Instruction *));

    /* Lookup instruction by name */
    Instruction *I = instruction_registry_lookup(vm_arg);

    if (!I) {
        fprintf(stderr, "Unknown VM instruction: %s\n", vm_arg);
        instruction_registry_list();   /* show available instructions */
        return 1;
    }

    prog.instructions[0] = I;


    /* System */
    Flags flags = {0};
    ABRSystem sys;
    abr_system_init(&sys, &pl, &prog, &flags);

    /* Run */
    WindowSet out = abr_system_run(&sys, &S, &G);

    /* Print */
    printf("\nFinal windows:\n");
    for (size_t i = 0; i < out.count; ++i) {
        printf("  window[%zu] (width=%zu, length=%zu): ",
               i,
               out.windows[i].width,
               out.windows[i].length);

        for (size_t b = 0; b < out.windows[i].length; ++b) {
            printf("%u", out.windows[i].bits[b]);
        }
        printf("\n");
    }

    return 0;
}
