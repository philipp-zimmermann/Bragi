marsLogging_add_component(threadsafety ENABLE true LEVEL 0)
add_executable(threadSafety ThreadSafety.cpp)
target_link_libraries(threadSafety marsLogging_config pthread)

marsLogging_add_component(benchmark)
marsLogging_set_options(benchmark ENABLE true LEVEL eval)
add_executable(benchmark Benchmark.cpp)
target_link_libraries(benchmark marsLogging_config pthread)

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/_sandbox.cpp)
  marsLogging_add_component(sandbox)
  add_executable(sandbox _sandbox.cpp)
  target_link_libraries(sandbox marsLogging_config)
endif()
