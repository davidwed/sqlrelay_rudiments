include ../config.mk

top_srcdir = ..
INSTALL = $(top_srcdir)/install-sh -c

install:
	$(top_srcdir)/mkinstalldirs $(docdir)
	$(top_srcdir)/mkinstalldirs $(docdir)/classes
	$(top_srcdir)/mkinstalldirs $(docdir)/css
	$(top_srcdir)/mkinstalldirs $(docdir)/programming
	$(INSTALL) -m 644 guide.html $(docdir)
	$(INSTALL) -m 644 index.html $(docdir)
	$(INSTALL) -m 644 introduction.html $(docdir)
	$(INSTALL) -m 644 invisible.gif $(docdir)
	$(INSTALL) -m 644 reference.html $(docdir)
	$(INSTALL) -m 644 css/styles.css $(docdir)/css
	$(INSTALL) -m 644 classes/commandline.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/daemonprocess.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/datetime.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/errorhandler.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/inetclientsocket.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/inetserversocket.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/listener.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/logger.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/memorypool.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/permissions.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/randomnumber.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/regularexpression.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/semaphoreset.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/sharedmemory.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/signalclasses.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/text.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/transport.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/unixclientsocket.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/unixserversocket.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/xmldom.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/xmldomnode.h.html $(docdir)/classes
	$(INSTALL) -m 644 classes/xmlsax.h.html $(docdir)/classes
	$(INSTALL) -m 644 programming/baseclasses.html $(docdir)/programming
	$(INSTALL) -m 644 programming/utilclasses.html $(docdir)/programming

uninstall:
	$(RMTREE) $(docdir)
