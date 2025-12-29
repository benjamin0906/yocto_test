require recipes-core/images/core-image-minimal.bb

IMAGE_INSTALL += "libstdc++ mtd-utils" 
IMAGE_INSTALL += "openssh openssl openssh-sftp-server"
IMAGE_INSTALL += "hello"
IMAGE_INSTALL += "raspi-gpio"
IMAGE_INSTALL += "kernel-module-spidev kernel-module-spi-bcm2835"
IMAGE_INSTALL += "rpi-gpio"

