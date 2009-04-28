# dnsmasq with support for DBus interface

require dnsmasq.inc

DEPENDS = "dbus"
EXTRA_OEMAKE = "COPTS=-DHAVE_DBUS"

do_install_append () {
        install -d ${D}${sysconfdir}/dbus-1/system.d
        install -m 644 dbus/dnsmasq.conf ${D}${sysconfdir}/dbus-1/system.d/
}
