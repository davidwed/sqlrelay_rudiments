top_builddir = ..

include ../config.mk

CPPFLAGS = $(BASECPPFLAGS) $(INC)../ $(INC)../include
LIBS=$(RUDIMENTSLIB) $(SOCKETLIBS) $(PTHREADLIB) $(SSLLIBS) $(PCRELIBS) $(NANOSLEEPLIB) $(CLOCKNANOSLEEPLIB) $(SHMOPENLIB) $(CRYPTLIB) $(INETATONLIB) $(BELIB) $(GNULIB) $(CRTLIB) $(DLLIB) $(DEBUGLIB)

PROGS = server \
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
	datetime \
	dictionary \
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
	shmfile \
	chat \
	crypt \
	dynamicarray \
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
	lock2 \
	handoff1 \
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
	intervaltimer \
	sys \
	reboot \
	halt \
	shutdown \
	server.pem

#ALLSTATIC=-all-static

.SUFFIXES: .lo .obj

.cpp.lo:
	$(LTCOMPILE) $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $< $(OUT)$@

.cpp.obj:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $<

all: $(PROGS)

clean:
	$(LTCLEAN) $(RM) $(PROGS) \
		*.exe \
		*.lo \
		*.o \
		*.obj \
		test.log \
		testfile \
		test.xml \
		ca.pem \
		server.pem \
		client.pem
	cd certificates $(AND) $(MAKE) clean
	cd msvc $(AND) cd test $(AND) $(RMTREE) Debug
	cd msvc $(AND) cd test $(AND) $(RMTREE) Release
	cd msvc $(AND) cd test $(AND) $(RMTREE) DebugCLR
	cd msvc $(AND) cd test $(AND) $(RMTREE) Release
	cd msvc $(AND) cd test $(AND) $(RMTREE) x64

server: server.cpp server.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) server.$(OBJ) $(ALLSTATIC) $(LIBS)

nullserver: nullserver.cpp nullserver.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) nullserver.$(OBJ) $(ALLSTATIC) $(LIBS)

sslsvr: sslsvr.cpp sslsvr.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) sslsvr.$(OBJ) $(ALLSTATIC) $(LIBS)

listener: listener.cpp listener.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) listener.$(OBJ) $(ALLSTATIC) $(LIBS)

inetsocketclient: inetsocketclient.cpp inetsocketclient.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) inetsocketclient.$(OBJ) $(ALLSTATIC) $(LIBS)

sslclnt: sslclnt.cpp sslclnt.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) sslclnt.$(OBJ) $(ALLSTATIC) $(LIBS)

unixsocketclient: unixsocketclient.cpp unixsocketclient.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) unixsocketclient.$(OBJ) $(ALLSTATIC) $(LIBS)

directory: directory.cpp directory.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) directory.$(OBJ) $(ALLSTATIC) $(LIBS)

daemon: daemon.cpp daemon.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) daemon.$(OBJ) $(ALLSTATIC) $(LIBS)

commandline: commandline.cpp commandline.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) commandline.$(OBJ) $(ALLSTATIC) $(LIBS)

parameterstring: parameterstring.cpp parameterstring.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) parameterstring.$(OBJ) $(ALLSTATIC) $(LIBS)

logger: logger.cpp logger.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) logger.$(OBJ) $(ALLSTATIC) $(LIBS)

environment: environment.cpp environment.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) environment.$(OBJ) $(ALLSTATIC) $(LIBS)

file: file.cpp file.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) file.$(OBJ) $(ALLSTATIC) $(LIBS)

filesystem: filesystem.cpp filesystem.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) filesystem.$(OBJ) $(ALLSTATIC) $(LIBS)

groupentry: groupentry.cpp groupentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) groupentry.$(OBJ) $(ALLSTATIC) $(LIBS)

hostentry: hostentry.cpp hostentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) hostentry.$(OBJ) $(ALLSTATIC) $(LIBS)

linkedlist: linkedlist.cpp linkedlist.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) linkedlist.$(OBJ) $(ALLSTATIC) $(LIBS)

