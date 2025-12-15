#!/bin/bash

$"parted -s $1 mklabel msdos mkpart primary fat32 1M 100M mkpart primary ext4 100M 100%"
mkfs.vfat $1"1"
mkfs.ext4 $1"2"

fatlabel $1"1" BOOT
e2label $1"2" ROOT
