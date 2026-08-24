#ifndef INCREMENT_PLUGIN_H
#define INCREMENT_PLUGIN_H

#include "abr_plugin.h"

/*
A: Mathematical description
   increment_plugin performs the identity transform:
   f(W) = W
   (This plugin is a placeholder used for testing ABI correctness.)

B: Engineering description
   The plugin simply returns the input WindowSet unchanged.
   No branching, no reversible transform.
*/

Plugin *make_increment_plugin(void);

#endif

