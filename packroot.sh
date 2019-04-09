#!/bin/bash	

cd init
make build copy
cd ..

cd root
find . | cpio -o -H newc | gzip > ../initramfs.cpio.gz
