top_builddir = ..

include ../config.mk
include ../features.mk

CPPFLAGS = $(BASECPPFLAGS) -I../ -I../include

SRCS = 
ifeq ($(INCLUDE_COMMANDLINE),1)
	SRCS += commandline.C
	LOBJS += commandline.lo
endif
ifeq ($(INCLUDE_PARAMETERSTRING),1)
	SRCS += parameterstring.C
	LOBJS += parameterstring.lo
endif
ifeq ($(INCLUDE_DAEMONPROCESS),1)
	SRCS += daemonprocess.C
	LOBJS += daemonprocess.lo
endif
ifeq ($(INCLUDE_DATETIME),1)
	SRCS += datetime.C
	LOBJS += datetime.lo
endif
ifeq ($(INCLUDE_TIMEZONEFILE),1)
	SRCS += timezonefile.C
	LOBJS += timezonefile.lo
endif
ifeq ($(INCLUDE_DTD),1)
	SRCS += dtd.C
	LOBJS += dtd.lo
endif
ifeq ($(INCLUDE_ERRORHANDLER),1)
	SRCS += errorhandler.C
	LOBJS += errorhandler.lo
endif
ifeq ($(INCLUDE_ENVIRONMENT),1)
	SRCS += environment.C
	LOBJS += environment.lo
endif
ifeq ($(INCLUDE_PATH),1)
	SRCS += path.C
	LOBJS += path.lo
endif
ifeq ($(INCLUDE_FILE),1)
	SRCS += file.C
	LOBJS += file.lo
endif
ifeq ($(INCLUDE_FILEDESCRIPTOR),1)
	SRCS += filedescriptor.C
	LOBJS += filedescriptor.lo
endif
ifeq ($(INCLUDE_FILESYSTEM),1)
	SRCS += filesystem.C
	LOBJS += filesystem.lo
endif
ifeq ($(INCLUDE_GROUPENTRY),1)
	SRCS += groupentry.C
	LOBJS += groupentry.lo
endif
ifeq ($(INCLUDE_HOSTENTRY),1)
	SRCS += hostentry.C
	LOBJS += hostentry.lo
endif
ifeq ($(INCLUDE_INETCLIENT),1)
	SRCS += inetclientsocket.C
	LOBJS += inetclientsocket.lo
endif
ifeq ($(INCLUDE_INETSERVER),1)
	SRCS += inetserversocket.C
	LOBJS += inetserversocket.lo
endif
ifeq ($(INCLUDE_LISTENER),1)
	SRCS += listener.C
	LOBJS += listener.lo
endif
ifeq ($(INCLUDE_LOGGER),1)
	SRCS += logger.C
	LOBJS += logger.lo
endif
ifeq ($(INCLUDE_MEMORYPOOL),1)
	SRCS += memorypool.C
	LOBJS += memorypool.lo
endif
ifeq ($(INCLUDE_PASSWDENTRY),1)
	SRCS += passwdentry.C
	LOBJS += passwdentry.lo
endif
ifeq ($(INCLUDE_PERMISSIONS),1)
	SRCS += permissions.C
	LOBJS += permissions.lo
endif
ifeq ($(INCLUDE_PROTOCOLENTRY),1)
	SRCS += protocolentry.C
	LOBJS += protocolentry.lo
endif
ifeq ($(INCLUDE_RANDOMNUMBER),1)
	SRCS += randomnumber.C
	LOBJS += randomnumber.lo
endif
ifeq ($(INCLUDE_REGULAREXPRESSION),1)
	SRCS += regularexpression.C
	LOBJS += regularexpression.lo
endif
ifeq ($(INCLUDE_RPCENTRY),1)
	SRCS += rpcentry.C
	LOBJS += rpcentry.lo
endif
ifeq ($(INCLUDE_SEMAPHORESET),1)
	SRCS += semaphoreset.C
	LOBJS += semaphoreset.lo
endif
ifeq ($(INCLUDE_SERVER),1)
	SRCS += server.C serversocket.C
	LOBJS += server.lo serversocket.lo
endif
ifeq ($(INCLUDE_SERVICEENTRY),1)
	SRCS += serviceentry.C
	LOBJS += serviceentry.lo
