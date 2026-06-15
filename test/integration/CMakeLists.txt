cmake_minimum_required(VERSION 3.21)
project(note_c_integration_test LANGUAGES C)

# Pull in note-c as a subdirectory, the way a downstream project would.
# NOTE_C_BUILD_TESTS defaults to OFF because we are not the top-level project.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../.. note-c)

add_executable(link_test link_test.c)
target_link_libraries(link_test PRIVATE note_c_lib)

enable_testing()
add_test(NAME link_test COMMAND link_test)
