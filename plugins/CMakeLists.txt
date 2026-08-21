add_library(abr_plugins
        increment_plugin.c
)

target_include_directories(abr_plugins PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(abr_plugins PUBLIC abr_core abr_sdk)
