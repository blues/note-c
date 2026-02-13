
target_compile_options(
    note_c
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wno-old-style-definition
)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    target_compile_options(note_c PUBLIC -m32 -mfpmath=sse -msse2)
    target_link_directories(note_c PUBLIC /lib32 /usr/lib32 /usr/lib/gcc/x86_64-linux-gnu/12/32)
    target_link_options(note_c PUBLIC -m32 -Wl,-melf_i386)
endif()

if(CMAKE_BUILD_TYPE)
    string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_TOLOWER)

    if(CMAKE_BUILD_TYPE_TOLOWER STREQUAL "debug")
        target_link_options(note_c PUBLIC -Og -ggdb)
    endif()
endif()
