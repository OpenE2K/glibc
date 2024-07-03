#!/bin/bash

# set -x
# echo $@

WDIR=$(realpath .)
shift
rsh $REMOTE_HOSTNAME "cd $WDIR;$@"
