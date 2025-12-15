#!/bin/bash

root_part=$( blkid | grep ROOT | cut -d ":" -f 1 )
boot_part=$( blkid | grep BOOT | cut -d ":" -f 1 )

curr_path=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd)

root_mount="/media/ROOT/"
boot_mount="/media/BOOT/"

#fstab="proc                    /proc           proc    defaults          0       0\\n""/dev/mmcblk0p1          /boot           vfat    defaults          0       2\\n""/dev/mmcblo0p2          /               ext4    defaults,noatime  0       1\\n"

#root_mount="$curr_path/test_root/"
#boot_mount="$curr_path/test_boot/"

image_rel_path="build/tmp/deploy/images/raspberrypi4-64/"

echo "ROOT partition: " $root_part "BOOT partition:" $boot_part

if [[ -n "$root_part" && -n "$boot_part" ]]; then
  echo "Root and boot partition has been found"
  mount $root_part $root_mount
  mount $boot_part $boot_mount

  rm -rf $root_mount"*"
  echo ${root_mount}"/*"

  cp "${curr_path}""/""${image_rel_path}"bcm2711-rpi-4-b.dtb"" "$boot_mount"
  cp "${curr_path}""/""${image_rel_path}"Image"" ${boot_mount}"kernel_rpilinux.img"
  #rm "${curr_path}"/poky/"${image_rel_path}"bootfiles/*.txt""
  cp "${curr_path}""/""${image_rel_path}"bootfiles/*"" ${boot_mount}
  tar -xjf "${curr_path}""/""${image_rel_path}"rpilinux-image-raspberrypi4-64.rootfs.tar.bz2"" -C "$root_mount"
  #echo -e $fstab > $root_mount"/etc/fstab"

  #umount /media/ROOT

fi
