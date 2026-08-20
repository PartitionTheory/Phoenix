#include "pipeline.h"

int abr_pipeline_init(Pipeline *pl, const Flags *flags)
{
    if (!pl) return -1;

    for (size_t i = 0; i < pl->count; ++i) {
        Plugin *p = pl->plugins[i];
        if (p && p->init) {
            int rc = p->init(p, flags);
            if (rc != 0) {
                return rc;
            }
        }
    }
    return 0;
}
