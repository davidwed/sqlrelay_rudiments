include ../config.mk

top_builddir = ..
INSTALL = $(top_builddir)/install-sh -c

CPPFLAGS = $(BASECPPFLAGS) -I../ -I../include

SRCS = commandline.C connectiondata.C daemonprocess.C datetime.C errorhandler.C fileproperties.C filesystem.C inetclientsocket.C inetserversocket.C listener.C logger.C memorypool.C permissions.C randomnumber.C regularexpression.C semaphoreset.C serversocket.C sharedmemory.C signalclasses.C stringbuffer.C text.C transport.C unixclientsocket.C unixserversocket.C unixsocket.C variablebuffer.C xmldom.C xmldomnode.C xmlsax.C
LOBJS = commandline.lo connectiondata.lo daemonprocess.lo datetime.lo errorhandler.lo fileproperties.lo filesystem.lo inetclientsocket.lo inetserversocket.lo listener.lo logger.lo memorypool.lo permissions.lo randomnumber.lo regularexpression.lo semaphoreset.lo serversocket.lo sharedmemory.lo signalclasses.lo stringbuffer.lo text.lo transport.lo unixclientsocket.lo unixserversocket.lo unixsocket.lo variablebuffer.lo xmldom.lo xmldomnode.lo xmlsax.lo
PLOBJS = commandline.p.lo connectiondata.p.lo daemonprocess.p.lo datetime.p.lo errorhandler.p.lo fileproperties.p.lo filesystem.p.lo inetclientsocket.p.lo inetserversocket.p.lo listener.p.lo logger.p.lo memorypool.p.lo permissions.p.lo randomnumber.p.lo regularexpression.p.lo semaphoreset.p.lo serversocket.p.lo sharedmemory.p.lo signalclasses.p.lo stringbuffer.p.lo text.p.lo transport.p.lo unixclientsocket.p.lo unixserversocket.p.lo unixsocket.p.lo variablebuffer.p.lo xmldom.p.lo xmldomnode.p.lo xmlsax.p.lo

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
