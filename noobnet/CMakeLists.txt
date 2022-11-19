cmake_minimum_required(VERSION 3.20.0)
project(noobnet)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")
  
include_directories(.)
include_directories(/usr/local/include)
include_directories(/usr/local/lib)

find_library(YAMLCPP yaml-cpp)
message("***", ${YAMLCPP})

set(LIB_SRC
    net/log.cc
    net/config.cc
    )

add_library(noobnet SHARED ${LIB_SRC})


add_executable(test tests/test.cc)
add_dependencies(test noobnet)
target_link_libraries(test noobnet ${YAMLCPP})

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config noobnet)
target_link_libraries(test_config noobnet ${YAMLCPP})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)