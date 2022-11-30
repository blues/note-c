cmake_minimum_required(VERSION 3.4)

if ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}") 
    message(FATAL_ERROR "In-source builds are disabled.
    Please create a build directory and use `cmake ..` inside it.
    NOTE: cmake will now create CMakeCache.txt and CMakeFiles/*.
          You must delete them, or cmake will refuse to work.")
endif()

project(note_c_tests)

# Automatically ignore CMake build directory.
if(NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
    file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif()

# GoogleTest requires C++11.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GTest CONFIG REQUIRED)
enable_testing()
include(GoogleTest)

get_filename_component(PARENT_DIR ../ ABSOLUTE)
add_library(
    note_c
    ${PARENT_DIR}/n_atof.c
    ${PARENT_DIR}/n_cjson.c
    ${PARENT_DIR}/n_const.c
    ${PARENT_DIR}/n_helpers.c
    ${PARENT_DIR}/n_i2c.c
    ${PARENT_DIR}/n_printf.c
    ${PARENT_DIR}/n_serial.c
    ${PARENT_DIR}/n_ua.c
    ${PARENT_DIR}/n_b64.c
    ${PARENT_DIR}/n_cjson_helpers.c
    ${PARENT_DIR}/n_ftoa.c
    ${PARENT_DIR}/n_hooks.c
    ${PARENT_DIR}/n_md5.c
    ${PARENT_DIR}/n_request.c
    ${PARENT_DIR}/n_str.c
)

function(add_test TEST_NAME)
    add_executable(
        ${TEST_NAME}
        ${TEST_NAME}.cpp
    )
    target_include_directories(
        ${TEST_NAME} PRIVATE
        ${PARENT_DIR}
    )
    target_link_libraries(
        ${TEST_NAME}
        note_c
        GTest::gtest
    )
    gtest_discover_tests(${TEST_NAME})
endfunction()

add_test(n_request_test)
add_test(n_hooks_test)
