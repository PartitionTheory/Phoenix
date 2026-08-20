#ifndef ABR_WIDTHSET_H
#define ABR_WIDTHSET_H

#include <stddef.h>

typedef struct {
    size_t *widths;
    size_t  count;
} WidthSet;

#endif /* ABR_WIDTHSET_H */
