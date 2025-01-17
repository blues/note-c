# Testing

This directory contains note-c unit tests written in C++ using Catch2. The
[Fake Function Framework](https://github.com/meekrosoft/fff) (fff) is used for
mocking and is bundled with the test code (see fff.h).

## Dependencies

- [CMake](https://cmake.org/install), version 3.20+.
- [Catch2](https://github.com/catchorg/Catch2), v3.
  - CMake will attempt to download and build Catch2 from source.
  - We require C++14, which is the minimum expected by Catch2 v3.

## Running the Tests

```sh
mkdir build
cd build
cmake ..
make -j
ctest
```

Alternatively, scripts/run_unit_tests.sh will do all these steps for you. This
script is also used by our CI pipeline (see the Dockerfile in
.github/actions/run_unit_tests).

### Notes

- We forbid in-source builds, so creating a build directory (e.g. `mkdir build`)
and building in there is mandatory.

## CMake Options

- COVERAGE: Adds the target `coverage` to the build. Requires lcov. To generate
a coverage report, run `make coverage` (Default: OFF).
- LOW_MEM: Runs the unit-tests over code compiled with the `NOTE_C_LOW_MEM`
flag enabled. This flag modifies the behavior of the library to perform better
on low memory hosts (Default: OFF).
- MEM_CHECK: Runs the unit-tests under `valgrind` to test for memory leaks
(Default: OFF).
- SHOW_MALLOC: Runs the unit-tests over code compiled with the
`NOTE_C_SHOW_MALLOC` flag enabled. This flag causes the host to log with each
memory allocation and free (Default: OFF).
- SINGLE_PRECISION: Runs the unit-tests over code compiled with the
`NOTE_C_SINGLE_PRECISION` flag enabled. All doubles are treated as single
precision, which is commonplace on many MCUs (Default: OFF).
- VERBOSE: Increase the verbosity of the build system (Default: OFF).

## Development

For the most part, each function in the API gets its own test executable in
test/src. The naming convention is FunctionName_test.cpp. Some functions make
sense to test together, like the NotePayload* family, so they're exercised in
one executable (NotePayload_test.cpp, in this case). Any functions that are
mocked need to be added to the list of mocked functions in the root
CMakeLists.txt (see MOCKED_FNS). Once written, the test needs to be added to
test/CMakeLists.txt with a call to the `add_test` macro.

### Generating a Coverage Report

Assuming you ran cmake with coverage enabled (`-DCOVERAGE=1`), ran `make
coverage`, and are in the build directory, you can view the coverage report as
HTML with:

```sh
cd build/test/coverage
genhtml lcov.info -o tmp
```

This will generate HTML under test/coverage/tmp. Open index.html in your
browser to view the report.

### Style

#### `CHECK` vs. `REQUIRE`

[Catch2 offers 2 primary assertion macros]
(https://github.com/catchorg/Catch2/blob/devel/docs/assertions.md): `REQUIRE`
and `CHECK`. We prefer `CHECK` anywhere you need to assert something in a test
case. However, if it doesn't make sense or it would cause a fault to continue a
test case, `REQUIRE` should be used to exit the case immediately on failure.
Example:

```c
REQUIRE(sizeof(expectedBuf) == actualLength);
CHECK(!memcmp(expectedBuf, actualBuf, actualLength));
```

Here we don't want to continue if the lengths of the two buffers differ, as the
`memcmp` may segfault.
