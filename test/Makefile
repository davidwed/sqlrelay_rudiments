top_builddir = ..

include ../config.mk

.PHONY: all clean

CPPFLAGS = $(BASECPPFLAGS) -I../ -I../include
LIBS=-L../src -lrudiments $(SOCKETLIBS) $(PTHREADLIB) $(SSLLIBS) $(PCRELIBS) $(NANOSLEEPLIB) $(CLOCKNANOSLEEPLIB) $(SHMOPENLIB) $(CRYPTLIB) $(INETATONLIB) $(BELIB) $(GNULIB) $(CRTLIB) $(DLLIB) $(DEBUGLIB)

PROGS = server$(EXE) \
	listener$(EXE) \
	inetsocketclient$(EXE) \
	unixsocketclient$(EXE) \
	directory$(EXE) \
	daemon$(EXE) \
	groupentry$(EXE) \
	commandline$(EXE) \
	parameterstring$(EXE) \
	hostentry$(EXE) \
	userentry$(EXE) \
	serviceentry$(EXE) \
	permissions$(EXE) \
	protocolentry$(EXE) \
	regularexpression$(EXE) \
	rpcentry$(EXE) \
	semaphoreset1$(EXE) \
	semaphoreset2$(EXE) \
	charstring$(EXE) \
	datetime$(EXE) \
	dictionary$(EXE) \
	environment$(EXE) \
	file$(EXE) \
	filesystem$(EXE) \
	linkedlist$(EXE) \
	singlylinkedlist$(EXE) \
	listmetrics$(EXE) \
	memorypool$(EXE) \
	randomnumber$(EXE) \
	sharedmemory1$(EXE) \
	sharedmemory2$(EXE) \
	signal$(EXE) \
	stringbuffer$(EXE) \
	bytebuffer$(EXE) \
	device$(EXE) \
	serialport$(EXE) \
	shmfile$(EXE) \
	chat$(EXE) \
	crypt$(EXE) \
	dynamiclib$(EXE) \
	error$(EXE) \
	math$(EXE) \
	thread$(EXE) \
	threadmutex$(EXE) \
	process$(EXE) \
	serialportprofile$(EXE) \
	snooze$(EXE) \
	memorymap$(EXE) \
	character$(EXE) \
	bytestring$(EXE) \
	sslsvr$(EXE) \
	sslclnt$(EXE) \
	oneshotsvr$(EXE) \
	oneshotclnt$(EXE) \
	lock1$(EXE) \
	lock2$(EXE) \
	handoff1$(EXE) \
	handoff2$(EXE) \
	handoffclient$(EXE) \
	nonhandoff1$(EXE) \
	nonhandoff2$(EXE) \
	nonhandoffclient$(EXE) \
	xattr$(EXE) \
	dtd$(EXE) \
	xmls$(EXE) \
	xmld$(EXE) xmlentities$(EXE) \
	inetclntbench$(EXE) \
	inetsvrbench$(EXE) \
	nullserver$(EXE) \
	logger$(EXE) \
	intervaltimer$(EXE) \
	sys$(EXE) \
	reboot$(EXE) \
	halt$(EXE) \
	shutdown$(EXE) \
	server.pem

#ALLSTATIC=-all-static

.SUFFIXES: .lo

.cpp.lo:
	$(LIBTOOL) --mode=compile $(CXX) $(CXXFLAGS) $(CPPFLAGS) -g -c $< -o $@

all: $(PROGS)

