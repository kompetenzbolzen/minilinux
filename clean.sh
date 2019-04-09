#!/bin/bash

cd init
make clean
cd ../root
rm -df init
cd ..
rm -df initramfs.cpio.gz
