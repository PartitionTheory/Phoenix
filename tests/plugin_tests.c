#include <stdio.h>
#include "plugin.h"
#include "windowset.h"
#include "flags.h"
#include "context.h"

/* Dummy plugin for test */
static WindowSet dummy_process(
    Plugin *self, const WindowSet *in, const Flags *flags, Context *ctx)
{
    printf("plugin_tests: dummy_process called\n");
    return *in;
}

int main(void)
{
    Plugin p = {0};
    p.process_set = dummy_process;

    WindowSet ws = {0};
    Flags flags = {0};
    Context ctx = {0};

    WindowSet out = p.process_set(&p, &ws, &flags, &ctx);

    printf("plugin_tests: ok\n");
    return 0;
}
