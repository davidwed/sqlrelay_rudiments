include ../config.mk

top_srcdir = ..
INSTALL = $(top_srcdir)/install-sh -c

install:
	$(top_srcdir)/mkinstalldirs $(bindir)
	$(INSTALL) rudiments-config $(bindir)

uninstall:
	$(RM) $(bindir)/rudiments-config
