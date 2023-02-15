##▁0▁INITIATE▁PROJECT▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
cmake_minimum_required(VERSION 3.14)
project(loggingExample)


##▁1▁GET▁THE▁BRAGI▁LIBRARY▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
if(NOT TARGET bragi_config)
  #                  ↓ the dir to the bragi CMakeLists.txt file
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/.. bragi)
  #                                             ↑ explicitly defined binary sub-directory

  # Note: As soon as bragi has the necessary functionalities this can be substuituted
  # with: `find_package(bragi)`
endif()


##▁2▁ADD▁TARGETS▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
###_the_bare_minimum:_hello_world_________________________________________________________
add_executable(hello_world bin/hello_world.cpp)
target_link_libraries(hello_world PRIVATE bragi_config)



###_a_more-comprehensive_example__________________________________________________________
# this adds the options for the component "example_comp":
bragi_add_component("example_comp" LEVEL debug ENABLE true)
bragi_set_options("example_comp" LEVEL trace ENABLE true)
# ENABLE & LEVEL needn't be passed, if not they would be defaulted LEVEL=info, ENABLE=true

add_executable(example bin/example.cpp)
# it is necessary to link against bragi_config, this defines the include directories
target_link_libraries(example PRIVATE bragi_config)
