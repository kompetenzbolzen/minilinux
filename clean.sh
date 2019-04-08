#!/bin/bash

cd init
make clean
cd ../root
rm -df *
cd ..
rm -df initramfs.cpio.gz
