# After PlatformIO's upload step (i.e. flashing the firmware onto the Swan), it
# runs the after_upload method registered in this file.

import os
import select
import time

Import("env")


# There's an issue with PlatformIO where, after uploading, it expects to see the
# test port (i.e. the Swan's USB, which outputs test logging) too quickly. After
# upload, it may take a moment for the device to appear, and so we don't want to
# proceed past the upload phase until it's appeared.
def _wait_for_test_port(env):
    port = env.GetProjectOption("test_port")
    print("Waiting for test port: " + port)

    timeout_secs = 30
    start = time.time()
    while not os.path.exists(port):
        if (time.time() - start) > timeout_secs:
            raise Exception(f"Timed out waiting for test port: {port}")
        time.sleep(0.1)

    return port


# The test firmware (test/test_main.cpp) does not run the tests until it receives
# "start\n" on the Swan's USB. It only starts listening once it has booted far
# enough to reach its read loop, at which point it prints:
#
#     Waiting to receive "start"...
#
# The freshly-reset Swan and the test port can become ready at nearly the same
# instant, so sending "start" the moment the port appears races the firmware's
# boot: the byte can be dropped before the read loop is running, leaving the
# firmware waiting forever with no output (a silent hang of the test step).
#
# To avoid that race, wait until we have actually seen the firmware's prompt,
# then send "start" -- at that point the firmware is guaranteed to be in its
# read loop. If the prompt isn't seen within the timeout (e.g. it was emitted
# before we opened the port and wasn't buffered), fall back to sending anyway,
# which is no worse than the previous unconditional behaviour.
#
# Use plain os-level file I/O rather than pyserial: opening via pyserial toggles
# the DTR/RTS modem-control lines, which errors on this virtual (TCP-backed)
# serial device -- the same reason the tests run with --no-reset.
def _start_tests(port):
    prompt = b'Waiting to receive "start"'
    prompt_timeout = 20  # seconds to wait for the firmware's prompt

    fd = os.open(port, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
    try:
        # 1) Wait until the firmware prints its prompt, i.e. it has booted into
        #    its read loop and is listening.
        buf = b""
        deadline = time.time() + prompt_timeout
        saw_prompt = False
        while time.time() < deadline:
            r, _, _ = select.select([fd], [], [], 0.5)
            if not r:
                continue
            try:
                data = os.read(fd, 4096)
            except (BlockingIOError, OSError):
                continue
            if data:
                buf = (buf + data)[-4096:]  # the prompt is short; keep the tail
                if prompt in buf:
                    saw_prompt = True
                    break
        if saw_prompt:
            print('Saw prompt; sending "start" to kick off tests...')
        else:
            print(
                f"Did not see the firmware prompt within {prompt_timeout}s; "
                'sending "start" anyway.'
            )

        # 2) Send "start" as a short burst rather than once. The very first send
        #    can be dropped while the just-reset Swan's USB re-enumeration
        #    settles (a ~2s window observed on hardware); a repeat a couple
        #    seconds later lands. Extra "start"s after the firmware has begun
        #    are harmless -- it has already left its read loop. Kept short (a few
        #    seconds) so it finishes before the firmware's post-start delay
        #    elapses and the test-monitor step takes over.
        for _ in range(8):
            try:
                os.write(fd, b"start\n")
            except OSError:
                pass
            time.sleep(0.4)
    finally:
        os.close(fd)


def after_upload(source, target, env):
    port = _wait_for_test_port(env)
    _start_tests(port)

    # This pause seems to prevent this error from happening at the start of the
    # test step: "device reports readiness to read but returned no data (device
    # disconnected or multiple access # on port?)" Not totally sure why, but
    # maybe this gives the OS a chance to cleanup anything using the test port.
    time.sleep(3)


env.AddPostAction("upload", after_upload)
