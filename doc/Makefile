top_builddir = ..

include ../config.mk

.PHONY: install uninstall

install:
	$(MKINSTALLDIRS) $(docdir)
	$(MKINSTALLDIRS) $(docdir)/classes/html
	$(MKINSTALLDIRS) $(docdir)/css
	$(MKINSTALLDIRS) $(docdir)/programming
	$(INSTALL) -m 644 installing.html $(docdir)
	$(INSTALL) -m 644 index.html $(docdir)
	$(INSTALL) -m 644 introduction.html $(docdir)
	$(INSTALL) -m 644 invisible.gif $(docdir)
	$(INSTALL) -m 644 css/styles.css $(docdir)/css
	for file in `/bin/ls classes/html/*.html classes/html/*.css classes/html/*.png classes/html/*.js`; \
	do \
		$(INSTALL) -m 644 $$file $(docdir)/classes/html; \
	done
	$(INSTALL) -m 644 programming/baseclasses.html $(docdir)/programming
	$(INSTALL) -m 644 programming/utilclasses.html $(docdir)/programming
	$(INSTALL) -m 644 programming/wrapperclasses.html $(docdir)/programming

uninstall:
	$(RMTREE) $(docdir)
