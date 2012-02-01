top_builddir = ..

include ../config.mk
include ../features.mk

.PHONY: all clean

CPPFLAGS = $(BASECPPFLAGS) -I../ -I../include
LIBS=-L../src -lrudiments $(SOCKETLIBS) $(PTHREADLIB) $(SSLLIBS) $(PCRELIBS) $(NANOSLEEPLIB) $(CLOCKNANOSLEEPLIB) $(SHMOPENLIB) $(CRYPTLIB) $(INETATONLIB) $(DLLIB)

#ALLSTATIC=-all-static

.SUFFIXES: .lo

PROGS =

ifeq ($(INCLUDE_DAEMONPROCESS),1)
ifeq ($(INCLUDE_PERMISSIONS),1)
ifeq ($(INCLUDE_INETSERVER),1)
ifeq ($(INCLUDE_FILE),1)
	PROGS += server$(EXE)
endif
endif
endif
endif

ifeq ($(INCLUDE_LISTENER),1)
ifeq ($(INCLUDE_INETSERVER),1)
ifeq ($(INCLUDE_UNIXSERVER),1)
ifeq ($(INCLUDE_PERMISSIONS),1)
	PROGS += listener$(EXE)
endif
endif
endif
endif

ifeq ($(INCLUDE_INETCLIENT),1)
ifeq ($(INCLUDE_ERROR),1)
	PROGS += inetclientsocket$(EXE)
endif
endif

ifeq ($(INCLUDE_UNIXCLIENT),1)
ifeq ($(INCLUDE_ERROR),1)
	PROGS += unixclientsocket$(EXE)
endif
endif


ifeq ($(INCLUDE_DIRECTORY),1)
ifeq ($(INCLUDE_FILE),1)
ifeq ($(INCLUDE_PERMISSIONS),1)
	PROGS += directory$(EXE)
endif
endif
endif

ifeq ($(INCLUDE_DAEMONPROCESS),1)
ifeq ($(INCLUDE_PERMISSIONS),1)
ifeq ($(INCLUDE_PROCESS),1)
ifeq ($(INCLUDE_FILE),1)
ifeq ($(INCLUDE_SNOOZE),1)
	PROGS += daemonprocess$(EXE)
endif
endif
endif
endif
endif

ifeq ($(INCLUDE_GROUPENTRY),1)
	PROGS += groupentry$(EXE)
endif

ifeq ($(INCLUDE_COMMANDLINE),1)
ifeq ($(INCLUDE_CHARSTRING),1)
	PROGS += commandline$(EXE)
endif
endif

ifeq ($(INCLUDE_PARAMETERSTRING),1)
	PROGS += parameterstring$(EXE)
endif

ifeq ($(INCLUDE_HOSTENTRY),1)
	PROGS += hostentry$(EXE)
endif

ifeq ($(INCLUDE_LOGGER),1)
	#PROGS += logger$(EXE)
endif

ifeq ($(INCLUDE_PASSWDENTRY),1)
	PROGS += passwdentry$(EXE)
endif

ifeq ($(INCLUDE_SERVICEENTRY),1)
	PROGS += serviceentry$(EXE)
endif

ifeq ($(INCLUDE_SHADOWENTRY),1)
	PROGS += shadowentry$(EXE)
endif

ifeq ($(INCLUDE_PERMISSIONS),1)
ifeq ($(INCLUDE_FILE),1)
	PROGS += permissions$(EXE)
endif
endif

ifeq ($(INCLUDE_PROTOCOLENTRY),1)
	PROGS += protocolentry$(EXE)
endif

ifeq ($(INCLUDE_REGULAREXPRESSION),1)
	PROGS += regularexpression$(EXE)
endif

ifeq ($(INCLUDE_RPCENTRY),1)
	PROGS += rpcentry$(EXE)
endif

ifeq ($(INCLUDE_SEMAPHORESET),1)
ifeq ($(INCLUDE_PERMISSIONS),1)
ifeq ($(INCLUDE_FILE),1)
	PROGS += semaphoreset1$(EXE)
	PROGS += semaphoreset2$(EXE)
