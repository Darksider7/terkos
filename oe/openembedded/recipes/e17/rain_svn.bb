LICENSE = "MIT"
PV = "0.0.3+svnr${SRCREV}"

require e-module.inc

do_configure_prepend() {
	sed -i -e /po/d configure.ac 
	sed -i -e s:\ po::g Makefile.am
}



