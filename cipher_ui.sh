#!/bin/sh
mode="664"

make

insmod  ./cipher.ko || exit 1

rm -f /dev/cipher0

major=`awk "\\$2==\"cipher\" {print \\$1}" /proc/devices`

mknod /dev/cipher0 c $major 0

group="staff"
grep '^staff:' /etc/group > /dev/null || group="wheel"

chgrp $group /dev/cipher0
chmod $mode /dev/cipher0

gcc cipher_ui.c -o cipher_ui.out

./cipher_ui.out /dev/cipher0 $major

rmmod ./cipher.ko