clean:
	$(LIBTOOL) --mode=clean $(RM) \
		listener$(EXE) \
		server$(EXE) \
		sslsvr$(EXE) \
		inetsocketclient$(EXE) \
		sslclnt$(EXE) \
		unixsocketclient$(EXE) \
		directory$(EXE) \
		daemon$(EXE) \
		groupentry$(EXE) \
		commandline$(EXE) \
		parameterstring$(EXE) \
		hostentry$(EXE) \
		logger$(EXE) \
		oneshotclnt$(EXE) \
		oneshotsvr$(EXE) \
		userentry$(EXE) \
		serviceentry$(EXE) \
		permissions$(EXE) \
		protocolentry$(EXE) \
		regularexpression$(EXE) \
		rpcentry$(EXE) \
		semaphoreset1$(EXE) \
		semaphoreset2$(EXE) \
		sharedmemory1$(EXE) \
		sharedmemory2$(EXE) \
		charstring$(EXE) \
		datetime$(EXE) \
		dictionary$(EXE) \
		dtd$(EXE) \
		environment$(EXE) \
		file$(EXE) \
		filesystem$(EXE) \
		linkedlist$(EXE) \
		singlylinkedlist$(EXE) \
		listmetrics$(EXE) \
		lock1$(EXE) \
		lock2$(EXE) \
		memorypool$(EXE) \
		randomnumber$(EXE) \
		signal$(EXE) \
		stringbuffer$(EXE) \
		handoff1$(EXE) \
		handoff2$(EXE) \
		handoffclient$(EXE) \
		nonhandoff1$(EXE) \
		nonhandoff2$(EXE) \
		nonhandoffclient$(EXE) \
		bytebuffer$(EXE) \
		xattr$(EXE) \
		xmls$(EXE) \
		xmld$(EXE) \
		xmlentities$(EXE) \
		intervaltimer$(EXE) \
		inetclntbench$(EXE) \
		inetsvrbench$(EXE) \
		device$(EXE) \
		serialport$(EXE) \
		shmfile$(EXE) \
		chat$(EXE) \
		crypt$(EXE) \
		dynamiclib$(EXE) \
		error$(EXE) \
		math$(EXE) \
		thread$(EXE) \
		threadmutex$(EXE) \
		process$(EXE) \
		serialportprofile$(EXE) \
		snooze$(EXE) \
		memorymap$(EXE) \
		character$(EXE) \
		bytestring$(EXE) \
		nullserver$(EXE) \
		sys$(EXE) \
		reboot$(EXE) \
		halt$(EXE) \
		shutdown$(EXE) \
		*.lo \
		*.o \
		test.log \
		testfile \
		test.xml \
		ca.pem \
		server.pem \
		client.pem
	$(RMTREE) .libs
	cd certificates; $(MAKE) clean
	$(RMTREE) msvc/test/Debug
	$(RMTREE) msvc/test/Release
	$(RMTREE) msvc/test/DebugCLR
	$(RMTREE) msvc/test/Release
	$(RMTREE) msvc/test/x64
	$(RMTREE) msvc/character/Debug
	$(RMTREE) msvc/character/Release
	$(RMTREE) msvc/character/DebugCLR
	$(RMTREE) msvc/character/Release
	$(RMTREE) msvc/character/x64

server$(EXE): server.cpp server.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ server.lo $(ALLSTATIC) $(LIBS)

nullserver$(EXE): nullserver.cpp nullserver.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ nullserver.lo $(ALLSTATIC) $(LIBS)

sslsvr$(EXE): sslsvr.cpp sslsvr.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ sslsvr.lo $(ALLSTATIC) $(LIBS)

listener$(EXE): listener.cpp listener.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ listener.lo $(ALLSTATIC) $(LIBS)

inetsocketclient$(EXE): inetsocketclient.cpp inetsocketclient.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ inetsocketclient.lo $(ALLSTATIC) $(LIBS)

sslclnt$(EXE): sslclnt.cpp sslclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sslclnt.lo $(ALLSTATIC) $(LIBS)

unixsocketclient$(EXE): unixsocketclient.cpp unixsocketclient.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ unixsocketclient.lo $(ALLSTATIC) $(LIBS)

directory$(EXE): directory.cpp directory.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ directory.lo $(ALLSTATIC) $(LIBS)

daemon$(EXE): daemon.cpp daemon.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ daemon.lo $(ALLSTATIC) $(LIBS)

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

singlylinkedlist$(EXE): singlylinkedlist.cpp singlylinkedlist.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ singlylinkedlist.lo $(ALLSTATIC) $(LIBS)

listmetrics$(EXE): listmetrics.cpp listmetrics.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ listmetrics.lo $(ALLSTATIC) $(LIBS)

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

userentry$(EXE): userentry.cpp userentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ userentry.lo $(ALLSTATIC) $(LIBS)

serviceentry$(EXE): serviceentry.cpp serviceentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ serviceentry.lo $(ALLSTATIC) $(LIBS)

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

bytebuffer$(EXE): bytebuffer.cpp bytebuffer.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ bytebuffer.lo $(ALLSTATIC) $(LIBS)

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

thread$(EXE): thread.cpp thread.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ thread.lo $(ALLSTATIC) $(LIBS)

threadmutex$(EXE): threadmutex.cpp threadmutex.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ threadmutex.lo $(ALLSTATIC) $(LIBS)

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

bytestring$(EXE): bytestring.cpp bytestring.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ bytestring.lo $(ALLSTATIC) $(LIBS)

sys$(EXE): sys.cpp sys.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sys.lo $(ALLSTATIC) $(LIBS)

reboot$(EXE): reboot.cpp reboot.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ reboot.lo $(ALLSTATIC) $(LIBS)

halt$(EXE): halt.cpp halt.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ halt.lo $(ALLSTATIC) $(LIBS)

shutdown$(EXE): shutdown.cpp shutdown.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ shutdown.lo $(ALLSTATIC) $(LIBS)

server.pem:
	cd certificates; $(MAKE) clean all
	cp certificates/certificates/ca.pem .
	cp certificates/certificatechains/server.pem .
	cp certificates/certificatechains/client.pem .
