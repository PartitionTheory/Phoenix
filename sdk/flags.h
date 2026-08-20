#ifndef ABR_FLAGS_H
#define ABR_FLAGS_H

#include <stddef.h>
#include <stdint.h>

/* Simple key/value flags for runtime configuration */
typedef struct {
    const char *key;
    const char *value;
} AbrFlagKV;

typedef struct {
    AbrFlagKV *items;
    size_t     count;
} Flags;

/* Optional helper to get a flag by key (returns NULL if not found) */
const char *abr_flags_get(const Flags *flags, const char *key);

#endif /* ABR_FLAGS_H */
