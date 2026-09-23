# Flash commands for the custom upload_command in platformio.ini.
#
# These commands live in a sourced file, not in -ex options, so that a failure
# stops the upload. GDB aborts a sourced file at the first command that fails,
# and with -batch it then exits non-zero. With -ex options, GDB runs the later
# commands anyway and exits 0 (or, with -batch, reports only the last command),
# so a failed connect or load looked like a successful upload.
#
# If the "target extended-remote" -ex before this file fails, the first
# "monitor" command here fails too, so that case also exits non-zero.
monitor reset halt
load
monitor reset
