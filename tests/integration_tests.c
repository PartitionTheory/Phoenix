#include <stdio.h>
#include <stdlib.h>

#include "abr_system.h"
#include "bitstream.h"
#include "widthset.h"
#include "pipeline.h"
#include "vm_program.h"
#include "vm_plugin.h"
#include "plugin.h"

/* Dummy plugin */
static WindowSet dummy_process(
    Plugin *self, const WindowSet *in, const Flags *flags, Context *ctx)
{
    printf("integration_tests: plugin exec\n");
    return *in;
}

int main(void)
{
    /* core input */
    uint8_t bits[4] = {1,0,1,0};
    Bitstream S = { bits, 4 };

    size_t widths_arr[2] = {2, 2};
    WidthSet G = { widths_arr, 2 };

    /* plugin */
    Plugin *p = calloc(1, sizeof(Plugin));
    p->process_set = dummy_process;

    /* pipeline */
    Pipeline pl;
    pl.plugins = calloc(1, sizeof(Plugin *));
    pl.plugins[0] = p;
    pl.count = 1;

    /* VM */
    Instruction *I = abr_instruction_from_plugin(p);
    VMProgram prog;
    prog.instructions = calloc(1, sizeof(Instruction *));
    prog.instructions[0] = I;
    prog.count = 1;

    /* system */
    Flags flags = {0};
    ABRSystem sys;
    abr_system_init(&sys, &pl, &prog, &flags);

    WindowSet out = abr_system_run(&sys, &S, &G);

    printf("integration_tests: ok\n");
    return 0;
}
