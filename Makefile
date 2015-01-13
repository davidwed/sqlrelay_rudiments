top_builddir = .

include config.mk

.PHONY: all clean install uninstall distclean

all:
	cd src; $(MAKE) all

clean:
	cd src; $(MAKE) clean
	cd test; $(MAKE) clean
	cd examples; $(MAKE) clean
	$(RMTREE) msvc/Debug
	$(RMTREE) msvc/Release
	$(RMTREE) msvc/DebugCLR
	$(RMTREE) msvc/ReleaseCLR
	$(RMTREE) msvc/x64

install:
	cd src; $(MAKE) install
	cd include; $(MAKE) install
	cd bin; $(MAKE) install
	cd doc; $(MAKE) install
	cd man; $(MAKE) install
	$(MKINSTALLDIRS) $(libdir)/pkgconfig
	$(CP) rudiments.pc $(libdir)/pkgconfig/rudiments.pc
	$(CHMOD) 644 $(libdir)/pkgconfig/rudiments.pc

uninstall:
	cd src; $(MAKE) uninstall
	cd include; $(MAKE) uninstall
	cd bin; $(MAKE) uninstall
	cd doc; $(MAKE) uninstall
	cd man; $(MAKE) uninstall
	$(RM) $(libdir)/pkgconfig/rudiments.pc

distclean: clean
	$(RM) config.mk config.cache config.h config.log config.status features.mk libtool rudiments.pc include/Makefile include/rudiments/private/config.h include/rudiments/private/config.h.in~ bin/rudiments-config src/Makefile msvc/librudiments.opensdf msvc/librudiments.sdf msvc/librudiments.suo
	$(RMTREE) autom4te.cache
