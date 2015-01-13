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
	kill$(EXE) \
	stringbuffer$(EXE) \
	bytebuffer$(EXE) \
	device$(EXE) \
	serialport$(EXE) \
	shmfile$(EXE) \
	chat$(EXE) \
	crypt$(EXE) \
	dynamicarray$(EXE) \
	staticarray$(EXE) \
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
	$(LTCOMPILE) $(CXX) $(CXXFLAGS) $(CPPFLAGS) -g -c $< -o $@

all: $(PROGS)

clean:
	$(LTCLEAN) $(RM) \
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
		kill$(EXE) \
		stringbuffer$(EXE) \
		handoff1$(EXE) \
		handoff2$(EXE) \
		handoffclient$(EXE) \
		nonhandoff1$(EXE) \
		nonhandoff2$(EXE) \
		nonhandoffclient$(EXE) \
		bytebuffer$(EXE) \
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
		dynamicarray$(EXE) \
		staticarray$(EXE) \
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
	$(LTLINK) $(CXX) -o $@ server.lo $(ALLSTATIC) $(LIBS)

nullserver$(EXE): nullserver.cpp nullserver.lo 
	$(LTLINK) $(CXX) -o $@ nullserver.lo $(ALLSTATIC) $(LIBS)

sslsvr$(EXE): sslsvr.cpp sslsvr.lo 
	$(LTLINK) $(CXX) -o $@ sslsvr.lo $(ALLSTATIC) $(LIBS)

listener$(EXE): listener.cpp listener.lo
	$(LTLINK) $(CXX) -o $@ listener.lo $(ALLSTATIC) $(LIBS)

inetsocketclient$(EXE): inetsocketclient.cpp inetsocketclient.lo
	$(LTLINK) $(CXX) -o $@ inetsocketclient.lo $(ALLSTATIC) $(LIBS)

sslclnt$(EXE): sslclnt.cpp sslclnt.lo
	$(LTLINK) $(CXX) -o $@ sslclnt.lo $(ALLSTATIC) $(LIBS)

unixsocketclient$(EXE): unixsocketclient.cpp unixsocketclient.lo
	$(LTLINK) $(CXX) -o $@ unixsocketclient.lo $(ALLSTATIC) $(LIBS)

directory$(EXE): directory.cpp directory.lo 
	$(LTLINK) $(CXX) -o $@ directory.lo $(ALLSTATIC) $(LIBS)

daemon$(EXE): daemon.cpp daemon.lo 
	$(LTLINK) $(CXX) -o $@ daemon.lo $(ALLSTATIC) $(LIBS)

commandline$(EXE): commandline.cpp commandline.lo
	$(LTLINK) $(CXX) -o $@ commandline.lo $(ALLSTATIC) $(LIBS)

parameterstring$(EXE): parameterstring.cpp parameterstring.lo
	$(LTLINK) $(CXX) -o $@ parameterstring.lo $(ALLSTATIC) $(LIBS)

logger$(EXE): logger.cpp logger.lo
	$(LTLINK) $(CXX) -o $@ logger.lo $(ALLSTATIC) $(LIBS)

environment$(EXE): environment.cpp environment.lo
	$(LTLINK) $(CXX) -o $@ environment.lo $(ALLSTATIC) $(LIBS)

file$(EXE): file.cpp file.lo
	$(LTLINK) $(CXX) -o $@ file.lo $(ALLSTATIC) $(LIBS)

filesystem$(EXE): filesystem.cpp filesystem.lo
	$(LTLINK) $(CXX) -o $@ filesystem.lo $(ALLSTATIC) $(LIBS)

groupentry$(EXE): groupentry.cpp groupentry.lo
	$(LTLINK) $(CXX) -o $@ groupentry.lo $(ALLSTATIC) $(LIBS)

hostentry$(EXE): hostentry.cpp hostentry.lo
	$(LTLINK) $(CXX) -o $@ hostentry.lo $(ALLSTATIC) $(LIBS)

linkedlist$(EXE): linkedlist.cpp linkedlist.lo
	$(LTLINK) $(CXX) -o $@ linkedlist.lo $(ALLSTATIC) $(LIBS)

singlylinkedlist$(EXE): singlylinkedlist.cpp singlylinkedlist.lo
	$(LTLINK) $(CXX) -o $@ singlylinkedlist.lo $(ALLSTATIC) $(LIBS)

listmetrics$(EXE): listmetrics.cpp listmetrics.lo
	$(LTLINK) $(CXX) -o $@ listmetrics.lo $(ALLSTATIC) $(LIBS)

lock1$(EXE): lock1.cpp lock1.lo
	$(LTLINK) $(CXX) -o $@ lock1.lo $(ALLSTATIC) $(LIBS)

lock2$(EXE): lock2.cpp lock2.lo
	$(LTLINK) $(CXX) -o $@ lock2.lo $(ALLSTATIC) $(LIBS)

memorypool$(EXE): memorypool.cpp memorypool.lo
	$(LTLINK) $(CXX) -o $@ memorypool.lo $(ALLSTATIC) $(LIBS)

