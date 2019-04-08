#!/bin/bash	

cd root
find . -print -depth | cpio -ov -H newc > ../initramfs.cpio.gz

