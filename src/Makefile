include ../config.mk

top_builddir = ..
INSTALL = $(top_builddir)/install-sh -c

CPPFLAGS = $(BASECPPFLAGS) -I../ -I../include

SRCS = commandline.C daemonprocess.C datetime.C dtd.C errorhandler.C file.C filedescriptor.C groupentry.C hostentry.C inetclientsocket.C inetserversocket.C listener.C logger.C memorypool.C passwdentry.C permissions.C protocolentry.C randomnumber.C regularexpression.C semaphoreset.C server.C serversocket.C sharedmemory.C signalclasses.C string.C stringbuffer.C table.C text.C datatransport.C datatransportfactory.C unixclientsocket.C unixserversocket.C unixsocket.C variablebuffer.C xmldom.C xmldomnode.C xmlsax.C
NLOBJS = commandline.n.lo daemonprocess.n.lo dtd.n.lo datetime.n.lo errorhandler.n.lo file.n.lo filedescriptor.n.lo groupentry.n.lo hostentry.n.lo inetclientsocket.n.lo inetserversocket.n.lo listener.n.lo logger.n.lo memorypool.n.lo passwdentry.n.lo permissions.n.lo protocolentry.n.lo randomnumber.n.lo regularexpression.n.lo semaphoreset.n.lo server.n.lo serversocket.n.lo sharedmemory.n.lo signalclasses.n.lo string.n.lo stringbuffer.n.lo table.n.lo text.n.lo datatransport.n.lo datatransportfactory.n.lo unixclientsocket.n.lo unixserversocket.n.lo unixsocket.n.lo variablebuffer.n.lo xmldom.n.lo xmldomnode.n.lo xmlsax.n.lo
PLOBJS = commandline.p.lo daemonprocess.p.lo dtd.p.lo datetime.p.lo errorhandler.p.lo file.p.lo filedescriptor.p.lo groupentry.p.lo hostentry.p.lo inetclientsocket.p.lo inetserversocket.p.lo listener.p.lo logger.p.lo memorypool.p.lo passwdentry.p.lo permissions.p.lo protocolentry.p.lo randomnumber.p.lo regularexpression.p.lo semaphoreset.p.lo server.p.lo serversocket.p.lo sharedmemory.p.lo signalclasses.p.lo string.p.lo stringbuffer.p.lo table.p.lo text.p.lo datatransport.p.lo datatransportfactory.p.lo unixclientsocket.p.lo unixserversocket.p.lo unixsocket.p.lo variablebuffer.p.lo xmldom.p.lo xmldomnode.p.lo xmlsax.p.lo

.SUFFIXES: .n.lo .p.lo

.C.n.lo:
	$(LIBTOOL) --mode=compile $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
	if ( test "$(STRIP)" != "touch" ); then \
		$(STRIP) -x -R .note -R .comment $@; \
	fi

.C.p.lo:
	$(LIBTOOL) --mode=compile $(CXX) $(DEBUGCXXFLAGS) $(CPPFLAGS) -c $< -o $@

all: librudiments.la librudiments_p.la

clean:
	$(LIBTOOL) --mode=clean $(RM) *.lo
	$(LIBTOOL) --mode=clean $(RM) *.la
	$(LIBTOOL) --mode=clean $(RM) *.o
	$(RMTREE) .libs

rebuild: clean all

librudiments.la: $(SRCS) $(NLOBJS)
	$(LIBTOOL) --mode=link $(CXX) -o $@ $(NLOBJS) -rpath $(libdir) -release $(VERSION) -version-info $(SONAME_VERSION_INFO)

librudiments_p.la: $(SRCS) $(PLOBJS)
	$(LIBTOOL) --mode=link $(CXX) -o $@ $(PLOBJS) -rpath $(libdir) -release $(VERSION) -version-info $(SONAME_VERSION_INFO)

install:
	$(top_builddir)/mkinstalldirs $(libdir)
	$(LIBTOOL) --mode=install $(INSTALL) -s librudiments.la $(libdir)/librudiments.la
	$(LIBTOOL) --mode=install $(INSTALL) librudiments_p.la $(libdir)/librudiments_p.la
	$(LIBTOOL) --mode=finish $(libdir)

uninstall:
	$(LIBTOOL) --mode=uninstall $(RM) $(libdir)/librudiments.la
	$(LIBTOOL) --mode=uninstall $(RM) $(libdir)/librudiments_p.la
