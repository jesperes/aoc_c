#!/bin/bash

FLAMEGRAPH="$HOME"/dev/FlameGraph

sudo perf record -F 99 -g "$@"
sudo chown "$(id -u):$(id -g)" perf.data*
# perf script -F comm,pid,tid,cpu,time,event,ip,sym,dso >perf.script
perf script >perf.script
"$FLAMEGRAPH"/stackcollapse-perf.pl perf.script >perf.folded
"$FLAMEGRAPH"/flamegraph.pl perf.folded >perf.svg