singlylinkedlist: singlylinkedlist.cpp singlylinkedlist.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) singlylinkedlist.$(OBJ) $(ALLSTATIC) $(LIBS)

listmetrics: listmetrics.cpp listmetrics.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) listmetrics.$(OBJ) $(ALLSTATIC) $(LIBS)

lock1: lock1.cpp lock1.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) lock1.$(OBJ) $(ALLSTATIC) $(LIBS)

lock2: lock2.cpp lock2.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) lock2.$(OBJ) $(ALLSTATIC) $(LIBS)

memorypool: memorypool.cpp memorypool.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) memorypool.$(OBJ) $(ALLSTATIC) $(LIBS)

oneshotclnt: oneshotclnt.cpp oneshotclnt.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) oneshotclnt.$(OBJ) $(ALLSTATIC) $(LIBS)

oneshotsvr: oneshotsvr.cpp oneshotsvr.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) oneshotsvr.$(OBJ) $(ALLSTATIC) $(LIBS)

userentry: userentry.cpp userentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) userentry.$(OBJ) $(ALLSTATIC) $(LIBS)

serviceentry: serviceentry.cpp serviceentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) serviceentry.$(OBJ) $(ALLSTATIC) $(LIBS)

permissions: permissions.cpp permissions.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) permissions.$(OBJ) $(ALLSTATIC) $(LIBS)

protocolentry: protocolentry.cpp protocolentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) protocolentry.$(OBJ) $(ALLSTATIC) $(LIBS)

regularexpression: regularexpression.cpp regularexpression.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) regularexpression.$(OBJ) $(ALLSTATIC) $(LIBS)

rpcentry: rpcentry.cpp rpcentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) rpcentry.$(OBJ) $(ALLSTATIC) $(LIBS)

semaphoreset1: semaphoreset1.cpp semaphoreset1.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) semaphoreset1.$(OBJ) $(ALLSTATIC) $(LIBS)

semaphoreset2: semaphoreset2.cpp semaphoreset2.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) semaphoreset2.$(OBJ) $(ALLSTATIC) $(LIBS)

sharedmemory1: sharedmemory1.cpp sharedmemory1.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) sharedmemory1.$(OBJ) $(ALLSTATIC) $(LIBS)

sharedmemory2: sharedmemory2.cpp sharedmemory2.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) sharedmemory2.$(OBJ) $(ALLSTATIC) $(LIBS)

charstring: charstring.cpp charstring.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) charstring.$(OBJ) $(ALLSTATIC) $(LIBS)

datetime: datetime.cpp datetime.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) datetime.$(OBJ) $(ALLSTATIC) $(LIBS)

dictionary: dictionary.cpp dictionary.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) dictionary.$(OBJ) $(ALLSTATIC) $(LIBS)

dtd: dtd.cpp dtd.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) dtd.$(OBJ) $(ALLSTATIC) $(LIBS)

randomnumber: randomnumber.cpp randomnumber.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) randomnumber.$(OBJ) $(ALLSTATIC) $(LIBS)

signal: signal.cpp signal.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) signal.$(OBJ) $(ALLSTATIC) $(LIBS)

kill: kill.cpp kill.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) kill.$(OBJ) $(ALLSTATIC) $(LIBS)

stringbuffer: stringbuffer.cpp stringbuffer.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) stringbuffer.$(OBJ) $(ALLSTATIC) $(LIBS)

handoff1: handoff1.cpp handoff1.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) handoff1.$(OBJ) $(ALLSTATIC) $(LIBS)

handoff2: handoff2.cpp handoff2.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) handoff2.$(OBJ) $(ALLSTATIC) $(LIBS)

handoffclient: handoffclient.cpp handoffclient.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) handoffclient.$(OBJ) $(ALLSTATIC) $(LIBS)

nonhandoff1: nonhandoff1.cpp nonhandoff1.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) nonhandoff1.$(OBJ) $(ALLSTATIC) $(LIBS)

nonhandoff2: nonhandoff2.cpp nonhandoff2.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) nonhandoff2.$(OBJ) $(ALLSTATIC) $(LIBS)

