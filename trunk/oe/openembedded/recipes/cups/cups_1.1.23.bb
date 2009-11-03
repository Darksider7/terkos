DESCRIPTION = "An Internet printing system for Unix."
SECTION = "console/utils"
LICENSE = "GPL LGPL"
DEPENDS = "openssl jpeg libpng zlib fakeroot-native"
PR = "r6"

SRC_URI = "ftp://ftp3.easysw.com/pub/cups/${PV}/cups-${PV}-source.tar.bz2 \
           file://strftime_fix.patch;patch=1"

inherit autotools binconfig

LDFLAGS += " -L${STAGING_LIBDIR} "

do_compile () {
	#Eeeks! Hack alert!
	grep CUPS config.h.in~ | grep define >> config.h
	sed -i s:STRIP:NOSTRIP: Makedefs
	sed -i s:serial:: backend/Makefile

	oe_runmake "SSLLIBS=-lssl -lcrypto -L${STAGING_LIBDIR}" \
		   "LIBPNG=-lpng -lm -L${STAGING_LIBDIR}" \
		   "LIBJPEG=-ljpeg -L${STAGING_LIBDIR}" \
		   "LIBZ=-lz -L${STAGING_LIBDIR}" \
		   "-I."
}

fakeroot do_install () {
	oe_runmake "DSTROOT=${D}" install
}

do_stage () {
	install -d ${STAGING_INCDIR}/cups
	install ${S}/cups/*.h ${STAGING_INCDIR}/cups/
	install ${S}/filter/*.h ${STAGING_INCDIR}/cups/
	oe_libinstall -C cups -so libcups ${STAGING_LIBDIR}
	oe_libinstall -C filter -so libcupsimage ${STAGING_LIBDIR}
}

#package the html for the webgui inside the main packages (~1MB uncompressed)
FILES_${PN} += "${datadir}/doc/cups/images \
		${datadir}/doc/cups/*html \
		${datadir}/doc/cups/*.css \
		"




