SUMMARY = "Hello World program"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"
SRC_URI = "file://add.c"
SRC_URI:append = " file://add.h"


S = "${WORKDIR}/build"

do_compile() {
	${CC} ${CFLAGS}  ${LDFLAGS} ${WORKDIR}/add.c -o ${S}/add
}
do_install() {
	install -d ${D}${bindir}
	install -m 0755 ${S}/add ${D}${bindir}/
}
