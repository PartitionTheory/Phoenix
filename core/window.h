#ifndef ABR_WINDOW_H
#define ABR_WINDOW_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *bits;   /* window contents */
    size_t   width;  /* requested width (may exceed actual length if overflow) */
    size_t   length; /* actual number of bits in this window */
} Window;

#endif /* ABR_WINDOW_H */
