include ../config.mk
include ../features.mk

top_builddir = ..
INSTALL = $(top_builddir)/install-sh -c

CPPFLAGS = $(BASECPPFLAGS) -I../ -I../include

SRCS = 
ifeq ($(INCLUDE_COMMANDLINE),1)
	SRCS += commandline.C
	NLOBJS += commandline.n.lo
	PLOBJS += commandline.p.lo
endif
ifeq ($(INCLUDE_PARAMETERSTRING),1)
	SRCS += parameterstring.C
	NLOBJS += parameterstring.n.lo
	PLOBJS += parameterstring.p.lo
endif
ifeq ($(INCLUDE_DAEMONPROCESS),1)
	SRCS += daemonprocess.C
	NLOBJS += daemonprocess.n.lo
	PLOBJS += daemonprocess.p.lo
endif
ifeq ($(INCLUDE_DATETIME),1)
	SRCS += datetime.C timezonefile.C
	NLOBJS += datetime.n.lo timezonefile.n.lo
	PLOBJS += datetime.p.lo timezonefile.p.lo
endif
ifeq ($(INCLUDE_DTD),1)
	SRCS += dtd.C
	NLOBJS += dtd.n.lo
	PLOBJS += dtd.p.lo
endif
ifeq ($(INCLUDE_ERRORHANDLER),1)
	SRCS += errorhandler.C
	NLOBJS += errorhandler.n.lo
	PLOBJS += errorhandler.p.lo
endif
ifeq ($(INCLUDE_ENVIRONMENT),1)
	SRCS += environment.C
	NLOBJS += environment.n.lo
	PLOBJS += environment.p.lo
endif
ifeq ($(INCLUDE_FILE),1)
	SRCS += file.C
	NLOBJS += file.n.lo
	PLOBJS += file.p.lo
endif
ifeq ($(INCLUDE_FILEDESCRIPTOR),1)
	SRCS += filedescriptor.C
	NLOBJS += filedescriptor.n.lo
	PLOBJS += filedescriptor.p.lo
endif
ifeq ($(INCLUDE_FILESYSTEM),1)
	SRCS += filesystem.C
	NLOBJS += filesystem.n.lo
	PLOBJS += filesystem.p.lo
endif
ifeq ($(INCLUDE_GROUPENTRY),1)
	SRCS += groupentry.C
	NLOBJS += groupentry.n.lo
	PLOBJS += groupentry.p.lo
endif
ifeq ($(INCLUDE_HOSTENTRY),1)
	SRCS += hostentry.C
	NLOBJS += hostentry.n.lo
	PLOBJS += hostentry.p.lo
endif
ifeq ($(INCLUDE_INETCLIENT),1)
	SRCS += inetclientsocket.C
	NLOBJS += inetclientsocket.n.lo
	PLOBJS += inetclientsocket.p.lo
endif
ifeq ($(INCLUDE_INETSERVER),1)
	SRCS += inetsocket.C inetserversocket.C
	NLOBJS += inetsocket.n.lo inetserversocket.n.lo
	PLOBJS += inetsocket.p.lo inetserversocket.p.lo
endif
ifeq ($(INCLUDE_LISTENER),1)
	SRCS += listener.C
	NLOBJS += listener.n.lo
	PLOBJS += listener.p.lo
endif
ifeq ($(INCLUDE_LOGGER),1)
	SRCS += logger.C
	NLOBJS += logger.n.lo
	PLOBJS += logger.p.lo
endif
ifeq ($(INCLUDE_MEMORYPOOL),1)
	SRCS += memorypool.C
	NLOBJS += memorypool.n.lo
	PLOBJS += memorypool.p.lo
endif
ifeq ($(INCLUDE_PASSWDENTRY),1)
	SRCS += passwdentry.C
	NLOBJS += passwdentry.n.lo
	PLOBJS += passwdentry.p.lo
endif
ifeq ($(INCLUDE_PERMISSIONS),1)
	SRCS += permissions.C
	NLOBJS += permissions.n.lo
	PLOBJS += permissions.p.lo
endif
ifeq ($(INCLUDE_PROTOCOLENTRY),1)
	SRCS += protocolentry.C
	NLOBJS += protocolentry.n.lo
	PLOBJS += protocolentry.p.lo
endif
ifeq ($(INCLUDE_RANDOMNUMBER),1)
	SRCS += randomnumber.C
	NLOBJS += randomnumber.n.lo
	PLOBJS += randomnumber.p.lo
endif
ifeq ($(INCLUDE_REGULAREXPRESSION),1)
	SRCS += regularexpression.C
	NLOBJS += regularexpression.n.lo
	PLOBJS += regularexpression.p.lo
endif
ifeq ($(INCLUDE_RPCENTRY),1)
	SRCS += rpcentry.C
	NLOBJS += rpcentry.n.lo
	PLOBJS += rpcentry.p.lo
