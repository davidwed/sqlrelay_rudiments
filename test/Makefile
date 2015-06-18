top_builddir = ..

include ../config.mk

CPPFLAGS = $(BASECPPFLAGS) $(INC)../ $(INC)../include
LIBS=$(RUDIMENTSLIB) $(SOCKETLIBS) $(PTHREADLIB) $(SSLLIBS) $(PCRELIBS) $(NANOSLEEPLIB) $(CLOCKNANOSLEEPLIB) $(CRYPTLIB) $(INETATONLIB) $(BELIB) $(GNULIB) $(CRTLIB) $(DLLIB) $(GETPGIDLIB) $(DEBUGLIB)

PROGS0 = server \
	listener \
	inetsocketclient \
	unixsocketclient \
	directory \
	daemon \
	groupentry \
	commandline \
	parameterstring \
	hostentry \
	userentry \
	serviceentry \
	permissions \
	protocolentry \
	regularexpression \
	rpcentry \
	semaphoreset1 \
	semaphoreset2 \
	charstring \
	datetime

PROGS1= dictionary \
	environment \
	file \
	filesystem \
	linkedlist \
	singlylinkedlist \
	listmetrics \
	memorypool \
	randomnumber \
	sharedmemory1 \
	sharedmemory2 \
	signal \
	kill \
	stringbuffer \
	bytebuffer \
	device \
	serialport \
	chat \
	crypt

PROGS2= dynamicarray \
	staticarray \
	dynamiclib \
	error \
	math \
	thread \
	threadmutex \
	process \
	serialportprofile \
	snooze \
	memorymap \
	character \
	bytestring \
	sslsvr \
	sslclnt \
	oneshotsvr \
	oneshotclnt \
	lock1 \
	lock2

PROGS3= handoff1 \
	handoff2 \
	handoffclient \
	nonhandoff1 \
	nonhandoff2 \
	nonhandoffclient \
	dtd \
	xmls \
	xmld \
	xmlentities \
	inetclntbench \
	inetsvrbench \
	nullserver \
	logger \
	sys \
	reboot \
	halt \
	shutdown

PROGS = $(PROGS0) $(PROGS1) $(PROGS2) $(PROGS3)

#ALLSTATIC=-all-static 

.SUFFIXES: .lo .obj

.cpp.lo:
	$(LTCOMPILE) $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $< $(OUT)$@

.cpp.obj:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $<

all: $(PROGS) server.pem

clean:
	$(LTCLEAN) $(RM) $(PROGS0)
	$(LTCLEAN) $(RM) $(PROGS1)
	$(LTCLEAN) $(RM) $(PROGS2)
	$(LTCLEAN) $(RM) $(PROGS3)
	$(LTCLEAN) $(RM) *.exe *.ilk *.idb *.pdb *.manifest *.lo *.o *.obj test.log testfile test.xml ca.pem server.pem client.pem
	cd certificates $(AND) $(MAKE) clean

server: server.cpp server.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) server.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

nullserver: nullserver.cpp nullserver.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) nullserver.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

sslsvr: sslsvr.cpp sslsvr.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) sslsvr.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

listener: listener.cpp listener.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) listener.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

inetsocketclient: inetsocketclient.cpp inetsocketclient.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) inetsocketclient.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

sslclnt: sslclnt.cpp sslclnt.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) sslclnt.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

unixsocketclient: unixsocketclient.cpp unixsocketclient.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) unixsocketclient.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

directory: directory.cpp directory.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) directory.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

daemon: daemon.cpp daemon.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) daemon.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

commandline: commandline.cpp commandline.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) commandline.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

parameterstring: parameterstring.cpp parameterstring.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) parameterstring.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

logger: logger.cpp logger.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) logger.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

environment: environment.cpp environment.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) environment.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

file: file.cpp file.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) file.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

filesystem: filesystem.cpp filesystem.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) filesystem.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

groupentry: groupentry.cpp groupentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) groupentry.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

hostentry: hostentry.cpp hostentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) hostentry.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

linkedlist: linkedlist.cpp linkedlist.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) linkedlist.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

singlylinkedlist: singlylinkedlist.cpp singlylinkedlist.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) singlylinkedlist.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

