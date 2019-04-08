#!/bin/bash

screen qemu-system-x86_64 -nographic -no-reboot -kernel bzImage -initrd initramfs.cpio.gz -m 256M -append "console=ttyS0 panic=1"

