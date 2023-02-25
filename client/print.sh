#!/bin/sh
pbmtext -width=480 "$*" | pamcut -pad -width 480 -height 32 | pamtopng | sudo ./kbdtool /dev/stdin
