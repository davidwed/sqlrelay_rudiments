top_builddir = .

include config.mk

all:
	cd src $(AND) $(MAKE) all

clean:
	cd src $(AND) $(MAKE) clean
	cd test $(AND) $(MAKE) clean
	cd bench $(AND) $(MAKE) clean
	cd examples $(AND) $(MAKE) clean

install:
	cd src $(AND) $(MAKE) install
	cd include $(AND) $(MAKE) install
	cd bin $(AND) $(MAKE) install
	cd doc $(AND) $(MAKE) install
	cd man $(AND) $(MAKE) install
	$(MKINSTALLDIRS) $(licensedir)
	$(CP) COPYING $(licensedir)
	$(CHMOD) 0644 $(licensedir)/COPYING
	$(MKINSTALLDIRS) $(libdir)/pkgconfig
	$(CP) rudiments.pc $(libdir)/pkgconfig
	$(CHMOD) 644 $(libdir)/pkgconfig/rudiments.pc

uninstall:
	cd src $(AND) $(MAKE) uninstall
	cd include $(AND) $(MAKE) uninstall
	cd bin $(AND) $(MAKE) uninstall
	cd doc $(AND) $(MAKE) uninstall
	cd man $(AND) $(MAKE) uninstall
	$(RM) $(libdir)/pkgconfig/rudiments.pc
	$(RMTREE) $(licensedir)

distclean: clean
	$(RM) config.mk config.cache config.h config.log config.status features.mk libtool rudiments.pc include/Makefile include/rudiments/private/config.h include/rudiments/private/config.h.in~ bin/rudiments-config src/Makefile msvc/rudiments.opensdf msvc/rudiments.sdf msvc/rudiments.suo
	$(RMTREE) autom4te.cache

# for packaging systems that like to grab the license directly from
# the source code and don't like where it gets installed by default
uninstall-license:
	$(RMTREE) $(licensedir)

cppcheck:
	cppcheck -j4 --enable=warning,performance,portability src > /dev/null

tests:
	cd test $(AND) $(MAKE) tests

tests-clean:
	cd test $(AND) $(MAKE) clean
