add_library(abr_core
        core/bitstream.h
        core/window.h
        core/windowset.h
        core/widthset.h
        core/core_extract.h        # <-- add this
        core/core_extract.c
        core/core_slice.c
        core/core_dummy.c
)
