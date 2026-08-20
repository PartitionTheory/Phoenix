#ifndef ABR_WINDOWSET_H
#define ABR_WINDOWSET_H

#include "window.h"
#include <stddef.h>

typedef struct {
    Window *windows;
    size_t  count;
} WindowSet;

#endif /* ABR_WINDOWSET_H */