endif
ifeq ($(INCLUDE_SEMAPHORESET),1)
	SRCS += semaphoreset.C
	NLOBJS += semaphoreset.n.lo
	PLOBJS += semaphoreset.p.lo
endif
ifeq ($(INCLUDE_SERVER),1)
	SRCS += server.C serversocket.C
	NLOBJS += server.n.lo serversocket.n.lo
	PLOBJS += server.p.lo serversocket.p.lo
endif
ifeq ($(INCLUDE_SERVICEENTRY),1)
	SRCS += serviceentry.C
	NLOBJS += serviceentry.n.lo
	PLOBJS += serviceentry.p.lo
endif
ifeq ($(INCLUDE_SHADOWENTRY),1)
	SRCS += shadowentry.C
	NLOBJS += shadowentry.n.lo
	PLOBJS += shadowentry.p.lo
endif
ifeq ($(INCLUDE_SHAREDMEMORY),1)
	SRCS += sharedmemory.C
	NLOBJS += sharedmemory.n.lo
	PLOBJS += sharedmemory.p.lo
endif
ifeq ($(INCLUDE_SIGNALCLASSES),1)
	SRCS += signalclasses.C
	NLOBJS += signalclasses.n.lo
	PLOBJS += signalclasses.p.lo
endif
ifeq ($(INCLUDE_STRING),1)
	SRCS += string.C
	NLOBJS += string.n.lo
	PLOBJS += string.p.lo
endif
ifeq ($(INCLUDE_STRINGBUFFER),1)
	SRCS += stringbuffer.C
	NLOBJS += stringbuffer.n.lo
	PLOBJS += stringbuffer.p.lo
endif
ifeq ($(INCLUDE_DATATRANSPORT),1)
	SRCS += datatransport.C datatransportfactory.C
	NLOBJS += datatransport.n.lo datatransportfactory.n.lo
	PLOBJS += datatransport.p.lo datatransportfactory.p.lo
endif
ifeq ($(INCLUDE_UNIXCLIENT),1)
	SRCS += unixclientsocket.C
	NLOBJS += unixclientsocket.n.lo
	PLOBJS += unixclientsocket.p.lo
endif
ifeq ($(INCLUDE_UNIXSERVER),1)
	SRCS += unixsocket.C unixserversocket.C
	NLOBJS += unixsocket.n.lo unixserversocket.n.lo
	PLOBJS += unixsocket.p.lo unixserversocket.p.lo
endif
ifeq ($(INCLUDE_VARIABLEBUFFER),1)
	SRCS += variablebuffer.C
	NLOBJS += variablebuffer.n.lo
	PLOBJS += variablebuffer.p.lo
endif
ifeq ($(INCLUDE_XMLDOM),1)
	SRCS += xmldom.C xmldomnode.C
	NLOBJS += xmldom.n.lo xmldomnode.n.lo
	PLOBJS += xmldom.p.lo xmldomnode.p.lo
endif
ifeq ($(INCLUDE_XMLSAX),1)
	SRCS += xmlsax.C
	NLOBJS += xmlsax.n.lo
	PLOBJS += xmlsax.p.lo
endif
	
.SUFFIXES: .n.lo .p.lo

.C.n.lo:
	$(LIBTOOL) --mode=compile $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
	if ( test "$(STRIP)" != "touch" ); then \
		$(STRIP) -x -R .note -R .comment $@; \
	fi

.C.p.lo:
	$(LIBTOOL) --mode=compile $(CXX) $(DEBUGCXXFLAGS) $(CPPFLAGS) -c $< -o $@

#all: librudiments.la librudiments_p.la
all: librudiments.la

clean:
	$(LIBTOOL) --mode=clean $(RM) *.lo
	$(LIBTOOL) --mode=clean $(RM) *.la
	$(LIBTOOL) --mode=clean $(RM) *.o
	$(RMTREE) .libs

rebuild: clean all

librudiments.la: $(SRCS) $(NLOBJS)
	$(LIBTOOL) --mode=link $(CXX) -o $@ $(NLOBJS) -rpath $(libdir) -release $(RUDIMENTS_VERSION) -version-info $(SONAME_VERSION_INFO)

librudiments_p.la: $(SRCS) $(PLOBJS)
	$(LIBTOOL) --mode=link $(CXX) -o $@ $(PLOBJS) -rpath $(libdir) -release $(RUDIMENTS_VERSION) -version-info $(SONAME_VERSION_INFO)

install:
	$(top_builddir)/mkinstalldirs $(libdir)
	$(LIBTOOL) --mode=install $(INSTALL) -s librudiments.la $(libdir)/librudiments.la
	#$(LIBTOOL) --mode=install $(INSTALL) librudiments_p.la $(libdir)/librudiments_p.la
	$(LIBTOOL) --mode=finish $(libdir)

uninstall:
	$(LIBTOOL) --mode=uninstall $(RM) $(libdir)/librudiments.la
	$(LIBTOOL) --mode=uninstall $(RM) $(libdir)/librudiments_p.la
