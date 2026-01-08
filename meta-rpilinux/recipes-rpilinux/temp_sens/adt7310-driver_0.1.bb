SUMMARY = "Temperature measurement app using spi sensor"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://src"

S = "${WORKDIR}/src"

TARGET_CC_ARCH += "${LDFLAGS}"

do_configure() {
  echo "szia"
}

do_compile() {
  oe_runmake
  #${CC} -o hi hi.c
}

do_install() {
  #oe_runmake install adt7310_driver
  install -d ${D}${bindir}
  install -m 0755 adt7310_driver.out ${D}${bindir}
  #install -d ${D}${bindir}
  #install -m 0755 hi ${D}${bindir}
}
