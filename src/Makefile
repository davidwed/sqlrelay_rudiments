include ../config.mk

top_builddir = ..
INSTALL = $(top_builddir)/install-sh -c

CPPFLAGS = $(BASECPPFLAGS) -I../ -I../include

SRCS = commandline.C connectiondata.C daemonprocess.C datetime.C dtd.C errorhandler.C file.C filedescriptor.C groupentry.C hostentry.C inetclientsocket.C inetserversocket.C listener.C logger.C memorypool.C passwdentry.C permissions.C protocolentry.C randomnumber.C regularexpression.C semaphoreset.C server.C serversocket.C sharedmemory.C signalclasses.C stringbuffer.C table.C text.C datatransport.C datatransportfactory.C unixclientsocket.C unixserversocket.C unixsocket.C variablebuffer.C xmldom.C xmldomnode.C xmlsax.C
LOBJS = commandline.lo connectiondata.lo daemonprocess.lo dtd.lo datetime.lo errorhandler.lo file.lo filedescriptor.lo groupentry.lo hostentry.lo inetclientsocket.lo inetserversocket.lo listener.lo logger.lo memorypool.lo passwdentry.lo permissions.lo protocolentry.lo randomnumber.lo regularexpression.lo semaphoreset.lo server.lo serversocket.lo sharedmemory.lo signalclasses.lo stringbuffer.lo table.lo text.lo datatransport.lo datatransportfactory.lo unixclientsocket.lo unixserversocket.lo unixsocket.lo variablebuffer.lo xmldom.lo xmldomnode.lo xmlsax.lo
PLOBJS = commandline.p.lo connectiondata.p.lo daemonprocess.p.lo dtd.p.lo datetime.p.lo errorhandler.p.lo file.p.lo filedescriptor.p.lo groupentry.p.lo hostentry.p.lo inetclientsocket.p.lo inetserversocket.p.lo listener.p.lo logger.p.lo memorypool.p.lo passwdentry.p.lo permissions.p.lo protocolentry.p.lo randomnumber.p.lo regularexpression.p.lo semaphoreset.p.lo server.p.lo serversocket.p.lo sharedmemory.p.lo signalclasses.p.lo stringbuffer.p.lo table.p.lo text.p.lo datatransport.p.lo datatransportfactory.p.lo unixclientsocket.p.lo unixserversocket.p.lo unixsocket.p.lo variablebuffer.p.lo xmldom.p.lo xmldomnode.p.lo xmlsax.p.lo

.SUFFIXES: .lo .p.lo

.C.lo:
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

librudiments.la: $(SRCS) $(LOBJS)
	$(LIBTOOL) --mode=link $(CXX) -o $@ $(LOBJS) -rpath $(libdir) -release $(VERSION) -version-info $(SONAME_VERSION_INFO)

librudiments_p.la: $(SRCS) $(PLOBJS)
	$(LIBTOOL) --mode=link $(CXX) -o $@ $(PLOBJS) $(DLLLIBS) -rpath $(libdir) -release $(VERSION) -version-info $(SONAME_VERSION_INFO)

install:
	$(top_builddir)/mkinstalldirs $(libdir)
	$(LIBTOOL) --mode=install $(INSTALL) -s librudiments.la $(libdir)/librudiments.la
	$(LIBTOOL) --mode=install $(INSTALL) librudiments_p.la $(libdir)/librudiments_p.la
	$(LIBTOOL) --mode=finish $(libdir)

uninstall:
	$(LIBTOOL) --mode=uninstall $(RM) $(libdir)/librudiments.la
	$(LIBTOOL) --mode=uninstall $(RM) $(libdir)/librudiments_p.la
