include config.mk

all:
	cd src; $(MAKE) all

testprogs:
	cd test; $(MAKE) all

clean:
	cd src; $(MAKE) clean
	cd test; $(MAKE) clean
	$(RMTREE) .pics */.pics */*/.pics
	$(RM) packages/RPMS/*.rpm
	$(RM) packages/SRPMS/*.rpm

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
	$(RM) config.mk config.cache config.h config.log config.status rudiments.spec include/rudiments/private/config.h include/rudiments/private/config.h.in~
	$(RM) rudiments.spec
	$(RM) rudiments.spec.in
	$(RM) bin/rudiments-config
	$(RM) libtool
	$(RMTREE) autom4te.cache

rpm:
	cd ..; \
	$(RM) $(RPM_BUILD_DIR)/SOURCES/rudiments-$(VERSION).tar.gz; \
	tar cf $(RPM_BUILD_DIR)/SOURCES/rudiments-$(VERSION).tar rudiments-$(VERSION); \
	gzip $(RPM_BUILD_DIR)/SOURCES/rudiments-$(VERSION).tar
	rpm -ba rudiments.spec
	mv $(RPM_BUILD_DIR)/SRPMS/rudiments-$(VERSION)*.src.rpm packages/SRPMS
	mv $(RPM_BUILD_DIR)/RPMS/*/rudiments-$(VERSION)*.rpm packages/RPMS
	mv $(RPM_BUILD_DIR)/RPMS/*/rudiments-devel-$(VERSION)*.rpm packages/RPMS
	chmod 666 packages/RPMS/*
	$(RMTREE) $(RPM_BUILD_DIR)/BUILD/rudiments-$(VERSION)
	$(RM) $(RPM_BUILD_DIR)/SOURCES/rudiments-$(VERSION).tar.gz

distclean: unconfig
