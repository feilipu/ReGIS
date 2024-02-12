#!/bin/sh

T=/dev/ttyACM0

./xterm-389/xterm +u8 -geometry 132x50 -ti 340 -tn 340 -e picocom $T
