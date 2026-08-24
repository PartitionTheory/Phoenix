#ifndef WINDOWSET_H
#define WINDOWSET_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *bits;
    size_t width;
    size_t length;
} Window;

typedef struct {
    Window *windows;
    size_t count;
} WindowSet;

#endif

