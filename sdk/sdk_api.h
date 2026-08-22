#ifndef ABR_SDK_API_H
#define ABR_SDK_API_H

#include "windowset.h"
#include "flags.h"
#include "context.h"

/*
A: Mathematical description
   The SDK layer defines the public-facing operations that plugins
   and CLI tools may rely on. At present, this is intentionally minimal.

B: Engineering description
   This header exists to provide a stable ABI boundary.
   Additional SDK functions can be added here without breaking plugins.
*/

void abr_sdk_dummy(void);

#endif

