top_builddir = ..

include ../config.mk

install:
	$(MKINSTALLDIRS) $(docdir)
	$(MKINSTALLDIRS) $(docdir)/css
	$(MKINSTALLDIRS) $(docdir)/classes/html
	$(MKINSTALLDIRS) $(docdir)/programming
	$(CP) installing.html $(docdir)
	$(CP) index.html $(docdir)
	$(CP) introduction.html $(docdir)
	$(CP) invisible.gif $(docdir)
	$(CP) css/styles.css $(docdir)/css
	$(CP) classes/html/*.html $(docdir)/classes/html
	$(CP) classes/html/*.png $(docdir)/classes/html
	$(CP) classes/html/*.css $(docdir)/classes/html
	$(CP) classes/html/*.js $(docdir)/classes/html
	$(CP) programming/guide.html $(docdir)/programming
	$(CHMOD) 644 $(docdir)/*.html
	$(CHMOD) 644 $(docdir)/*.gif
	$(CHMOD) 644 $(docdir)/css/*
	$(CHMOD) 644 $(docdir)/classes/html/*.html
	$(CHMOD) 644 $(docdir)/classes/html/*.png
	$(CHMOD) 644 $(docdir)/classes/html/*.css
	$(CHMOD) 644 $(docdir)/classes/html/*.js
	$(CHMOD) 644 $(docdir)/programming/*.html

uninstall:
	$(RMTREE) $(docdir)