endif
endif
endif

ifeq ($(INCLUDE_CHARSTRING),1)
	PROGS += charstring$(EXE)
endif

ifeq ($(INCLUDE_DATETIME),1)
	PROGS += datetime$(EXE)
endif

ifeq ($(INCLUDE_TIMEZONEFILE),1)
	PROGS += timezonefile$(EXE)
endif

PROGS += dictionary$(EXE)

ifeq ($(INCLUDE_ENVIRONMENT),1)
	PROGS += environment$(EXE)
endif

ifeq ($(INCLUDE_GROUPENTRY),1)
ifeq ($(INCLUDE_PASSWDENTRY),1)
ifeq ($(INCLUDE_FILE),1)
ifeq ($(INCLUDE_PERMISSIONS),1)
ifeq ($(INCLUDE_DATETIME),1)
	PROGS += file$(EXE)
endif
endif
endif
endif
endif

ifeq ($(INCLUDE_FILESYSTEM),1)
	PROGS += filesystem$(EXE)
endif

	PROGS += linkedlist$(EXE)

ifeq ($(INCLUDE_MEMORYPOOL),1)
	PROGS += memorypool$(EXE)
endif

ifeq ($(INCLUDE_DATETIME),1)
ifeq ($(INCLUDE_RANDOMNUMBER),1)
	PROGS += randomnumber$(EXE)
endif
endif

ifeq ($(INCLUDE_SHAREDMEMORY),1)
ifeq ($(INCLUDE_PERMISSIONS),1)
ifeq ($(INCLUDE_FILE),1)
ifeq ($(INCLUDE_SNOOZE),1)
	PROGS += sharedmemory1$(EXE)
	PROGS += sharedmemory2$(EXE)
endif
endif
endif
endif

ifeq ($(INCLUDE_SIGNALCLASSES),1)
	PROGS += signal$(EXE)
endif

ifeq ($(INCLUDE_STRINGBUFFER),1)
	PROGS += stringbuffer$(EXE)
endif

ifeq ($(INCLUDE_VARIABLEBUFFER),1)
ifeq ($(INCLUDE_RAWBUFFER),1)
	PROGS += variablebuffer$(EXE)
endif
endif

ifeq ($(INCLUDE_INTERVALTIMER),1)
ifeq ($(INCLUDE_SIGNALCLASSES),1)
ifeq ($(INCLUDE_SNOOZE),1)
	PROGS += intervaltimer$(EXE)
endif
endif
endif

ifeq ($(INCLUDE_DEVICE),1)
	PROGS += device$(EXE)
endif

ifeq ($(INCLUDE_SERIALPORT),1)
	PROGS += serialport$(EXE)
endif

ifeq ($(INCLUDE_SHMFILE),1)
	PROGS += shmfile$(EXE)
endif

ifeq ($(INCLUDE_CHAT),1)
	PROGS += chat$(EXE)
endif

ifeq ($(INCLUDE_CRYPT),1)
	PROGS += crypt$(EXE)
endif

ifeq ($(INCLUDE_DYNAMICLIB),1)
	PROGS += dynamiclib$(EXE)
endif

ifeq ($(INCLUDE_ERROR),1)
	PROGS += error$(EXE)
endif

ifeq ($(INCLUDE_MATH),1)
	PROGS += math$(EXE)
endif

ifeq ($(INCLUDE_MUTEX),1)
	PROGS += mutex$(EXE)
endif

ifeq ($(INCLUDE_PROCESS),1)
	PROGS += process$(EXE)
endif

ifeq ($(INCLUDE_SERIALPORT),1)
	PROGS += serialportprofile$(EXE)
endif

ifeq ($(INCLUDE_SNOOZE),1)
	PROGS += snooze$(EXE)
endif

ifeq ($(INCLUDE_MEMORYMAP),1)
	PROGS += memorymap$(EXE)
endif

