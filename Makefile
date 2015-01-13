top_builddir = .

include config.mk

.PHONY: all clean install uninstall distclean

all:
	cd src $(AND) $(MAKE) all

clean:
	cd src $(AND) $(MAKE) clean
	cd test $(AND) $(MAKE) clean
	cd examples $(AND) $(MAKE) clean
	$(RMTREE) msvc/Debug
	$(RMTREE) msvc/Release
	$(RMTREE) msvc/DebugCLR
	$(RMTREE) msvc/ReleaseCLR
	$(RMTREE) msvc/x64

install:
	cd src $(AND) $(MAKE) install
	cd include $(AND) $(MAKE) install
	cd bin $(AND) $(MAKE) install
	cd doc $(AND) $(MAKE) install
	cd man $(AND) $(MAKE) install
	$(MKINSTALLDIRS) $(libdir)/pkgconfig
	$(CP) rudiments.pc $(libdir)/pkgconfig/rudiments.pc
	$(CHMOD) 644 $(libdir)/pkgconfig/rudiments.pc

uninstall:
	cd src $(AND) $(MAKE) uninstall
	cd include $(AND) $(MAKE) uninstall
	cd bin $(AND) $(MAKE) uninstall
	cd doc $(AND) $(MAKE) uninstall
	cd man $(AND) $(MAKE) uninstall
	$(RM) $(libdir)/pkgconfig/rudiments.pc

distclean: clean
	$(RM) config.mk config.cache config.h config.log config.status features.mk libtool rudiments.pc include/Makefile include/rudiments/private/config.h include/rudiments/private/config.h.in~ bin/rudiments-config src/Makefile msvc/librudiments.opensdf msvc/librudiments.sdf msvc/librudiments.suo
	$(RMTREE) autom4te.cache
