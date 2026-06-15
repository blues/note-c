cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0095 NEW)

if ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
    message(FATAL_ERROR "In-source builds are not allowed.
    Please create a build directory and use `cmake ..` inside it.
    NOTE: cmake will now create CMakeCache.txt and CMakeFiles/*.
          You must delete them, or cmake will refuse to work.")
endif()

project(note_c)

# Automatically ignore CMake build directory.
if(NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
    file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif()

option(NOTE_C_BUILD_DOCS "Build docs." OFF)
option(NOTE_C_BUILD_TESTS "Build tests." ${PROJECT_IS_TOP_LEVEL})
option(NOTE_C_COVERAGE "Compile for test NOTE_C_COVERAGE reporting." OFF)
option(NOTE_C_LOW_MEM "Build the library tailored for low memory usage." OFF)
option(NOTE_C_MEM_CHECK "Run tests with Valgrind." OFF)
option(NOTE_NODEBUG "Build the library without debug information." OFF)
option(NOTE_C_NO_LIBC "Audit libc dependencies by linking the note_c test/audit library without libc, generating errors for any undefined symbols." OFF)
option(NOTE_C_SHOW_MALLOC "Build the library with flags required to log memory usage." OFF)
option(NOTE_C_SINGLE_PRECISION "Use single precision for JSON floating point numbers." OFF)
option(NOTE_C_HEARTBEAT_CALLBACK "Enable heartbeat callback support." OFF)

# NOTE_C_NO_LIBC is a link-time undefined-symbol audit (see
# scripts/check_libc_dependencies.sh). It only has any effect on the shared
# `note_c` test/audit target, which is built when NOTE_C_BUILD_TESTS is ON. The
# static `note_c_lib` is archived with `ar` (no link step), so the flag would
# silently do nothing there. Fail loudly rather than appear to audit when it
# can't.
if(NOTE_C_NO_LIBC AND NOT NOTE_C_BUILD_TESTS)
    message(FATAL_ERROR
        "NOTE_C_NO_LIBC is only meaningful for the note_c shared test/audit "
        "target. Enable NOTE_C_BUILD_TESTS, or leave NOTE_C_NO_LIBC disabled "
        "when consuming note_c_lib.")
endif()

set(NOTE_C_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Detect whether the system already provides strlcpy/strlcat (macOS, *BSD).
# When present, we skip the bundled implementations in n_str.c and define
# guards so note.h doesn't redeclare them (which collides with the platform's
# fortified macros).
include(CheckSymbolExists)
check_symbol_exists(strlcpy "string.h" HAVE_STRLCPY)
check_symbol_exists(strlcat "string.h" HAVE_STRLCAT)

set(NOTE_C_SOURCES
    ${NOTE_C_SRC_DIR}/n_atof.c
    ${NOTE_C_SRC_DIR}/n_b64.c
    ${NOTE_C_SRC_DIR}/n_cjson.c
    ${NOTE_C_SRC_DIR}/n_cjson_helpers.c
    ${NOTE_C_SRC_DIR}/n_cobs.c
    ${NOTE_C_SRC_DIR}/n_const.c
    ${NOTE_C_SRC_DIR}/n_ftoa.c
    ${NOTE_C_SRC_DIR}/n_helpers.c
    ${NOTE_C_SRC_DIR}/n_hooks.c
    ${NOTE_C_SRC_DIR}/n_i2c.c
    ${NOTE_C_SRC_DIR}/n_md5.c
    ${NOTE_C_SRC_DIR}/n_printf.c
    ${NOTE_C_SRC_DIR}/n_request.c
    ${NOTE_C_SRC_DIR}/n_serial.c
)
# n_str.c provides weak strlcpy/strlcat. On systems that already have them
# the file won't compile because the platform headers define these as
# fortified macros that conflict with the function definitions.
if(NOT HAVE_STRLCPY OR NOT HAVE_STRLCAT)
    list(APPEND NOTE_C_SOURCES ${NOTE_C_SRC_DIR}/n_str.c)
endif()

# Apply the include path and compile-definition configuration shared by every
# note-c library target. Build- and test-specific settings (warnings, 32-bit
# cross-compile, FFF interposition, NOTE_C_TEST, NOTE_C_NO_LIBC) are applied at
# each target's own definition site, not here.
function(note_c_configure_common target)
    target_include_directories(${target} PUBLIC ${NOTE_C_SRC_DIR})

    if(HAVE_STRLCPY)
        target_compile_definitions(${target} PUBLIC HAVE_STRLCPY)
    endif()
    if(HAVE_STRLCAT)
        target_compile_definitions(${target} PUBLIC HAVE_STRLCAT)
    endif()

    if(NOTE_C_LOW_MEM)
        target_compile_definitions(${target} PUBLIC NOTE_C_LOW_MEM)
    else()
        # n_ua.c is empty when NOTE_C_LOW_MEM is defined, which leads to an
        # empty-translation-unit warning, so only compile it otherwise.
        target_sources(${target} PRIVATE ${NOTE_C_SRC_DIR}/n_ua.c)
    endif()

    if(NOTE_NODEBUG)
        target_compile_definitions(${target} PUBLIC NOTE_NODEBUG)
    endif()
    if(NOTE_C_SHOW_MALLOC)
        target_compile_definitions(${target} PUBLIC NOTE_C_SHOW_MALLOC)
    endif()
    if(NOTE_C_SINGLE_PRECISION)
        target_compile_definitions(${target} PUBLIC NOTE_C_SINGLE_PRECISION)
    endif()
    if(NOTE_C_HEARTBEAT_CALLBACK)
        target_compile_definitions(${target} PUBLIC NOTE_C_HEARTBEAT_CALLBACK)
    endif()
endfunction()

# ---------------------------------------------------------------------------
# note_c_lib — static library for downstream consumers.
#
# Projects that include note-c as a submodule can simply do:
#
#   add_subdirectory(note-c)
#   target_link_libraries(my_app PRIVATE note_c_lib)
#
# This target carries the include path and platform compile definitions
# (HAVE_STRLCPY, etc.) but no test flags, 32-bit cross-compile options,
# or other CI-specific settings.
# ---------------------------------------------------------------------------
add_library(note_c_lib STATIC ${NOTE_C_SOURCES})
note_c_configure_common(note_c_lib)

# NOTE_C_NO_LIBC is intentionally NOT applied to note_c_lib: it is a static
# archive, so target_link_options has no effect (no link step), and the audit
# is performed against the shared note_c target below.

# ---------------------------------------------------------------------------
# note_c — shared library used by the unit test harness.
#
# This target adds -Werror, 32-bit cross-compile flags (Linux CI), FFF
# interposition support (macOS), and the NOTE_C_TEST define. It is NOT
# intended for downstream consumption.
# ---------------------------------------------------------------------------
if(NOTE_C_BUILD_TESTS)
    add_library(note_c SHARED ${NOTE_C_SOURCES})
    note_c_configure_common(note_c)

    target_compile_options(
        note_c
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
            -Og
            -ggdb
    )
    # The Linux CI runs tests in 32-bit mode. These flags are x86-specific and
    # unavailable on Apple toolchains or non-x86 architectures.
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|i[3-6]86")
        target_compile_options(note_c PUBLIC -m32 -mfpmath=sse -msse2)
    endif()
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|i[3-6]86")
        target_link_directories(
            note_c
            PUBLIC
                /lib32
                /usr/lib32
                /usr/lib/gcc/x86_64-linux-gnu/12/32
        )
        target_link_options(
            note_c
            PUBLIC
                -m32
                -Wl,-melf_i386
        )
    endif()
    if(APPLE)
        # Use flat namespace so test fakes (FFF) can interpose library symbols
        target_link_options(note_c PRIVATE -Wl,-flat_namespace)
    endif()

    # The libc dependency audit (scripts/check_libc_dependencies.sh) links this
    # shared target without libc so the linker reports undefined symbols. This
    # only works on a target that is actually linked, hence note_c, not the
    # static note_c_lib.
    if(NOTE_C_NO_LIBC)
        target_link_options(
            note_c
            PRIVATE
                -nostdlib
                -nodefaultlibs
                LINKER:--no-undefined
        )
    endif()

    # Including CTest here rather than in test/CMakeLists.txt allows us to run
    # ctest from the root build directory (e.g. build/ instead of build/test/).
    # We also need to set MEMORYCHECK_COMMAND_OPTIONS before including this.
    # See: https://stackoverflow.com/a/60741757
    if(NOTE_C_MEM_CHECK)
        # Go ahead and make sure we can find valgrind while we're here.
        find_program(VALGRIND valgrind REQUIRED)
        message(STATUS "Found valgrind: ${VALGRIND}")
        set(MEMORYCHECK_COMMAND_OPTIONS "--leak-check=full --error-exitcode=1")
    endif(NOTE_C_MEM_CHECK)
    include(CTest)

    target_compile_definitions(
        note_c
        PUBLIC
            NOTE_C_TEST
    )
    target_include_directories(
        note_c
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/test/include
    )

    add_subdirectory(test)
endif(NOTE_C_BUILD_TESTS)

if(NOTE_C_BUILD_DOCS)
    add_subdirectory(docs)
endif(NOTE_C_BUILD_DOCS)
