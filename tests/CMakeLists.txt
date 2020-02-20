cmake_minimum_required(VERSION 3.8)

project(compile_time_list_tests)

add_executable(${PROJECT_NAME}
  main.cpp
  core.cpp
  unfoldr.cpp
  run_time.cpp
  )

target_link_libraries(${PROJECT_NAME} CONAN_PKG::catch2)

target_include_directories(${PROJECT_NAME} PRIVATE ../include) 

add_test(
  NAME test_all
  COMMAND compile_time_list_tests)

