#include <stdio.h>
#include <stdlib.h>
#include "vm_program.h"
#include "vm_plugin.h"
#include "plugin.h"
#include "windowset.h"
#include "flags.h"
#include "context.h"

/* Dummy plugin */
static WindowSet dummy_process(
    Plugin *self, const WindowSet *in, const Flags *flags, Context *ctx)
{
    printf("vm_tests: plugin exec\n");
    return *in;
}

int main(void)
{
    Plugin *p = calloc(1, sizeof(Plugin));
    p->process_set = dummy_process;

    Instruction *I = abr_instruction_from_plugin(p);

    VMProgram prog;
    prog.instructions = calloc(1, sizeof(Instruction *));
    prog.instructions[0] = I;
    prog.count = 1;

    WindowSet ws = {0};
    Flags flags = {0};
    Context ctx = {0};

    WindowSet out = abr_vm_exec(&prog, &ws, &flags, &ctx);

    printf("vm_tests: ok\n");
    return 0;
}
