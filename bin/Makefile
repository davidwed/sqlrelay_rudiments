top_builddir = ..

include ../config.mk

.PHONY: install uninstall

install:
	$(MKINSTALLDIRS) $(bindir)
	$(INSTALL) rudiments-config $(bindir)

uninstall:
	$(RM) $(bindir)/rudiments-config
