#include <stdio.h>
#include <stdlib.h>

#include "abr_system.h"
#include "bitstream.h"
#include "widthset.h"

#include "increment_plugin.h"
#include "double_instruction.h"

int main(void)
{
    /* Bitstream: 10110010 */
    uint8_t bits[8] = {1,0,1,1,0,0,1,0};
    Bitstream S = { bits, 8 };

    /* Widths: [3, 2, 3] */
    size_t widths_arr[3] = {3, 2, 3};
    WidthSet G = { widths_arr, 3 };

    /* Pipeline: increment plugin */
    Plugin *p_inc = make_increment_plugin();

    Pipeline pl;
    pl.plugins = calloc(1, sizeof(Plugin *));
    pl.plugins[0] = p_inc;
    pl.count = 1;

    /* VM Program: double instruction */
    Instruction *I_double = make_double_instruction();

    VMProgram prog;
    prog.instructions = calloc(1, sizeof(Instruction *));
    prog.instructions[0] = I_double;
    prog.count = 1;

    /* System */
    Flags flags = {0};
    ABRSystem sys;
    abr_system_init(&sys, &pl, &prog, &flags);

    /* Run ABR */
    WindowSet out = abr_system_run(&sys, &S, &G);

    /* Print result */
    printf("\nFinal windows:\n");
    for (size_t i = 0; i < out.count; ++i) {
        printf("  window[%zu] = %u\n", i, out.windows[i].value);
    }

    return 0;
}
