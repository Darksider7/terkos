require glib.inc

SRC_URI = "http://ftp.gnome.org/pub/GNOME/sources/glib/2.14/glib-${PV}.tar.bz2 \
           file://glibconfig-sysdefs.h \
           file://configure-libtool.patch;patch=1 \
           file://gcc-4.2-inline-fix.patch;patch=1"
