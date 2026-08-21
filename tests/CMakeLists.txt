cmake_minimum_required(VERSION 3.16)

enable_testing()

include_directories(..)

add_executable(core_tests core_tests.c)
target_link_libraries(core_tests abr_core abr_sdk abr_pipeline abr_vm abr_system)
add_test(NAME core_tests COMMAND core_tests)

add_executable(plugin_tests plugin_tests.c)
target_link_libraries(plugin_tests abr_core abr_sdk abr_pipeline abr_vm abr_system)
add_test(NAME plugin_tests COMMAND plugin_tests)

add_executable(pipeline_tests pipeline_tests.c)
target_link_libraries(pipeline_tests abr_core abr_sdk abr_pipeline abr_vm abr_system)
add_test(NAME pipeline_tests COMMAND pipeline_tests)

add_executable(vm_tests vm_tests.c)
target_link_libraries(vm_tests abr_core abr_sdk abr_pipeline abr_vm abr_system)
add_test(NAME vm_tests COMMAND vm_tests)

add_executable(integration_tests integration_tests.c)
target_link_libraries(integration_tests abr_core abr_sdk abr_pipeline abr_vm abr_system)
add_test(NAME integration_tests COMMAND integration_tests)

add_executable(minimal_example minimal_example.c)
target_link_libraries(minimal_example abr_core abr_sdk abr_pipeline abr_vm abr_system)
add_test(NAME minimal_example COMMAND minimal_example)

target_include_directories(abr_pipeline PUBLIC plugins)
target_include_directories(abr_vm PUBLIC plugins)
target_include_directories(abr_system PUBLIC plugins)

target_link_libraries(minimal_example
        abr_core abr_sdk abr_pipeline abr_vm abr_system abr_plugins
)


