DESCRIPTION = "Socketcan kernel modules"
HOMEPAGE = "http://developer.berlios.de/projects/socketcan/"
SECTION = "kernel/modules"
LICENSE = "GPL"
DEPENDS = "virtual/kernel"
PV = "0.0+svnr${SRCREV}"
PR = "r1"

SRC_URI = "svn://svn.berlios.de/socketcan;module=trunk;proto=svn"

S = "${WORKDIR}/trunk/kernel/2.6"

inherit module

