SUMMARY = "Hello World program"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"
SRC_URI = "file://main.c"
# we can write it like this 
#DEPENDS = "add"
##########################
#or if we use PROVIDES in the another recipe we can use the alias name like this  
DEPENDS = "libmath"
S = "${WORKDIR}" 
do_compile() {
	${CC} ${CFLAGS} ${LDFLAGS} main.c -o main -L${STAGING_LIBDIR} -lmath -Wl,--hash-style=gnu
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 main ${D}${bindir}
}
