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

install: install-libs
	cd include; $(MAKE) install
	cd bin; $(MAKE) install
	cd doc; $(MAKE) install

uninstall:
	cd src; $(MAKE) uninstall
	cd include; $(MAKE) uninstall
	cd bin; $(MAKE) uninstall
	cd doc; $(MAKE) uninstall

unconfig: clean
	$(RM) config.mk config.cache config.h config.log config.status include/rudiments/private/config.h include/rudiments/private/config.h.in~
	$(RM) bin/rudiments-config
	$(RM) libtool
	$(RMTREE) autom4te.cache

distclean: unconfig

tgz: all
	mkdir /tmp/rudiments-$(RUDIMENTS_VERSION)
	make prefix=/tmp/rudiments-$(RUDIMENTS_VERSION)/$(prefix) install
	cd /tmp/rudiments-$(RUDIMENTS_VERSION); makepkg rudiments-$(RUDIMENTS_VERSION).tgz
	mv /tmp/rudiments-$(RUDIMENTS_VERSION)/rudiments-$(RUDIMENTS_VERSION).tgz $(PWD)
	$(RMTREE) /tmp/rudiments-$(RUDIMENTS_VERSION)
