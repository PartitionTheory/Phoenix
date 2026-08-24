#ifndef ABR_STREAM_H
#define ABR_STREAM_H

#include <stddef.h>
#include <stdint.h>
#include "bigint.h"
#include "bigint_list.h"
#include "window_extract.h"
#include "reconstruct.h"
#include "abr_config.h"

/*
 * ABR Stream API (Phoenix Edition)
 *
 * This header defines the streaming interface used by:
 *   - CLI
 *   - pipeline
 *   - system run modes
 *   - reconstruction logic
 *
 * The stream is conceptually:
 *
 *     BigInt → windows → pipeline → reconstruction → output BigInt
 *
 * Math / Engineering view:
 *   A stream is a morphism between two BigInt objects, mediated by
 *   a window decomposition and a reconstruction accumulator.
 *
 *   Formally:
 *
 *       S : BigInt → BigInt
 *
 *   where S is composed of:
 *
 *       extract ∘ pipeline ∘ reconstruct
 *
 *   Each stage is pure and parameterized by AbrConfig.
 */

/* -----------------------------------------------------------
 * Stream context
 * -----------------------------------------------------------
 *
 * This object holds:
 *   - input BigInt
 *   - extracted windows
 *   - reconstruction accumulator
 *   - configuration
 *
 * Math view:
 *   The stream context is the tuple:
 *
 *       (x, W, A, C)
 *
 *   where:
 *     x = input BigInt
 *     W = list of windows
 *     A = reconstruction accumulator
 *     C = configuration
 */
typedef struct {
    const BigInt     *input;
    BigIntList        windows;
    ReconstructAcc    acc;
    AbrConfig         config;
} AbrStream;

/* -----------------------------------------------------------
 * Stream initialization
 * -----------------------------------------------------------
 *
 * Initializes:
 *   - window list
 *   - reconstruction accumulator
 *   - configuration
 *
 * Math view:
 *   Initializes the tuple (x, W, A, C) with W = ∅ and A = empty accumulator.
 */
void abr_stream_init(AbrStream *s, const BigInt *input, const AbrConfig *cfg);

/* -----------------------------------------------------------
 * Stream execution
 * -----------------------------------------------------------
 *
 * Performs:
 *   1. window_extract_all()
 *   2. pipeline execution (later steps)
 *   3. reconstruction
 *
 * Math view:
 *   Computes:
 *
 *       S(x) = reconstruct(pipeline(extract(x)))
 *
 *   All three stages are pure functions.
 */
int abr_stream_run(AbrStream *s, BigInt *output);

/* -----------------------------------------------------------
 * Stream cleanup
 * -----------------------------------------------------------
 *
 * Frees:
 *   - window list
 *   - reconstruction accumulator
 *
 * Math view:
 *   Clears W and A from the tuple (x, W, A, C).
 */
void abr_stream_free(AbrStream *s);

#endif /* ABR_STREAM_H */
