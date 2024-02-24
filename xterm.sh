#!/bin/sh

T=$1

./xterm-389/xterm +u8 -font 10x20 -geometry 80x24 -ti 340 -tn 340 -e ./picocom/picocom $T
