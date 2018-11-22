cmake_minimum_required(VERSION 2.8)
project(udp-reader)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(READER_NAME "udp_reader")

include_directories(include)

find_package(Boost COMPONENTS unit_test_framework)

add_library(${READER_NAME} SHARED "src/ReaderBase.cpp")

# Unit tests
if (Boost_FOUND)
    ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)
    add_executable(test-reader "test/test_ReaderBase.cpp")
    target_include_directories(test-reader PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(test-reader ${Boost_LIBRARIES} ${READER_NAME})
    add_test(NAME test-1 COMMAND test-reader)
endif ()

# Install
set_target_properties(${READER_NAME} PROPERTIES PUBLIC_HEADER "include/ReaderBase.hpp")
install(
    TARGETS ${READER_NAME}
    EXPORT "udp_reader-export"
    LIBRARY DESTINATION "lib/${READER_NAME}"
    ARCHIVE DESTINATION "lib/${READER_NAME}"
    PUBLIC_HEADER DESTINATION "include/${READER_NAME}"
)
install(
    EXPORT "udp_reader-export"
    FILE "udp_reader-config.cmake"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/udp_reader/cmake"
)
