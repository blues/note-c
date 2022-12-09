# Testing

This directory contains note-c unit tests written in C++ using Catch2. The
[Fake Function Framework](https://github.com/meekrosoft/fff) (fff) is used for
mocking and is bundled with the test code (see fff.h).

## Dependencies

- [CMake](https://cmake.org/install), version 3.13+.
- [Catch2](https://github.com/catchorg/Catch2), v3.
    - If Catch2 isn't installed, we'll attempt to download and build Catch2 from
    source.
    - We require C++14, which is the minimum expected by Catch2 v3.

## Running the Tests

```sh
mkdir build
cd build
cmake ..
make -j
ctest
```

### Notes

- We forbid in-source builds, so creating a build directory (e.g. `mkdir build`)
and building in there is mandatory.

## CMake Options

- BUILD_CATCH: Download and build Catch2 from source, rather than looking for an
installed verison. Default: OFF.