oneshotclnt$(EXE): oneshotclnt.cpp oneshotclnt.lo
	$(LTLINK) $(CXX) -o $@ oneshotclnt.lo $(ALLSTATIC) $(LIBS)

oneshotsvr$(EXE): oneshotsvr.cpp oneshotsvr.lo
	$(LTLINK) $(CXX) -o $@ oneshotsvr.lo $(ALLSTATIC) $(LIBS)

userentry$(EXE): userentry.cpp userentry.lo
	$(LTLINK) $(CXX) -o $@ userentry.lo $(ALLSTATIC) $(LIBS)

serviceentry$(EXE): serviceentry.cpp serviceentry.lo
	$(LTLINK) $(CXX) -o $@ serviceentry.lo $(ALLSTATIC) $(LIBS)

permissions$(EXE): permissions.cpp permissions.lo
	$(LTLINK) $(CXX) -o $@ permissions.lo $(ALLSTATIC) $(LIBS)

protocolentry$(EXE): protocolentry.cpp protocolentry.lo
	$(LTLINK) $(CXX) -o $@ protocolentry.lo $(ALLSTATIC) $(LIBS)

regularexpression$(EXE): regularexpression.cpp regularexpression.lo
	$(LTLINK) $(CXX) -o $@ regularexpression.lo $(ALLSTATIC) $(LIBS)

rpcentry$(EXE): rpcentry.cpp rpcentry.lo
	$(LTLINK) $(CXX) -o $@ rpcentry.lo $(ALLSTATIC) $(LIBS)

semaphoreset1$(EXE): semaphoreset1.cpp semaphoreset1.lo
	$(LTLINK) $(CXX) -o $@ semaphoreset1.lo $(ALLSTATIC) $(LIBS)

semaphoreset2$(EXE): semaphoreset2.cpp semaphoreset2.lo
	$(LTLINK) $(CXX) -o $@ semaphoreset2.lo $(ALLSTATIC) $(LIBS)

sharedmemory1$(EXE): sharedmemory1.cpp sharedmemory1.lo
	$(LTLINK) $(CXX) -o $@ sharedmemory1.lo $(ALLSTATIC) $(LIBS)

sharedmemory2$(EXE): sharedmemory2.cpp sharedmemory2.lo
	$(LTLINK) $(CXX) -o $@ sharedmemory2.lo $(ALLSTATIC) $(LIBS)

charstring$(EXE): charstring.cpp charstring.lo
	$(LTLINK) $(CXX) -o $@ charstring.lo $(ALLSTATIC) $(LIBS)

datetime$(EXE): datetime.cpp datetime.lo
	$(LTLINK) $(CXX) -o $@ datetime.lo $(ALLSTATIC) $(LIBS)

dictionary$(EXE): dictionary.cpp dictionary.lo
	$(LTLINK) $(CXX) -o $@ dictionary.lo $(ALLSTATIC) $(LIBS)

dtd$(EXE): dtd.cpp dtd.lo
	$(LTLINK) $(CXX) -o $@ dtd.lo $(ALLSTATIC) $(LIBS)

randomnumber$(EXE): randomnumber.cpp randomnumber.lo
	$(LTLINK) $(CXX) -o $@ randomnumber.lo $(ALLSTATIC) $(LIBS)

signal$(EXE): signal.cpp signal.lo
	$(LTLINK) $(CXX) -o $@ signal.lo $(ALLSTATIC) $(LIBS)

kill$(EXE): kill.cpp kill.lo
	$(LTLINK) $(CXX) -o $@ kill.lo $(ALLSTATIC) $(LIBS)

stringbuffer$(EXE): stringbuffer.cpp stringbuffer.lo 
	$(LTLINK) $(CXX) -o $@ stringbuffer.lo $(ALLSTATIC) $(LIBS)

handoff1$(EXE): handoff1.cpp handoff1.lo
	$(LTLINK) $(CXX) -o $@ handoff1.lo $(ALLSTATIC) $(LIBS)

handoff2$(EXE): handoff2.cpp handoff2.lo
	$(LTLINK) $(CXX) -o $@ handoff2.lo $(ALLSTATIC) $(LIBS)

handoffclient$(EXE): handoffclient.cpp handoffclient.lo
	$(LTLINK) $(CXX) -o $@ handoffclient.lo $(ALLSTATIC) $(LIBS)

nonhandoff1$(EXE): nonhandoff1.cpp nonhandoff1.lo
	$(LTLINK) $(CXX) -o $@ nonhandoff1.lo $(ALLSTATIC) $(LIBS)

nonhandoff2$(EXE): nonhandoff2.cpp nonhandoff2.lo
	$(LTLINK) $(CXX) -o $@ nonhandoff2.lo $(ALLSTATIC) $(LIBS)

nonhandoffclient$(EXE): nonhandoffclient.cpp nonhandoffclient.lo
	$(LTLINK) $(CXX) -o $@ nonhandoffclient.lo $(ALLSTATIC) $(LIBS)

bytebuffer$(EXE): bytebuffer.cpp bytebuffer.lo
	$(LTLINK) $(CXX) -o $@ bytebuffer.lo $(ALLSTATIC) $(LIBS)

