top_builddir = ..

include ../config.mk

.PHONY: install uninstall

install:
	$(MKINSTALLDIRS) $(mandir)
	$(MKINSTALLDIRS) $(mandir)/man1
	$(INSTALL) -m 644 rudiments-config.1 $(mandir)/man1

uninstall:
	$(RM) $(mandir)/man1/rudiments-config.1
