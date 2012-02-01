top_builddir = ..

include ../config.mk
include ../features.mk

.PHONY: all clean install uninstall

CPPFLAGS = $(BASECPPFLAGS) -I../ -I../include

SRCS = namevaluepairs.cpp winsock.cpp
LOBJS = namevaluepairs.lo winsock.lo
ifeq ($(INCLUDE_CLIENT),1)
	SRCS += client.cpp
	LOBJS += client.lo
endif
ifeq ($(INCLUDE_COMMANDLINE),1)
	SRCS += commandline.cpp
	LOBJS += commandline.lo
endif
ifeq ($(INCLUDE_PARAMETERSTRING),1)
	SRCS += parameterstring.cpp
	LOBJS += parameterstring.lo
endif
ifeq ($(INCLUDE_DAEMONPROCESS),1)
	SRCS += daemonprocess.cpp
	LOBJS += daemonprocess.lo
endif
ifeq ($(INCLUDE_PROCESS),1)
	SRCS += process.cpp
	LOBJS += process.lo
endif
ifeq ($(INCLUDE_DATETIME),1)
	SRCS += datetime.cpp
	LOBJS += datetime.lo
endif
ifeq ($(INCLUDE_TIMEZONEFILE),1)
	SRCS += timezonefile.cpp
	LOBJS += timezonefile.lo
endif
ifeq ($(INCLUDE_DTD),1)
	SRCS += dtd.cpp
	LOBJS += dtd.lo
endif
ifeq ($(INCLUDE_ENVIRONMENT),1)
	SRCS += environment.cpp
	LOBJS += environment.lo
endif
ifeq ($(INCLUDE_FILEDESCRIPTOR),1)
	SRCS += filedescriptor.cpp
	LOBJS += filedescriptor.lo
endif
ifeq ($(INCLUDE_FILE),1)
	SRCS += file.cpp
	LOBJS += file.lo
endif
ifeq ($(INCLUDE_SHMFILE),1)
	SRCS += shmfile.cpp
	LOBJS += shmfile.lo
endif
ifeq ($(INCLUDE_DEVICE),1)
	SRCS += device.cpp
	LOBJS += device.lo
endif
ifeq ($(INCLUDE_FILESYSTEM),1)
	SRCS += filesystem.cpp
	LOBJS += filesystem.lo
endif
ifeq ($(INCLUDE_DIRECTORY),1)
	SRCS += directory.cpp
	LOBJS += directory.lo
endif
ifeq ($(INCLUDE_GROUPENTRY),1)
	SRCS += groupentry.cpp
	LOBJS += groupentry.lo
endif
ifeq ($(INCLUDE_HOSTENTRY),1)
	SRCS += hostentry.cpp
	LOBJS += hostentry.lo
endif
ifeq ($(INCLUDE_INETCLIENT),1)
	SRCS += inetclientsocket.cpp
	LOBJS += inetclientsocket.lo
	CPPFLAGS += -DINCLUDE_INETCLIENT
endif
ifeq ($(INCLUDE_INETSERVER),1)
	SRCS += inetserversocket.cpp
	LOBJS += inetserversocket.lo
	CPPFLAGS += -DINCLUDE_INETSERVER
endif
ifeq ($(INCLUDE_LISTENER),1)
	SRCS += listener.cpp
	LOBJS += listener.lo
endif
ifeq ($(INCLUDE_LOGGER),1)
	SRCS += logger.cpp
	LOBJS += logger.lo
endif
ifeq ($(INCLUDE_MEMORYPOOL),1)
	SRCS += memorypool.cpp
	LOBJS += memorypool.lo
endif
ifeq ($(INCLUDE_PASSWDENTRY),1)
	SRCS += passwdentry.cpp
	LOBJS += passwdentry.lo
endif
ifeq ($(INCLUDE_PERMISSIONS),1)
	SRCS += permissions.cpp
	LOBJS += permissions.lo
endif
ifeq ($(INCLUDE_PROTOCOLENTRY),1)
	SRCS += protocolentry.cpp
	LOBJS += protocolentry.lo
endif
ifeq ($(INCLUDE_RANDOMNUMBER),1)
	SRCS += randomnumber.cpp
	LOBJS += randomnumber.lo
endif
ifeq ($(INCLUDE_REGULAREXPRESSION),1)
	SRCS += regularexpression.cpp
	LOBJS += regularexpression.lo
endif
ifeq ($(USE_BUILT_IN_REGULAREXPRESSION),1)
	SRCS += regex.c
	LOBJS += regex.lo
