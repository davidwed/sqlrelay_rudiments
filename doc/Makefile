top_builddir = ..

include ../config.mk

.PHONY: install uninstall html

install:
	$(MKINSTALLDIRS) $(docdir)
	$(MKINSTALLDIRS) $(docdir)/classes
	$(MKINSTALLDIRS) $(docdir)/css
	$(MKINSTALLDIRS) $(docdir)/programming
	$(INSTALL) -m 644 guide.html $(docdir)
	$(INSTALL) -m 644 index.html $(docdir)
	$(INSTALL) -m 644 introduction.html $(docdir)
	$(INSTALL) -m 644 invisible.gif $(docdir)
	$(INSTALL) -m 644 reference.html $(docdir)
	$(INSTALL) -m 644 css/styles.css $(docdir)/css
	for i in `/bin/ls classes/*.h.html`; \
	do \
		$(INSTALL) -m 644 $$i $(docdir)/classes; \
	done
	$(INSTALL) -m 644 programming/baseclasses.html $(docdir)/programming
	$(INSTALL) -m 644 programming/utilclasses.html $(docdir)/programming

uninstall:
	$(RMTREE) $(docdir)

html:
	cd classes; \
	rm *.html; \
	for i in `/bin/ls ../../include/rudiments/*.h`; \
	do \
		BASENAME=`/bin/basename $$i`; \
		echo "processing $$BASENAME"; \
		/usr/X11R6/bin/gvim $$i +":runtime! syntax/2html.vim" +"colorscheme default" +":x! ./$$BASENAME.html" +"q!"; \
		sleep 2; \
	done
