require wlan-ng-utils.inc

PR = "r0"

PV = "0.2.8+svnr${SRCREV}"

SRC_URI += "svn://svn.shaftnet.org/linux-wlan-ng;module=trunk \
        file://scripts-makefile-hostcc.patch;patch=1 \
	file://pcmciasrc.patch;patch=1 \
	file://hostldflags.patch;patch=1"

S = "${WORKDIR}/trunk"