nonhandoffclient: nonhandoffclient.cpp nonhandoffclient.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) nonhandoffclient.$(OBJ) $(ALLSTATIC) $(LIBS)

bytebuffer: bytebuffer.cpp bytebuffer.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) bytebuffer.$(OBJ) $(ALLSTATIC) $(LIBS)

xmls: xmls.cpp xmls.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) xmls.$(OBJ) $(ALLSTATIC) $(LIBS)

xmld: xmld.cpp xmld.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) xmld.$(OBJ) $(ALLSTATIC) $(LIBS)

xmlentities: xmlentities.cpp xmlentities.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) xmlentities.$(OBJ) $(ALLSTATIC) $(LIBS)

intervaltimer: intervaltimer.cpp intervaltimer.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) intervaltimer.$(OBJ) $(ALLSTATIC) $(LIBS)

inetclntbench: inetclntbench.cpp inetclntbench.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) inetclntbench.$(OBJ) $(ALLSTATIC) $(LIBS)

inetsvrbench: inetsvrbench.cpp inetsvrbench.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) inetsvrbench.$(OBJ) $(ALLSTATIC) $(LIBS)

device: device.cpp device.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) device.$(OBJ) $(ALLSTATIC) $(LIBS)

serialport: serialport.cpp serialport.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) serialport.$(OBJ) $(ALLSTATIC) $(LIBS)

shmfile: shmfile.cpp shmfile.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) shmfile.$(OBJ) $(ALLSTATIC) $(LIBS)

chat: chat.cpp chat.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) chat.$(OBJ) $(ALLSTATIC) $(LIBS)

crypt: crypt.cpp crypt.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) crypt.$(OBJ) $(ALLSTATIC) $(LIBS)

dynamicarray: dynamicarray.cpp dynamicarray.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) dynamicarray.$(OBJ) $(ALLSTATIC) $(LIBS)

staticarray: staticarray.cpp staticarray.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) staticarray.$(OBJ) $(ALLSTATIC) $(LIBS)

dynamiclib: dynamiclib.cpp dynamiclib.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) dynamiclib.$(OBJ) $(ALLSTATIC) $(LIBS)

error: error.cpp error.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) error.$(OBJ) $(ALLSTATIC) $(LIBS)

math: math.cpp math.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) math.$(OBJ) $(ALLSTATIC) $(LIBS)

thread: thread.cpp thread.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) thread.$(OBJ) $(ALLSTATIC) $(LIBS)

threadmutex: threadmutex.cpp threadmutex.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) threadmutex.$(OBJ) $(ALLSTATIC) $(LIBS)

process: process.cpp process.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) process.$(OBJ) $(ALLSTATIC) $(LIBS)

serialportprofile: serialportprofile.cpp serialportprofile.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) serialportprofile.$(OBJ) $(ALLSTATIC) $(LIBS)

snooze: snooze.cpp snooze.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) snooze.$(OBJ) $(ALLSTATIC) $(LIBS)

memorymap: memorymap.cpp memorymap.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) memorymap.$(OBJ) $(ALLSTATIC) $(LIBS)

character: character.cpp character.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) character.$(OBJ) $(ALLSTATIC) $(LIBS)

bytestring: bytestring.cpp bytestring.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) bytestring.$(OBJ) $(ALLSTATIC) $(LIBS)

sys: sys.cpp sys.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) sys.$(OBJ) $(ALLSTATIC) $(LIBS)

reboot: reboot.cpp reboot.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) reboot.$(OBJ) $(ALLSTATIC) $(LIBS)

halt: halt.cpp halt.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) halt.$(OBJ) $(ALLSTATIC) $(LIBS)

shutdown: shutdown.cpp shutdown.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) shutdown.$(OBJ) $(ALLSTATIC) $(LIBS)

server.pem:
	cd certificates $(AND) $(MAKE) clean all
	cp certificates/certificates/ca.pem .
	cp certificates/certificatechains/server.pem .
	cp certificates/certificatechains/client.pem .
