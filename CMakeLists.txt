cmake_minimum_required(VERSION 2.8)
project(udp-reader)

macro(use_cxx11)
    if (CMAKE_VERSION VERSION_LESS "3.1")
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
        else ()
            message (FATAL_ERROR "Could not set the compilter standard to C++11.")
        endif ()
    else ()
        set(CMAKE_CXX_STANDARD 11)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_EXTENSIONS OFF)
    endif ()
endmacro(use_cxx11)
use_cxx11()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

include_directories(include)

find_package(Boost COMPONENTS unit_test_framework)

add_library(udp-reader SHARED "src/ReaderBase.cpp")

# Unit tests
if (Boost_FOUND)
    ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)
    add_executable(test-reader "test/test_ReaderBase.cpp")
    target_include_directories(test-reader PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(test-reader ${Boost_LIBRARIES} udp-reader)
    add_test(NAME test-1 COMMAND test-reader)
endif ()

# Install
set_target_properties(udp-reader PROPERTIES PUBLIC_HEADER "include/ReaderBase.hpp")
install(
    TARGETS udp-reader
    LIBRARY DESTINATION "lib/"
    PUBLIC_HEADER DESTINATION "include/udp-reader"
)
