require ${PN}.inc

PR = "r0"

SRC_URI = "${HANDHELDS_CVS};tag=${TAG};module=opie/noncore/applets/memoryapplet \
           ${HANDHELDS_CVS};tag=${TAG};module=opie/noncore/settings/sysinfo \
           ${HANDHELDS_CVS};tag=${TAG};module=opie/pics"
