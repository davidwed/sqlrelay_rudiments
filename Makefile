top_builddir = .

include config.mk

PWD=$(shell pwd)

all:
	cd src; $(MAKE) all

testprogs:
	cd test; $(MAKE) all

clean:
	cd src; $(MAKE) clean
	cd test; $(MAKE) clean
	$(RMTREE) .pics */.pics */*/.pics

rebuild:
	cd src; $(MAKE) rebuild

install-libs:
	cd src; $(MAKE) install

install-includes:
	cd include; $(MAKE) install

install-bin:
	cd bin; $(MAKE) install

install-doc:
	cd doc; $(MAKE) install

install-pc:
	$(MKINSTALLDIRS) $(libdir)/pkgconfig
	$(INSTALL) -m 0644 rudiments.pc $(libdir)/pkgconfig/rudiments.pc

install: install-libs install-includes install-bin install-doc install-pc

uninstall:
	cd src; $(MAKE) uninstall
	cd include; $(MAKE) uninstall
	cd bin; $(MAKE) uninstall
	cd doc; $(MAKE) uninstall
	$(RM) $(libdir)/pkgconfig/rudiments.pc

unconfig: clean
	$(RM) config.mk config.cache config.h config.log config.status include/rudiments/private/config.h include/rudiments/private/config.h.in~ features.mk
	$(RM) bin/rudiments-config
	$(RM) libtool
	$(RMTREE) autom4te.cache
	$(RM) strip-comment-note
	$(RM) rudiments.pc

distclean: unconfig

slackware: all
	mkdir /tmp/rudiments-$(RUDIMENTS_VERSION)
	make prefix=/tmp/rudiments-$(RUDIMENTS_VERSION)/$(prefix) install
	cd /tmp/rudiments-$(RUDIMENTS_VERSION); makepkg rudiments-$(RUDIMENTS_VERSION).tgz
	mv /tmp/rudiments-$(RUDIMENTS_VERSION)/rudiments-$(RUDIMENTS_VERSION).tgz $(PWD)
	$(RMTREE) /tmp/rudiments-$(RUDIMENTS_VERSION)
