cmake_minimum_required(VERSION 3.20.0)
project(noobnet)

include(cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -lpthread -Wno-deprecated-declarations")
  
include_directories(.)
include_directories(/usr/local/include)
include_directories(/usr/local/lib)

find_library(YAMLCPP yaml-cpp)
find_library(PTHREAD pthread)
message("***", ${YAMLCPP})

set(LIB_SRC
    net/log.cc
    net/config.cc
    net/thread.cc
    net/utils.cc
    net/mutex.cc
    )

add_library(noobnet SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(noobnet) #__FILE__

set(LIBS
        noobnet
        pthread
        yaml-cpp)

message("***", ${LIBS})

add_executable(test tests/test.cc)
add_dependencies(test noobnet)
force_redefine_file_macro_for_sources(test) #__FILE__
target_link_libraries(test noobnet ${LIBS})

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config noobnet)
force_redefine_file_macro_for_sources(test_config) #__FILE__
target_link_libraries(test_config noobnet ${LIBS})

add_executable(test_thread tests/test_thread.cc)
add_dependencies(test_thread noobnet)
force_redefine_file_macro_for_sources(test_thread) #__FILE__
target_link_libraries(test_thread noobnet ${LIBS})

add_executable(test_fiber tests/test_fiber.cc)
add_dependencies(test_fiber noobnet)
force_redefine_file_macro_for_sources(test_fiber) #__FILE__
target_link_libraries(test_fiber noobnet ${LIBS})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)