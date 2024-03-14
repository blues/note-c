#!/bin/bash

if [ -n "$MD5SRV_PORT" ]; then
    echo "INFO: Using MD5 server port $MD5SRV_PORT." >&2
else
    echo "ERROR: MD5SRV_PORT not defined." >&2
    exit 1
fi

if ! which tmole > /dev/null 2>&1; then
    echo "ERROR: tmole command not found." >&2
    exit 1
fi

echo "INFO: Starting tmole..." >&2
tmole $MD5SRV_PORT > tmole.log 2>&1 &
TMOLE_PID=$!
echo "INFO: tmole PID is $TMOLE_PID." >&2
echo "TMOLE_PID=$TMOLE_PID" >> $GITHUB_ENV
timeout 10 bash -c "until test -e tmole.log; do sleep 0.1; done"
if [ $? -ne 0 ]; then
    echo "ERROR: tmole failed to start." >&2
    exit 1
fi

sleep 1

# find the url
exec 3< tmole.log
# ignore the first line
read <&3 output
read <&3 output

echo "Getting MD5 server URL from tmole.log..." >&2
MD5SRV_URL=`echo "$output" | grep https | cut -d " " -f1`
if [ -z "$MD5SRV_URL" ]; then
    echo "ERROR: Failed to get MD5 server URL." >&2
    exit 1
else
    echo "INFO: Got MD5 server URL." >&2
fi

echo "MD5SRV_URL=$MD5SRV_URL" >> $GITHUB_ENV
# Only create tmole_ready once MD5SRV_URL has been set, as the next step in the
# GitHub workflow depends on it.
touch tmole_ready

sleep 2

echo "INFO: tmole ready. Logging to `realpath tmole.log`" >&2
