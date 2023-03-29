cmake_minimum_required (VERSION 3.5)
project (gtest_lesson)
set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -mavx2 -pthread -g -ggdb -O0")
aux_source_directory(. SRC_FILES)
add_executable (test_main ${SRC_FILES})
target_link_libraries (test_main 
  libgtest.a
  libgmock.a # 仅在需要mock测试时引入
)

