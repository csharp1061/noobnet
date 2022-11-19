cmake_minimum_required(VERSION 3.20.0)
project(noobnet)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")
    
set(LIB_SRC
    net/log.cc
    )

add_library(noobnet SHARED ${LIB_SRC})


add_executable(test tests/test.cc)
add_dependencies(test noobnet)
target_link_libraries(test noobnet)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)