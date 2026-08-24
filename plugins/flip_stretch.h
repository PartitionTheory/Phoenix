#ifndef FLIP_STRETCH_H
#define FLIP_STRETCH_H

#include "abr_plugin.h"

/*
A: Mathematical description
   flip_stretch performs a bitwise NOT:
   For each bit b in window W: f(b) = 1 - b.

B: Engineering description
   This plugin mutates the bits in-place (engine-owned memory).
   It is non-branching and non-reversible.
*/

Plugin *make_flip_stretch_plugin(void);

#endif

