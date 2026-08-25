#
# A: Mathematical description
#    Plugins form a family of transforms T_i : WindowSet → WindowSet.
#
# B: Engineering description
#    Build all plugin .c files into a single static library.
#

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(abr_plugins
        increment_plugin.c
        flip_stretch.c
        branch_merge_split.c
        abr_plugin_registry.c
)