ifeq ($(INCLUDE_CHARACTER),1)
	PROGS += character$(EXE)
endif

ifeq ($(INCLUDE_RAWBUFFER),1)
	PROGS += rawbuffer$(EXE)
endif

ifeq ($(INCLUDE_DAEMONPROCESS),1)
ifeq ($(INCLUDE_PERMISSIONS),1)
ifeq ($(INCLUDE_INETSERVER),1)
ifeq ($(INCLUDE_CHARSTRING),1)
ifeq ($(INCLUDE_ERROR),1)
ifeq ($(INCLUDE_FILE),1)
	PROGS += sslsvr$(EXE)
endif
endif
endif
endif
endif
endif

ifeq ($(INCLUDE_INETCLIENT),1)
ifeq ($(INCLUDE_CHARSTRING),1)
ifeq ($(INCLUDE_ERROR),1)
	PROGS += sslclnt$(EXE)
endif
endif
endif

ifeq ($(INCLUDE_DAEMONPROCESS),1)
ifeq ($(INCLUDE_PERMISSIONS),1)
ifeq ($(INCLUDE_INETSERVER),1)
ifeq ($(INCLUDE_SNOOZE),1)
ifeq ($(INCLUDE_ERROR),1)
ifeq ($(INCLUDE_FILE),1)
	PROGS += oneshotsvr$(EXE)
endif
endif
endif
endif
endif
endif

ifeq ($(INCLUDE_INETCLIENT),1)
	PROGS += oneshotclnt$(EXE)
endif

ifeq ($(INCLUDE_FILE),1)
ifeq ($(INCLUDE_PERMISSIONS),1)
ifeq ($(INCLUDE_SNOOZE),1)
	PROGS += lock1$(EXE)
endif
endif
endif

ifeq ($(INCLUDE_FILE),1)
ifeq ($(INCLUDE_SNOOZE),1)
	PROGS += lock2$(EXE)
endif
endif

ifeq ($(INCLUDE_LISTENER),1)
ifeq ($(INCLUDE_INETSERVER),1)
ifeq ($(INCLUDE_UNIXSERVER),1)
ifeq ($(INCLUDE_INETCLIENT),1)
ifeq ($(INCLUDE_UNIXCLIENT),1)
ifeq ($(INCLUDE_PERMISSIONS),1)
	PROGS += handoff1$(EXE)
endif
endif
endif
endif
endif
endif

ifeq ($(INCLUDE_UNIXCLIENT),1)
ifeq ($(INCLUDE_PERMISSIONS),1)
	PROGS += handoff2$(EXE)
endif
endif

ifeq ($(INCLUDE_INETCLIENT),1)
	PROGS += handoffclient$(EXE)
endif

ifeq ($(INCLUDE_INETSERVER),1)
ifeq ($(INCLUDE_PERMISSIONS),1)
	PROGS += nonhandoff1$(EXE)
endif
endif

ifeq ($(INCLUDE_INETSERVER),1)
ifeq ($(INCLUDE_PERMISSIONS),1)
	PROGS += nonhandoff2$(EXE)
endif
endif

ifeq ($(INCLUDE_INETCLIENTSOCKET),1)
	PROGS += nonhandoffclient$(EXE)
endif

ifeq ($(INCLUDE_FILE),1)
ifeq ($(INCLUDE_PERMISSIONS),1)
ifeq ($(INCLUDE_ERROR),1)
	PROGS += xattr$(EXE)
endif
endif
endif

ifeq ($(INCLUDE_DTD),1)
	PROGS += dtd$(EXE)
endif

ifeq ($(INCLUDE_XMLSAX),1)
	PROGS += xmls$(EXE)
endif

ifeq ($(INCLUDE_XMLDOM),1)
	PROGS += xmld$(EXE) xmlentities$(EXE)
endif

ifeq ($(INCLUDE_INETCLIENT),1)
ifeq ($(INCLUDE_ERROR),1)
	PROGS += inetclntbench$(EXE)
endif
endif

