[![Coverage Status](https://coveralls.io/repos/github/blues/note-c/badge.svg?branch=master)](https://coveralls.io/github/blues/note-c?branch=master)

# note-c

The note-c C library for communicating with the
[Blues Wireless][blues] Notecard via serial or I²C.

This library allows you to control a Notecard by writing a C
or C++ program. Your program may programmatically configure Notecard and send
Notes to [Notehub.io][notehub].

This library is used by the [note-arduino library][note-arduino], which includes
it as a git subtree.

## Documentation

The documentation for this library can be found [here](https://blues.github.io/note-c/index.html).

## CMake

The CMake build system is primarily here for testing note-c on a development
machine. You can use it to generate a static or shared note-c library, but
embedded users will typically just compile all the .c source files into their
firmware image. For more on testing, see test/README.md.

### Options

- BUILD_TESTS: Build the tests. See the tests directory. Default: ON.
- BUILD_SHARED_LIBS: Build the note-c library as shared instead of static. This
reduces the total size of the compiled tests. Default: ON.

## Logging

`note-c` provides a comprehensive and flexible logging functionality.

To activate logging, you must provide a callback for logging via
`hookDebugOutput()`. The callback takes the following form:

```c
typedef size_t (*debugOutputFn) (const char *text);
```

The callback is responsible for taking a character array (C-style string) and
returning the number of bytes processed (written out) as confirmation. The
exact implementation will be up to the user who provided the function pointer,
but its presence will active logging in the library.

### Library Logging

#### Log Levels

`note-c` provides for four (4) levels of logging. Here they are listed from
most severe to most verbose:

0. `NOTE_C_LOG_LEVEL_ERROR`
1. `NOTE_C_LOG_LEVEL_WARN`
2. `NOTE_C_LOG_LEVEL_INFO`
3. `NOTE_C_LOG_LEVEL_DEBUG`

By default, `note-c` logs at `NOTE_C_LOG_LEVEL_INFO`.

#### Default Logging Behavior

To modify the default behavior, you may specify the desired log level at compile
time, as follows:

```sh
-DNOTE_C_LOG_LEVEL=0
```

_**NOTE:** In the example above, you will notice we used zero (`0`), instead of
`NOTE_C_LOG_LEVEL_ERROR`. This is because those values are internal to the
library, and not available in the context of the command line._

Here, we have decided to show only the most severe (i.e. `[ERROR]`) logs.
Alternatively, you may set the level to any of the values listed above.

#### Dynamic Logging Behavior

In the previous section, we discussed setting the base (or default) logging
behavior for the library. However, you may also set the log level dynamically,
during runtime, by using the `NoteSetLogLevel()` API.

```c
NoteSetLogLevel(NOTE_C_LOG_LEVEL_WARN)
```

### Notecard Sync Logging (`[SYNC]`)

Tangential to the standard logging behavior, `note-c` also provides a helper
function to invoke/extract synchronization logs from the Notecard.

- `NoteDebugSyncStatus()`

Instead of toggling features inside the library, this helper functions sends a
request to the Notecard to inquire about its synchronization status and logs
those details.

The function is designed to be called in a loop, and throttled by a parameter.
See [the documentation page](https://blues.github.io/note-c/api_reference.html#c.NoteDebugSyncStatus)
for more information.

## Contributing

We love issues, fixes, and pull requests from everyone. By participating in this
project, you agree to abide by the Blues Inc [code of conduct].

For details on contributions we accept and the process for contributing, see our
[contribution guide](CONTRIBUTING.md).

## More Information

For additional Notecard SDKs and Libraries, see:

- [note-arduino][note-arduino] for Arduino support
- [note-python][note-python] for Python
- [note-go][note-go] for Go

## To learn more about Blues Wireless, the Notecard and Notehub, see:

- [blues.com](https://blues.io)
- [notehub.io][notehub]
- [wireless.dev](https://wireless.dev)

## License

Copyright (c) 2019 Blues Inc. Released under the MIT license. See
[LICENSE](LICENSE) for details.

[blues]: https://blues.com
[notehub]: https://notehub.io
[note-arduino]: https://github.com/blues/note-arduino
[note-go]: https://github.com/blues/note-go
[note-python]: https://github.com/blues/note-python
[code of conduct]: https://blues.github.io/opensource/code-of-conduct
