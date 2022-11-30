include(FetchContent)

# Catch2 v3 requires C++14.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_CATCH "Fetch and build Catch2 from source." OFF)

macro(download_catch2)
    message(STATUS "Downloading Catch2...")
    FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG        v3.2.0
    )
    FetchContent_MakeAvailable(Catch2)
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
endmacro(download_catch2)

if(BUILD_CATCH)
    download_catch2()
else()
    find_package(Catch2 3)

    if(Catch2_FOUND)
        message(STATUS "Found Catch2: ${Catch2_CONFIG}")
    else()
        message(STATUS "Couldn't find installed Catch2.")
        download_catch2()
    endif(Catch2_FOUND)
endif(BUILD_CATCH)

include(Catch)

function(add_test TEST_NAME)
    add_executable(
        ${TEST_NAME}
        ${TEST_NAME}.cpp
    )
    target_include_directories(
        ${TEST_NAME} PRIVATE
        ${NOTE_C_SRC_DIR}
    )
    target_link_libraries(
        ${TEST_NAME}
        note_c
        Catch2::Catch2WithMain
    )

    catch_discover_tests(${TEST_NAME})
endfunction()

add_test(NoteTransaction_test)
add_test(NoteRequest_test)
