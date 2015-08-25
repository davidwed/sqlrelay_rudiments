top_builddir = ..

include ../config.mk

all:

install:
	$(MKINSTALLDIRS) $(docdir)
	$(MKINSTALLDIRS) $(docdir)/css
	$(MKINSTALLDIRS) $(docdir)/classes/html
	$(MKINSTALLDIRS) $(docdir)/features
	$(MKINSTALLDIRS) $(docdir)/programming
	$(CP) installing.html $(docdir)
	$(CP) index.html $(docdir)
	$(CP) css/styles.css $(docdir)/css
	$(CP) classes/html/*.html $(docdir)/classes/html
	$(CP) classes/html/*.png $(docdir)/classes/html
	$(CP) classes/html/*.css $(docdir)/classes/html
	$(CP) classes/html/*.js $(docdir)/classes/html
	$(CP) features/*.html $(docdir)/features
	$(CP) programming/*.html $(docdir)/programming
	$(CHMOD) 644 $(docdir)/*.html
	$(CHMOD) 644 $(docdir)/css/*
	$(CHMOD) 644 $(docdir)/classes/html/a*.html
	$(CHMOD) 644 $(docdir)/classes/html/b*.html
	$(CHMOD) 644 $(docdir)/classes/html/c*.html
	$(CHMOD) 644 $(docdir)/classes/html/d*.html
	$(CHMOD) 644 $(docdir)/classes/html/e*.html
	$(CHMOD) 644 $(docdir)/classes/html/f*.html
	$(CHMOD) 644 $(docdir)/classes/html/g*.html
	$(CHMOD) 644 $(docdir)/classes/html/h*.html
	$(CHMOD) 644 $(docdir)/classes/html/i*.html
	$(CHMOD) 644 $(docdir)/classes/html/l*.html
	$(CHMOD) 644 $(docdir)/classes/html/m*.html
	$(CHMOD) 644 $(docdir)/classes/html/n*.html
	$(CHMOD) 644 $(docdir)/classes/html/p*.html
	$(CHMOD) 644 $(docdir)/classes/html/r*.html
	$(CHMOD) 644 $(docdir)/classes/html/s*.html
	$(CHMOD) 644 $(docdir)/classes/html/t*.html
	$(CHMOD) 644 $(docdir)/classes/html/u*.html
	$(CHMOD) 644 $(docdir)/classes/html/w*.html
	$(CHMOD) 644 $(docdir)/classes/html/x*.html
	$(CHMOD) 644 $(docdir)/classes/html/*.png
	$(CHMOD) 644 $(docdir)/classes/html/*.css
	$(CHMOD) 644 $(docdir)/classes/html/*.js
	$(CHMOD) 644 $(docdir)/features/*.html
	$(CHMOD) 644 $(docdir)/programming/*.html

uninstall:
	$(RMTREE) $(docdir)
