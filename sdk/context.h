#ifndef ABR_CONTEXT_H
#define ABR_CONTEXT_H

#include <stddef.h>
#include <stdint.h>

/* Execution context for plugins and VM */
typedef struct {
    int   branch_id;     /* branch identity (0 for main) */
    int   error_code;    /* 0 = OK, non-zero = error */
    void *user_data;     /* optional user-defined pointer */

    /* Reserved for future: reversible metadata, history, etc. */
} Context;

#endif /* ABR_CONTEXT_H */