listmetrics: listmetrics.cpp listmetrics.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) listmetrics.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

lock1: lock1.cpp lock1.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) lock1.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

lock2: lock2.cpp lock2.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) lock2.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

memorypool: memorypool.cpp memorypool.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) memorypool.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

oneshotclnt: oneshotclnt.cpp oneshotclnt.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) oneshotclnt.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

oneshotsvr: oneshotsvr.cpp oneshotsvr.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) oneshotsvr.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

userentry: userentry.cpp userentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) userentry.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

serviceentry: serviceentry.cpp serviceentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) serviceentry.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

permissions: permissions.cpp permissions.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) permissions.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

protocolentry: protocolentry.cpp protocolentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) protocolentry.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

regularexpression: regularexpression.cpp regularexpression.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) regularexpression.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

rpcentry: rpcentry.cpp rpcentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) rpcentry.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

semaphoreset1: semaphoreset1.cpp semaphoreset1.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) semaphoreset1.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

semaphoreset2: semaphoreset2.cpp semaphoreset2.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) semaphoreset2.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

sharedmemory1: sharedmemory1.cpp sharedmemory1.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) sharedmemory1.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

sharedmemory2: sharedmemory2.cpp sharedmemory2.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) sharedmemory2.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

charstring: charstring.cpp charstring.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) charstring.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

datetime: datetime.cpp datetime.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) datetime.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

dictionary: dictionary.cpp dictionary.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) dictionary.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

dtd: dtd.cpp dtd.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) dtd.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

randomnumber: randomnumber.cpp randomnumber.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) randomnumber.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

signal: signal.cpp signal.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) signal.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

kill: kill.cpp kill.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) kill.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

stringbuffer: stringbuffer.cpp stringbuffer.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) stringbuffer.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

handoff1: handoff1.cpp handoff1.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) handoff1.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

handoff2: handoff2.cpp handoff2.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) handoff2.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

handoffclient: handoffclient.cpp handoffclient.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) handoffclient.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

nonhandoff1: nonhandoff1.cpp nonhandoff1.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) nonhandoff1.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

nonhandoff2: nonhandoff2.cpp nonhandoff2.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) nonhandoff2.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

nonhandoffclient: nonhandoffclient.cpp nonhandoffclient.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) nonhandoffclient.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

bytebuffer: bytebuffer.cpp bytebuffer.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) bytebuffer.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

xmls: xmls.cpp xmls.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) xmls.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

xmld: xmld.cpp xmld.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) xmld.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

xmlentities: xmlentities.cpp xmlentities.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) xmlentities.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

inetclntbench: inetclntbench.cpp inetclntbench.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) inetclntbench.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

inetsvrbench: inetsvrbench.cpp inetsvrbench.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) inetsvrbench.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

device: device.cpp device.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) device.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

serialport: serialport.cpp serialport.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) serialport.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

chat: chat.cpp chat.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) chat.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

crypt: crypt.cpp crypt.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) crypt.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

dynamicarray: dynamicarray.cpp dynamicarray.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) dynamicarray.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

staticarray: staticarray.cpp staticarray.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) staticarray.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

dynamiclib: dynamiclib.cpp dynamiclib.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) dynamiclib.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

error: error.cpp error.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) error.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

math: math.cpp math.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) math.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

thread: thread.cpp thread.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) thread.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

threadmutex: threadmutex.cpp threadmutex.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) threadmutex.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

process: process.cpp process.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) process.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

serialportprofile: serialportprofile.cpp serialportprofile.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) serialportprofile.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

snooze: snooze.cpp snooze.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) snooze.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

memorymap: memorymap.cpp memorymap.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) memorymap.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

character: character.cpp character.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) character.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

bytestring: bytestring.cpp bytestring.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) bytestring.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

sys: sys.cpp sys.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) sys.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

reboot: reboot.cpp reboot.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) reboot.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

halt: halt.cpp halt.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) halt.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

shutdown: shutdown.cpp shutdown.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) shutdown.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

server.pem:
	cd certificates $(AND) $(MAKE) clean all
	cp certificates/certificates/ca.pem .
	cp certificates/certificatechains/server.pem .
	cp certificates/certificatechains/client.pem .
