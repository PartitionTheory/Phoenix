#include <stdlib.h>
#include <string.h>
#include "abr_stream.h"
#include "window_extract.h"
#include "reconstruct.h"

/*
 * ABR Stream (Phoenix Edition)
 *
 * Implements:
 *   abr_stream_init()
 *   abr_stream_run()
 *   abr_stream_free()
 *
 * Math / Engineering view:
 *   The stream is the composite morphism:
 *
 *       S = R ∘ P ∘ E
 *
 *   where:
 *       E = window_extract_all
 *       P = pipeline (stub for now)
 *       R = reconstruction
 *
 *   This file implements the orchestration of these stages.
 */

/* -----------------------------------------------------------
 * Initialization
 * -----------------------------------------------------------
 */
void abr_stream_init(AbrStream *s, const BigInt *input, const AbrConfig *cfg)
{
    s->input = input;
    s->config = *cfg;

    bigint_list_init(&s->windows);
    reconstruct_init(&s->acc, cfg);
}

/* -----------------------------------------------------------
 * Run stream
 * -----------------------------------------------------------
 *
 * Performs:
 *   1. Extract windows
 *   2. Run pipeline (stub)
 *   3. Reconstruct BigInt
 */
int abr_stream_run(AbrStream *s, BigInt *output)
{
    /* 1. Extract windows */
    if (window_extract_all(s->input, &s->config, &s->windows) != 0)
        return -1;

    /* 2. Pipeline stage (Phoenix stub)
     *
     * Math view:
     *   P(W) = W   (identity pipeline for now)
     *
     * Later:
     *   - pipeline_linear
     *   - pipeline_reverse
     *   - pipeline_branching
     *   - pipeline_init
     */
    /* No-op pipeline */

    /* 3. Reconstruction */
    for (size_t i = 0; i < s->windows.count; i++) {
        BitWindow w;
        bitwindow_init(&w, s->config.bitwidth.window_bits);

        /* Convert BigInt → BitWindow */
        /* (We already have BigInt windows, so we wrap them) */
        w.value = s->windows.items[i];
        w.window_bits = s->config.bitwidth.window_bits;

        if (reconstruct_consume(&s->acc, &w, i) != 0) {
            bitwindow_free(&w);
            return -2;
        }

        /* Do not free w.value — owned by list */
        bitwindow_free(&w);
    }

    /* Finalize */
    if (reconstruct_finalize(&s->acc, output) != 0)
        return -3;

    return 0;
}

/* -----------------------------------------------------------
 * Free stream
 * -----------------------------------------------------------
 */
void abr_stream_free(AbrStream *s)
{
    bigint_list_free(&s->windows);
    reconstruct_free(&s->acc);
}
