add_library(warning_flags INTERFACE)
include(CompilerWarnings)
target_add_warning_flags(warning_flags TREAT_AS_ERRORS)


bragi_add_component(threadsafety ENABLE true LEVEL 0)
add_executable(threadSafety ThreadSafety.cpp)
target_link_libraries(threadSafety bragi_config pthread warning_flags)

bragi_add_component(benchmark)
bragi_set_options(benchmark ENABLE true LEVEL eval)
add_executable(benchmark Benchmark.cpp)
target_link_libraries(benchmark bragi_config pthread warning_flags)

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/_sandbox.cpp)
  bragi_add_component(sandbox)
  add_executable(sandbox _sandbox.cpp)
  target_link_libraries(sandbox bragi_config warning_flags)
endif()
