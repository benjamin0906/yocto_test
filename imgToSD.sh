#!/bin/bash

curr_path=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd)
boot_mount="/media/BOOT/"
image_rel_path="build/tmp/deploy/images/raspberrypi4-64/"

sudo bzcat build/tmp/deploy/images/raspberrypi4-64/rpilinux-image-raspberrypi4-64.rootfs.wic.bz2 | sudo dd of="$1" status=progress

boot_part=$( blkid | grep boot | cut -d ":" -f 1 )

if [[ -n "$boot_part" ]]; then
  echo "boot partition has been found"

  mount $boot_part $boot_mount

  cp "${curr_path}""/""${image_rel_path}"bcm2711-rpi-4-b.dtb"" "$boot_mount"
  cp "${curr_path}""/""${image_rel_path}"Image"" ${boot_mount}"kernel_rpilinux.img"
  cp "${curr_path}""/""${image_rel_path}"bootfiles/*"" ${boot_mount}

  umount /media/BOOT

fi

