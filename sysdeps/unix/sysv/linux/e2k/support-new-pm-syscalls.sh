#!/bin/bash

# Determine if `$CC -mptr128' implements new PM ABI and thus supports new
# syscalls. At the user level this requires __NEW_PM_SYSCALLS to be defined
# in the generated <bits/syscall.h> whichever of the three supported modes
# glibc is currently built for. If $CC (say, the present e2k-linux-gcc) does
# NOT support PM at all, the generated <bits/syscall.h> is intentionally made
# "compatible" with the latest PM filesystems.
CC=$1

echo -e \
"#ifndef __ptr128_new_abi__\n"\
"# error unsupported\n"\
"#endif" \
| $CC -x c -mptr128 - -S -o - 2>&1 | grep -q "unsupported"

if [ $? != 0 ]; then
    echo "yes";
else
    echo "no";
fi