ifeq ($(INCLUDE_DAEMONPROCESS),1)
ifeq ($(INCLUDE_PERMISSIONS),1)
ifeq ($(INCLUDE_INETSERVER),1)
ifeq ($(INCLUDE_CHARSTRING),1)
ifeq ($(INCLUDE_FILE),1)
	PROGS += inetsvrbench$(EXE)
endif
endif
endif
endif
endif

ifeq ($(INCLUDE_DAEMONPROCESS),1)
ifeq ($(INCLUDE_PERMISSIONS),1)
ifeq ($(INCLUDE_INETSERVER),1)
ifeq ($(INCLUDE_FILE),1)
ifeq ($(INCLUDE_FILE),1)
	PROGS += nullserver$(EXE)
endif
endif
endif
endif
endif

ifeq ($(INCLUDE_INETSERVER),1)
	PROGS += server.pem
endif

all: $(PROGS)

clean:
	$(LIBTOOL) --mode=clean $(RM) ca.pem
	$(LIBTOOL) --mode=clean $(RM) server.pem
	$(LIBTOOL) --mode=clean $(RM) client.pem
	$(LIBTOOL) --mode=clean $(RM) *.lo
	$(LIBTOOL) --mode=clean $(RM) *.o
	$(LIBTOOL) --mode=clean $(RM) *.exe
	$(LIBTOOL) --mode=clean $(RM) listener server sslsvr inetclientsocket sslclnt unixclientsocket directory daemonprocess groupentry commandline parameterstring hostentry logger oneshotclnt oneshotsvr passwdentry serviceentry shadowentry permissions protocolentry regularexpression rpcentry semaphoreset1 semaphoreset2 sharedmemory1 sharedmemory2 charstring datetime timezonefile dictionary dtd environment file filesystem linkedlist lock1 lock2 memorypool randomnumber signal stringbuffer handoff1 handoff2 handoffclient nonhandoff1 nonhandoff2 nonhandoffclient variablebuffer xattr xmls xmld xmlentities intervaltimer inetclntbench inetsvrbench device serialport shmfile chat crypt dynamiclib error math mutex process serialportprofile snooze memorymap character rawbuffer nullserver
	$(RM) test.log
	$(RMTREE) .libs
	$(RM) testfile
	$(MAKE) -C certificates clean
	$(RM) test.xml

.cpp.lo:
	$(LIBTOOL) --mode=compile $(CXX) $(CXXFLAGS) $(CPPFLAGS) -g -c $< -o $@

server$(EXE): server.cpp server.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ server.lo $(ALLSTATIC) $(LIBS)

nullserver$(EXE): nullserver.cpp nullserver.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ nullserver.lo $(ALLSTATIC) $(LIBS)

sslsvr$(EXE): sslsvr.cpp sslsvr.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ sslsvr.lo $(ALLSTATIC) $(LIBS)

listener$(EXE): listener.cpp listener.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ listener.lo $(ALLSTATIC) $(LIBS)

inetclientsocket$(EXE): inetclientsocket.cpp inetclientsocket.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ inetclientsocket.lo $(ALLSTATIC) $(LIBS)

sslclnt$(EXE): sslclnt.cpp sslclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sslclnt.lo $(ALLSTATIC) $(LIBS)

unixclientsocket$(EXE): unixclientsocket.cpp unixclientsocket.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ unixclientsocket.lo $(ALLSTATIC) $(LIBS)

directory$(EXE): directory.cpp directory.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ directory.lo $(ALLSTATIC) $(LIBS)

daemonprocess$(EXE): daemonprocess.cpp daemonprocess.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ daemonprocess.lo $(ALLSTATIC) $(LIBS)

commandline$(EXE): commandline.cpp commandline.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ commandline.lo $(ALLSTATIC) $(LIBS)

parameterstring$(EXE): parameterstring.cpp parameterstring.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ parameterstring.lo $(ALLSTATIC) $(LIBS)

