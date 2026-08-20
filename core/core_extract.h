#ifndef ABR_CORE_EXTRACT_H
#define ABR_CORE_EXTRACT_H

#include "bitstream.h"
#include "windowset.h"
#include "widthset.h"

WindowSet abr_extract(const Bitstream *S, const WidthSet *G);

#endif
