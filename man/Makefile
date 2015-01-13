top_builddir = ..

include ../config.mk

.PHONY: install uninstall

install:
	$(MKINSTALLDIRS) $(mandir)
	$(MKINSTALLDIRS) $(mandir)/man1
	$(CP) rudiments-config.1 $(mandir)/man1
	$(CHMOD) 644 $(mandir)/man1/rudiments-config.1

uninstall:
	$(RM) $(mandir)/man1/rudiments-config.1
