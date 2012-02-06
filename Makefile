top_builddir = .

include config.mk

PWD=$(shell pwd)

.PHONY: all testprogs clean install-libs install-includes install-bin \
	 install-doc install-man install-pc install uninstall unconfig distclean

all:
	cd src; $(MAKE) all

testprogs:
	cd test; $(MAKE) all

clean:
	cd src; $(MAKE) clean
	cd test; $(MAKE) clean
	cd msvc; $(RMTREE) Debug
	$(RMTREE) .pics */.pics */*/.pics

install-libs:
	cd src; $(MAKE) install

install-includes:
	cd include; $(MAKE) install

install-bin:
	cd bin; $(MAKE) install

install-doc:
	cd doc; $(MAKE) install

install-man:
	cd man; $(MAKE) install

install-pc:
	$(MKINSTALLDIRS) $(libdir)/pkgconfig
	$(INSTALL) -m 0644 rudiments.pc $(libdir)/pkgconfig/rudiments.pc

install: install-libs install-includes install-bin install-doc install-man install-pc

uninstall:
	cd src; $(MAKE) uninstall
	cd include; $(MAKE) uninstall
	cd bin; $(MAKE) uninstall
	cd doc; $(MAKE) uninstall
	cd man; $(MAKE) uninstall
	$(RM) $(libdir)/pkgconfig/rudiments.pc

distclean: clean
	$(RM) config.mk config.cache config.h config.log config.status include/rudiments/private/config.h include/rudiments/private/config.h.in~ features.mk
	$(RM) bin/rudiments-config
	$(RM) libtool
	$(RMTREE) autom4te.cache
	$(RM) rudiments.pc
	$(RM) msvc/librudiments.opensdf msvc/librudiments.sdf msvc/librudiments.suo
