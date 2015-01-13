top_builddir = ..

include ../config.mk

install:
	$(MKINSTALLDIRS) $(bindir)
	$(CP) rudiments-config $(bindir)
	$(CHMOD) 755 $(bindir)/rudiments-config

uninstall:
	$(RM) $(bindir)/rudiments-config
