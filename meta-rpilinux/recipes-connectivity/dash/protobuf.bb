LICENSE = "GPL-3.0-only"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-3.0-only;md5=c79ff39f19dfec6d293b95dea7b07891"

#LIC_FILES_CHKSUM = "md5=c79ff39f19dfec6d293b95dea7b07891"

SRC_URI="git://github.com/protocolbuffers/protobuf.git;branch=30.x;protocol=https"
SRCREV="d295af5c3002c08e1bfd9d7f9e175d0a4d015f1e"

S = "${WORKDIR}/git"

inherit cmake pkgconfig

DEPENDS += "abseil zlib"

EXTRA_OECMAKE += "-DCMAKE_POSITION_INDEPENDENT_CODE=ON -DBUILD_SHARED_LIBS=ON -DCMAKE_CXX_STANDARD=17 -Dprotobuf_BUILD_TESTS=OFF"