logger$(EXE): logger.cpp logger.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ logger.lo $(ALLSTATIC) $(LIBS)

environment$(EXE): environment.cpp environment.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ environment.lo $(ALLSTATIC) $(LIBS)

file$(EXE): file.cpp file.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ file.lo $(ALLSTATIC) $(LIBS)

filesystem$(EXE): filesystem.cpp filesystem.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ filesystem.lo $(ALLSTATIC) $(LIBS)

groupentry$(EXE): groupentry.cpp groupentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ groupentry.lo $(ALLSTATIC) $(LIBS)

hostentry$(EXE): hostentry.cpp hostentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ hostentry.lo $(ALLSTATIC) $(LIBS)

linkedlist$(EXE): linkedlist.cpp linkedlist.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ linkedlist.lo $(ALLSTATIC) $(LIBS)

lock1$(EXE): lock1.cpp lock1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ lock1.lo $(ALLSTATIC) $(LIBS)

lock2$(EXE): lock2.cpp lock2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ lock2.lo $(ALLSTATIC) $(LIBS)

memorypool$(EXE): memorypool.cpp memorypool.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ memorypool.lo $(ALLSTATIC) $(LIBS)

oneshotclnt$(EXE): oneshotclnt.cpp oneshotclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ oneshotclnt.lo $(ALLSTATIC) $(LIBS)

oneshotsvr$(EXE): oneshotsvr.cpp oneshotsvr.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ oneshotsvr.lo $(ALLSTATIC) $(LIBS)

passwdentry$(EXE): passwdentry.cpp passwdentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ passwdentry.lo $(ALLSTATIC) $(LIBS)

serviceentry$(EXE): serviceentry.cpp serviceentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ serviceentry.lo $(ALLSTATIC) $(LIBS)

shadowentry$(EXE): shadowentry.cpp shadowentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ shadowentry.lo $(ALLSTATIC) $(LIBS)

permissions$(EXE): permissions.cpp permissions.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ permissions.lo $(ALLSTATIC) $(LIBS)

protocolentry$(EXE): protocolentry.cpp protocolentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ protocolentry.lo $(ALLSTATIC) $(LIBS)

regularexpression$(EXE): regularexpression.cpp regularexpression.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ regularexpression.lo $(ALLSTATIC) $(LIBS)

rpcentry$(EXE): rpcentry.cpp rpcentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ rpcentry.lo $(ALLSTATIC) $(LIBS)

semaphoreset1$(EXE): semaphoreset1.cpp semaphoreset1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ semaphoreset1.lo $(ALLSTATIC) $(LIBS)

semaphoreset2$(EXE): semaphoreset2.cpp semaphoreset2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ semaphoreset2.lo $(ALLSTATIC) $(LIBS)

sharedmemory1$(EXE): sharedmemory1.cpp sharedmemory1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sharedmemory1.lo $(ALLSTATIC) $(LIBS)

sharedmemory2$(EXE): sharedmemory2.cpp sharedmemory2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sharedmemory2.lo $(ALLSTATIC) $(LIBS)

charstring$(EXE): charstring.cpp charstring.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ charstring.lo $(ALLSTATIC) $(LIBS)

datetime$(EXE): datetime.cpp datetime.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ datetime.lo $(ALLSTATIC) $(LIBS)

timezonefile$(EXE): timezonefile.cpp timezonefile.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ timezonefile.lo $(ALLSTATIC) $(LIBS)

dictionary$(EXE): dictionary.cpp dictionary.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ dictionary.lo $(ALLSTATIC) $(LIBS)

dtd$(EXE): dtd.cpp dtd.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ dtd.lo $(ALLSTATIC) $(LIBS)

randomnumber$(EXE): randomnumber.cpp randomnumber.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ randomnumber.lo $(ALLSTATIC) $(LIBS)

signal$(EXE): signal.cpp signal.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ signal.lo $(ALLSTATIC) $(LIBS)

stringbuffer$(EXE): stringbuffer.cpp stringbuffer.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ stringbuffer.lo $(ALLSTATIC) $(LIBS)

