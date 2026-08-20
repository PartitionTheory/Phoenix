#ifndef ABR_BITSTREAM_H
#define ABR_BITSTREAM_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *bits;   /* each element is 0 or 1 */
    size_t   length; /* number of bits */
} Bitstream;

#endif /* ABR_BITSTREAM_H */
