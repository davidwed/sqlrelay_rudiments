top_builddir = .

include config.mk

.PHONY: all clean install uninstall distclean

all:
	$(MAKE) -C src all

clean:
	$(MAKE) -C src clean
	$(MAKE) -C test clean
	$(RMTREE) msvc/Debug

install:
	$(MAKE) -C src install
	$(MAKE) -C include install
	$(MAKE) -C bin install
	$(MAKE) -C doc install
	$(MAKE) -C man install
	$(MKINSTALLDIRS) $(libdir)/pkgconfig
	$(INSTALL) -m 0644 rudiments.pc $(libdir)/pkgconfig/rudiments.pc

uninstall:
	$(MAKE) -C src uninstall
	$(MAKE) -C include uninstall
	$(MAKE) -C bin uninstall
	$(MAKE) -C doc uninstall
	$(MAKE) -C man uninstall
	$(RM) $(libdir)/pkgconfig/rudiments.pc

distclean: clean
	$(RM) config.mk config.cache config.h config.log config.status features.mk libtool rudiments.pc include/Makefile include/rudiments/private/config.h include/rudiments/private/config.h.in~ bin/rudiments-config src/Makefile msvc/librudiments.opensdf msvc/librudiments.sdf msvc/librudiments.suo
	$(RMTREE) autom4te.cache