handoff1$(EXE): handoff1.cpp handoff1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ handoff1.lo $(ALLSTATIC) $(LIBS)

handoff2$(EXE): handoff2.cpp handoff2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ handoff2.lo $(ALLSTATIC) $(LIBS)

handoffclient$(EXE): handoffclient.cpp handoffclient.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ handoffclient.lo $(ALLSTATIC) $(LIBS)

nonhandoff1$(EXE): nonhandoff1.cpp nonhandoff1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ nonhandoff1.lo $(ALLSTATIC) $(LIBS)

nonhandoff2$(EXE): nonhandoff2.cpp nonhandoff2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ nonhandoff2.lo $(ALLSTATIC) $(LIBS)

nonhandoffclient$(EXE): nonhandoffclient.cpp nonhandoffclient.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ nonhandoffclient.lo $(ALLSTATIC) $(LIBS)

variablebuffer$(EXE): variablebuffer.cpp variablebuffer.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ variablebuffer.lo $(ALLSTATIC) $(LIBS)

xattr$(EXE): xattr.cpp xattr.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ xattr.lo $(ALLSTATIC) $(LIBS)

xmls$(EXE): xmls.cpp xmls.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ xmls.lo $(ALLSTATIC) $(LIBS)

xmld$(EXE): xmld.cpp xmld.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ xmld.lo $(ALLSTATIC) $(LIBS)

xmlentities$(EXE): xmlentities.cpp xmlentities.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ xmlentities.lo $(ALLSTATIC) $(LIBS)

intervaltimer$(EXE): intervaltimer.cpp intervaltimer.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ intervaltimer.lo $(ALLSTATIC) $(LIBS)

inetclntbench$(EXE): inetclntbench.cpp inetclntbench.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ inetclntbench.lo $(ALLSTATIC) $(LIBS)

inetsvrbench$(EXE): inetsvrbench.cpp inetsvrbench.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ inetsvrbench.lo $(ALLSTATIC) $(LIBS)

device$(EXE): device.cpp device.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ device.lo $(ALLSTATIC) $(LIBS)

serialport$(EXE): serialport.cpp serialport.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ serialport.lo $(ALLSTATIC) $(LIBS)

shmfile$(EXE): shmfile.cpp shmfile.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ shmfile.lo $(ALLSTATIC) $(LIBS)

chat$(EXE): chat.cpp chat.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ chat.lo $(ALLSTATIC) $(LIBS)

crypt$(EXE): crypt.cpp crypt.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ crypt.lo $(ALLSTATIC) $(LIBS)

dynamiclib$(EXE): dynamiclib.cpp dynamiclib.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ dynamiclib.lo $(ALLSTATIC) $(LIBS)

error$(EXE): error.cpp error.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ error.lo $(ALLSTATIC) $(LIBS)

math$(EXE): math.cpp math.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ math.lo $(ALLSTATIC) $(LIBS)

mutex$(EXE): mutex.cpp mutex.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ mutex.lo $(ALLSTATIC) $(LIBS)

process$(EXE): process.cpp process.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ process.lo $(ALLSTATIC) $(LIBS)

serialportprofile$(EXE): serialportprofile.cpp serialportprofile.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ serialportprofile.lo $(ALLSTATIC) $(LIBS)

snooze$(EXE): snooze.cpp snooze.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ snooze.lo $(ALLSTATIC) $(LIBS)

memorymap$(EXE): memorymap.cpp memorymap.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ memorymap.lo $(ALLSTATIC) $(LIBS)

character$(EXE): character.cpp character.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ character.lo $(ALLSTATIC) $(LIBS)

rawbuffer$(EXE): rawbuffer.cpp rawbuffer.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ rawbuffer.lo $(ALLSTATIC) $(LIBS)

server.pem:
	$(MAKE) -C certificates
	cp certificates/certificates/ca.pem .
	cp certificates/certificatechains/server.pem .
	cp certificates/certificatechains/client.pem .