xmls$(EXE): xmls.cpp xmls.lo
	$(LTLINK) $(CXX) -o $@ xmls.lo $(ALLSTATIC) $(LIBS)

xmld$(EXE): xmld.cpp xmld.lo
	$(LTLINK) $(CXX) -o $@ xmld.lo $(ALLSTATIC) $(LIBS)

xmlentities$(EXE): xmlentities.cpp xmlentities.lo
	$(LTLINK) $(CXX) -o $@ xmlentities.lo $(ALLSTATIC) $(LIBS)

intervaltimer$(EXE): intervaltimer.cpp intervaltimer.lo
	$(LTLINK) $(CXX) -o $@ intervaltimer.lo $(ALLSTATIC) $(LIBS)

inetclntbench$(EXE): inetclntbench.cpp inetclntbench.lo 
	$(LTLINK) $(CXX) -o $@ inetclntbench.lo $(ALLSTATIC) $(LIBS)

inetsvrbench$(EXE): inetsvrbench.cpp inetsvrbench.lo 
	$(LTLINK) $(CXX) -o $@ inetsvrbench.lo $(ALLSTATIC) $(LIBS)

device$(EXE): device.cpp device.lo 
	$(LTLINK) $(CXX) -o $@ device.lo $(ALLSTATIC) $(LIBS)

serialport$(EXE): serialport.cpp serialport.lo 
	$(LTLINK) $(CXX) -o $@ serialport.lo $(ALLSTATIC) $(LIBS)

shmfile$(EXE): shmfile.cpp shmfile.lo 
	$(LTLINK) $(CXX) -o $@ shmfile.lo $(ALLSTATIC) $(LIBS)

chat$(EXE): chat.cpp chat.lo 
	$(LTLINK) $(CXX) -o $@ chat.lo $(ALLSTATIC) $(LIBS)

crypt$(EXE): crypt.cpp crypt.lo 
	$(LTLINK) $(CXX) -o $@ crypt.lo $(ALLSTATIC) $(LIBS)

dynamicarray$(EXE): dynamicarray.cpp dynamicarray.lo 
	$(LTLINK) $(CXX) -o $@ dynamicarray.lo $(ALLSTATIC) $(LIBS)

staticarray$(EXE): staticarray.cpp staticarray.lo 
	$(LTLINK) $(CXX) -o $@ staticarray.lo $(ALLSTATIC) $(LIBS)

dynamiclib$(EXE): dynamiclib.cpp dynamiclib.lo 
	$(LTLINK) $(CXX) -o $@ dynamiclib.lo $(ALLSTATIC) $(LIBS)

error$(EXE): error.cpp error.lo 
	$(LTLINK) $(CXX) -o $@ error.lo $(ALLSTATIC) $(LIBS)

math$(EXE): math.cpp math.lo 
	$(LTLINK) $(CXX) -o $@ math.lo $(ALLSTATIC) $(LIBS)

thread$(EXE): thread.cpp thread.lo 
	$(LTLINK) $(CXX) -o $@ thread.lo $(ALLSTATIC) $(LIBS)

threadmutex$(EXE): threadmutex.cpp threadmutex.lo 
	$(LTLINK) $(CXX) -o $@ threadmutex.lo $(ALLSTATIC) $(LIBS)

process$(EXE): process.cpp process.lo 
	$(LTLINK) $(CXX) -o $@ process.lo $(ALLSTATIC) $(LIBS)

serialportprofile$(EXE): serialportprofile.cpp serialportprofile.lo 
	$(LTLINK) $(CXX) -o $@ serialportprofile.lo $(ALLSTATIC) $(LIBS)

snooze$(EXE): snooze.cpp snooze.lo 
	$(LTLINK) $(CXX) -o $@ snooze.lo $(ALLSTATIC) $(LIBS)

memorymap$(EXE): memorymap.cpp memorymap.lo 
	$(LTLINK) $(CXX) -o $@ memorymap.lo $(ALLSTATIC) $(LIBS)

character$(EXE): character.cpp character.lo 
	$(LTLINK) $(CXX) -o $@ character.lo $(ALLSTATIC) $(LIBS)

bytestring$(EXE): bytestring.cpp bytestring.lo 
	$(LTLINK) $(CXX) -o $@ bytestring.lo $(ALLSTATIC) $(LIBS)

sys$(EXE): sys.cpp sys.lo
	$(LTLINK) $(CXX) -o $@ sys.lo $(ALLSTATIC) $(LIBS)

reboot$(EXE): reboot.cpp reboot.lo
	$(LTLINK) $(CXX) -o $@ reboot.lo $(ALLSTATIC) $(LIBS)

halt$(EXE): halt.cpp halt.lo
	$(LTLINK) $(CXX) -o $@ halt.lo $(ALLSTATIC) $(LIBS)

shutdown$(EXE): shutdown.cpp shutdown.lo
	$(LTLINK) $(CXX) -o $@ shutdown.lo $(ALLSTATIC) $(LIBS)

server.pem:
	cd certificates; $(MAKE) clean all
	cp certificates/certificates/ca.pem .
	cp certificates/certificatechains/server.pem .
	cp certificates/certificatechains/client.pem .