endif
ifeq ($(INCLUDE_RPCENTRY),1)
	SRCS += rpcentry.cpp
	LOBJS += rpcentry.lo
endif
ifeq ($(INCLUDE_SEMAPHORESET),1)
	SRCS += semaphoreset.cpp
	LOBJS += semaphoreset.lo
endif
ifeq ($(INCLUDE_SERVER),1)
	SRCS += server.cpp
	LOBJS += server.lo
endif
ifeq ($(INCLUDE_SERVICEENTRY),1)
	SRCS += serviceentry.cpp
	LOBJS += serviceentry.lo
endif
ifeq ($(INCLUDE_SHADOWENTRY),1)
	SRCS += shadowentry.cpp
	LOBJS += shadowentry.lo
endif
ifeq ($(INCLUDE_SHAREDMEMORY),1)
	SRCS += sharedmemory.cpp
	LOBJS += sharedmemory.lo
endif
ifeq ($(INCLUDE_SIGNALCLASSES),1)
	SRCS += signalclasses.cpp
	LOBJS += signalclasses.lo
endif
ifeq ($(INCLUDE_CHARSTRING),1)
	SRCS += charstring.cpp
	LOBJS += charstring.lo
endif
ifeq ($(INCLUDE_CHARACTER),1)
	SRCS += character.cpp
	LOBJS += character.lo
endif
ifeq ($(INCLUDE_STRINGBUFFER),1)
	SRCS += stringbuffer.cpp
	LOBJS += stringbuffer.lo
endif
ifeq ($(INCLUDE_UNIXCLIENT),1)
	SRCS += unixclientsocket.cpp
	LOBJS += unixclientsocket.lo
	CPPFLAGS += -DINCLUDE_UNIXCLIENT
endif
ifeq ($(INCLUDE_MODEMCLIENT),1)
	SRCS += modemclient.cpp
	LOBJS += modemclient.lo
endif
ifeq ($(INCLUDE_MODEMSERVER),1)
	SRCS += modemserver.cpp
	LOBJS += modemserver.lo
	CPPFLAGS += -DINCLUDE_MODEMSERVER
endif
ifeq ($(INCLUDE_UNIXSERVER),1)
	SRCS += unixserversocket.cpp
	LOBJS += unixserversocket.lo
	CPPFLAGS += -DINCLUDE_UNIXSERVER
endif
ifeq ($(INCLUDE_VARIABLEBUFFER),1)
	SRCS += variablebuffer.cpp
	LOBJS += variablebuffer.lo
endif
ifeq ($(INCLUDE_XMLDOM),1)
	SRCS += xmldom.cpp xmldomnode.cpp
	LOBJS += xmldom.lo xmldomnode.lo
endif
ifeq ($(INCLUDE_XMLSAX),1)
	SRCS += xmlsax.cpp
	LOBJS += xmlsax.lo
endif
ifeq ($(INCLUDE_INTERVALTIMER),1)
	SRCS += intervaltimer.cpp
	LOBJS += intervaltimer.lo
endif
ifeq ($(INCLUDE_SNOOZE),1)
	SRCS += snooze.cpp
	LOBJS += snooze.lo
endif
ifeq ($(INCLUDE_DYNAMICLIB),1)
	SRCS += dynamiclib.cpp
	LOBJS += dynamiclib.lo
endif
ifeq ($(INCLUDE_CRYPT),1)
	SRCS += crypt.cpp
	LOBJS += crypt.lo
endif
ifeq ($(INCLUDE_MATH),1)
	SRCS += math.cpp
	LOBJS += math.lo
endif
ifeq ($(INCLUDE_RAWBUFFER),1)
	SRCS += rawbuffer.cpp
	LOBJS += rawbuffer.lo
endif
ifeq ($(INCLUDE_MEMORYMAP),1)
	SRCS += memorymap.cpp
	LOBJS += memorymap.lo
endif
ifeq ($(INCLUDE_MUTEX),1)
	SRCS += mutex.cpp
	LOBJS += mutex.lo
endif
ifeq ($(INCLUDE_SERIALPORTPROFILE),1)
	SRCS += serialportprofile.cpp
	LOBJS += serialportprofile.lo
endif
ifeq ($(INCLUDE_SERIALPORT),1)
	SRCS += serialport.cpp
	LOBJS += serialport.lo
endif
ifeq ($(INCLUDE_CHAT),1)
	SRCS += chat.cpp
	LOBJS += chat.lo
