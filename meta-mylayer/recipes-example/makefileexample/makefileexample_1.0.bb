SUMMARY = "automation build with simple make file "
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"
SRC_URI = "file://lib-add.c \
	   file://lib-sub.c \
	   file://lib-mul.c \
	   file://makefileexample.c \
	   file://Makefile \
 	  "
S = "${WORKDIR}" 
# \ escape character to scape space 
LIBS = "lib-add.c\ lib-sub.c\ lib-mul.c"
EXTRA_OEMAKE:append = " TARGET=${PN} libs=${LIBS}"
EXTRA_OEMAKE:append = " DESTDIR=${D} BINDIR=${bindir}"
do_compile() {
	#${CC} ${CFLAGS} -c lib-add.c -o lib-add.o   
	#${CC} ${CFLAGS} -c lib-sub.c -o lib-sub.o   
	#${CC} ${CFLAGS} -c lib-mul.c -o lib-mul.o   
	#${CC} ${CFLAGS} -c makefileexample.c -o makefileexample.o 
	#${CC} ${CFLAGS} ${LDFLAGS} makefileexample.o lib-add.o lib-sub.o lib-mul.o -o main
	oe_runmake
}

do_install() {
	#install -d ${D}${bindir}
	#install -m 0755 makefileexample ${D}${bindir}
	#oe_runmake install DESTDIR=${D} BINDIR=${bindir}
	# we can use EXTRA_OEMAKE , in this case we only write 
	oe_runmake install
}