endif
ifeq ($(INCLUDE_SHADOWENTRY),1)
	SRCS += shadowentry.C
	LOBJS += shadowentry.lo
endif
ifeq ($(INCLUDE_SHAREDMEMORY),1)
	SRCS += sharedmemory.C
	LOBJS += sharedmemory.lo
endif
ifeq ($(INCLUDE_SIGNALCLASSES),1)
	SRCS += signalclasses.C
	LOBJS += signalclasses.lo
endif
ifeq ($(INCLUDE_CHARSTRING),1)
	SRCS += charstring.C
	LOBJS += charstring.lo
endif
ifeq ($(INCLUDE_STRINGBUFFER),1)
	SRCS += stringbuffer.C
	LOBJS += stringbuffer.lo
endif
ifeq ($(INCLUDE_DATATRANSPORT),1)
	SRCS += datatransport.C datatransportfactory.C
	LOBJS += datatransport.lo datatransportfactory.lo
endif
ifeq ($(INCLUDE_UNIXCLIENT),1)
	SRCS += unixclientsocket.C
	LOBJS += unixclientsocket.lo
endif
ifeq ($(INCLUDE_UNIXSERVER),1)
	SRCS += unixserversocket.C
	LOBJS += unixserversocket.lo
endif
ifeq ($(INCLUDE_VARIABLEBUFFER),1)
	SRCS += variablebuffer.C
	LOBJS += variablebuffer.lo
endif
ifeq ($(INCLUDE_XMLDOM),1)
	SRCS += xmldom.C xmldomnode.C
	LOBJS += xmldom.lo xmldomnode.lo
endif
ifeq ($(INCLUDE_XMLSAX),1)
	SRCS += xmlsax.C
	LOBJS += xmlsax.lo
endif

# if either inetserver or inetclient are defined, include inetsocket.h
ifeq ($(INCLUDE_INETSERVER),1)
	SRCS += inetsocket.C
	LOBJS += inetsocket.lo
else
	ifeq ($(INCLUDE_INETCLIENT),1)
		SRCS += inetsocket.C
		LOBJS += inetsocket.lo
	endif
endif

# if either unixserver or unixclient are defined, include unixsocket.h
ifeq ($(INCLUDE_UNIXSERVER),1)
	SRCS += unixsocket.C
	LOBJS += unixsocket.lo
else
	ifeq ($(INCLUDE_UNIXCLIENT),1)
		SRCS += unixsocket.C
		LOBJS += unixsocket.lo
	endif
endif

# if inet/unix server/socket are defined, include socket.h
ifeq ($(INCLUDE_INETSERVER),1)
	SRCS += socket.C
	LOBJS += socket.lo
else
	ifeq ($(INCLUDE_UNIXSERVER),1)
		SRCS += socket.C
		LOBJS += socket.lo
	else
		ifeq ($(INCLUDE_INETCLIENT),1)
			SRCS += socket.C
			LOBJS += socket.lo
		else
			ifeq ($(INCLUDE_UNIXCLIENT),1)
				SRCS += socket.C
				LOBJS += socket.lo
			endif
		endif
	endif
endif
	
.SUFFIXES: .lo

.C.lo:
	$(LIBTOOL) --mode=compile $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
	$(STRIPCOMMENTNOTE) $@

all: librudiments.la

clean:
	$(LIBTOOL) --mode=clean $(RM) *.lo
	$(LIBTOOL) --mode=clean $(RM) *.la
	$(LIBTOOL) --mode=clean $(RM) *.o
	$(RMTREE) .libs

rebuild: clean all

librudiments.la: $(SRCS) $(LOBJS)
	$(LIBTOOL) --mode=link $(CXX) -o $@ $(LOBJS) -rpath $(libdir) -release $(RUDIMENTS_VERSION) -version-info $(SONAME_VERSION_INFO) -no-undefined $(SSLLIBS) $(PCRELIBS)

install:
	$(MKINSTALLDIRS) $(libdir)
	$(INSTALL) $(INSTALLSTRIP) librudiments.la $(libdir)/librudiments.la
	$(LIBTOOL) --mode=finish $(libdir)

uninstall:
	$(LIBTOOL) --mode=uninstall $(RM) $(libdir)/librudiments.la
	$(LIBTOOL) --mode=uninstall $(RM) $(libdir)/librudiments_p.la