endif
ifeq ($(INCLUDE_ERROR),1)
	SRCS += error.cpp
	LOBJS += error.lo
endif

# if either modemserver or modemclient are defined, include modemutil.cpp
ifeq ($(INCLUDE_MODEMSERVER),1)
	SRCS += modemutil.cpp
	LOBJS += modemutil.lo
else
	ifeq ($(INCLUDE_MODEMCLIENT),1)
		SRCS += modemutil.cpp
		LOBJS += modemutil.lo
		CPPFLAGS += -DINCLUDE_MODEMCLIENT
	endif
endif

# if either inetserver or inetclient are defined, include inetsocketutil.cpp
ifeq ($(INCLUDE_INETSERVER),1)
	SRCS += inetsocketutil.cpp
	LOBJS += inetsocketutil.lo
else
	ifeq ($(INCLUDE_INETCLIENT),1)
		SRCS += inetsocketutil.cpp
		LOBJS += inetsocketutil.lo
	endif
endif

# if either unixserver or unixclient are defined, include unixsocketutil.cpp
ifeq ($(INCLUDE_UNIXSERVER),1)
	SRCS += unixsocketutil.cpp
	LOBJS += unixsocketutil.lo
else
	ifeq ($(INCLUDE_UNIXCLIENT),1)
		SRCS += unixsocketutil.cpp
		LOBJS += unixsocketutil.lo
	endif
endif

# if inet/unix server are defined, include serversocket.cpp
ifeq ($(INCLUDE_INETSERVER),1)
	SRCS += serversocket.cpp
	LOBJS += serversocket.lo
else
	ifeq ($(INCLUDE_UNIXSERVER),1)
		SRCS += serversocket.cpp
		LOBJS += serversocket.lo
	endif
endif

# if inet/unix client are defined, include clientsocket.cpp
ifeq ($(INCLUDE_INETCLIENT),1)
	SRCS += clientsocket.cpp
	LOBJS += clientsocket.lo
else
	ifeq ($(INCLUDE_UNIXCLIENT),1)
		SRCS += clientsocket.cpp
		LOBJS += clientsocket.lo
	endif
endif

# if any server and client are defined, include clientserverfactory
ifeq ($(INCLUDE_INETSERVER),1)
	ifeq ($(INCLUDE_INETCLIENT),1)
		SRCS += clientserverfactory.cpp
		LOBJS += clientserverfactory.lo
	else
		ifeq ($(INCLUDE_UNIXCLIENT),1)
			SRCS += clientserverfactory.cpp
			LOBJS += clientserverfactory.lo
		endif
	endif
else
	ifeq ($(INCLUDE_UNIXSERVER),1)
		ifeq ($(INCLUDE_INETCLIENT),1)
			SRCS += clientserverfactory.cpp
			LOBJS += clientserverfactory.lo
		else
			ifeq ($(INCLUDE_UNIXCLIENT),1)
				SRCS += clientserverfactory.cpp
				LOBJS += clientserverfactory.lo
			endif
		endif
	endif
endif
	
.SUFFIXES: .lo

.c.lo:
	$(LIBTOOL) --mode=compile $(CC) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

.cpp.lo:
	$(LIBTOOL) --mode=compile $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

all: librudiments.la

clean:
	$(LIBTOOL) --mode=clean $(RM) *.lo
	$(LIBTOOL) --mode=clean $(RM) *.la
	$(LIBTOOL) --mode=clean $(RM) *.o
	$(RMTREE) .libs

librudiments.la: $(SRCS) $(LOBJS)
	$(LIBTOOL) --mode=link $(CXX) -o $@ $(LOBJS) $(LDFLAGS) $(SOCKETLIBS) $(PTHREADLIB) $(SSLLIBS) $(PCRELIBS) $(NANOSLEEPLIB) $(CLOCKNANOSLEEPLIB) $(SHMOPENLIB) $(CRYPTLIB) $(INETATONLIB) $(NETAPI32LIB) $(DLLIB) -rpath $(libdir) -release $(RUDIMENTS_VERSION) -version-info $(SONAME_VERSION_INFO) -no-undefined

install:
	$(MKINSTALLDIRS) $(libdir)
	$(LTINSTALL) librudiments.la $(libdir)/librudiments.la
	$(LIBTOOL) --mode=finish $(libdir)

uninstall:
	$(LIBTOOL) --mode=uninstall $(RM) $(libdir)/librudiments.la
	$(LIBTOOL) --mode=uninstall $(RM) $(libdir)/librudiments_p.la
