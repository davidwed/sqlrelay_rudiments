top_builddir = ..

include ../config.mk

CPPFLAGS = $(BASECPPFLAGS) $(INC)../ $(INC)../include
LIBS=$(RUDIMENTSLIB) $(SOCKETLIBS) $(PTHREADLIB) $(SSLLIBS) $(PCRELIBS) $(NANOSLEEPLIB) $(CLOCKNANOSLEEPLIB) $(SHMOPENLIB) $(CRYPTLIB) $(INETATONLIB) $(BELIB) $(GNULIB) $(CRTLIB) $(DLLIB) $(DEBUGLIB)

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

.SUFFIXES: .lo .obj

.cpp.lo:
	$(LTCOMPILE) $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $< $(OUT)$@

.cpp.obj:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $<

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

server$(EXE): server.cpp server.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ server.$(OBJ) $(ALLSTATIC) $(LIBS)

nullserver$(EXE): nullserver.cpp nullserver.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ nullserver.$(OBJ) $(ALLSTATIC) $(LIBS)

sslsvr$(EXE): sslsvr.cpp sslsvr.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ sslsvr.$(OBJ) $(ALLSTATIC) $(LIBS)

listener$(EXE): listener.cpp listener.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ listener.$(OBJ) $(ALLSTATIC) $(LIBS)

inetsocketclient$(EXE): inetsocketclient.cpp inetsocketclient.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ inetsocketclient.$(OBJ) $(ALLSTATIC) $(LIBS)

sslclnt$(EXE): sslclnt.cpp sslclnt.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ sslclnt.$(OBJ) $(ALLSTATIC) $(LIBS)

unixsocketclient$(EXE): unixsocketclient.cpp unixsocketclient.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ unixsocketclient.$(OBJ) $(ALLSTATIC) $(LIBS)

directory$(EXE): directory.cpp directory.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ directory.$(OBJ) $(ALLSTATIC) $(LIBS)

daemon$(EXE): daemon.cpp daemon.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ daemon.$(OBJ) $(ALLSTATIC) $(LIBS)

commandline$(EXE): commandline.cpp commandline.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ commandline.$(OBJ) $(ALLSTATIC) $(LIBS)

parameterstring$(EXE): parameterstring.cpp parameterstring.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ parameterstring.$(OBJ) $(ALLSTATIC) $(LIBS)

logger$(EXE): logger.cpp logger.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ logger.$(OBJ) $(ALLSTATIC) $(LIBS)

environment$(EXE): environment.cpp environment.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ environment.$(OBJ) $(ALLSTATIC) $(LIBS)

file$(EXE): file.cpp file.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ file.$(OBJ) $(ALLSTATIC) $(LIBS)

filesystem$(EXE): filesystem.cpp filesystem.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ filesystem.$(OBJ) $(ALLSTATIC) $(LIBS)

groupentry$(EXE): groupentry.cpp groupentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ groupentry.$(OBJ) $(ALLSTATIC) $(LIBS)

hostentry$(EXE): hostentry.cpp hostentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ hostentry.$(OBJ) $(ALLSTATIC) $(LIBS)

linkedlist$(EXE): linkedlist.cpp linkedlist.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ linkedlist.$(OBJ) $(ALLSTATIC) $(LIBS)

singlylinkedlist$(EXE): singlylinkedlist.cpp singlylinkedlist.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ singlylinkedlist.$(OBJ) $(ALLSTATIC) $(LIBS)

listmetrics$(EXE): listmetrics.cpp listmetrics.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ listmetrics.$(OBJ) $(ALLSTATIC) $(LIBS)

lock1$(EXE): lock1.cpp lock1.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ lock1.$(OBJ) $(ALLSTATIC) $(LIBS)

lock2$(EXE): lock2.cpp lock2.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ lock2.$(OBJ) $(ALLSTATIC) $(LIBS)

memorypool$(EXE): memorypool.cpp memorypool.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ memorypool.$(OBJ) $(ALLSTATIC) $(LIBS)

oneshotclnt$(EXE): oneshotclnt.cpp oneshotclnt.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ oneshotclnt.$(OBJ) $(ALLSTATIC) $(LIBS)

oneshotsvr$(EXE): oneshotsvr.cpp oneshotsvr.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ oneshotsvr.$(OBJ) $(ALLSTATIC) $(LIBS)

userentry$(EXE): userentry.cpp userentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ userentry.$(OBJ) $(ALLSTATIC) $(LIBS)

serviceentry$(EXE): serviceentry.cpp serviceentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ serviceentry.$(OBJ) $(ALLSTATIC) $(LIBS)

permissions$(EXE): permissions.cpp permissions.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ permissions.$(OBJ) $(ALLSTATIC) $(LIBS)

protocolentry$(EXE): protocolentry.cpp protocolentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ protocolentry.$(OBJ) $(ALLSTATIC) $(LIBS)

regularexpression$(EXE): regularexpression.cpp regularexpression.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ regularexpression.$(OBJ) $(ALLSTATIC) $(LIBS)

rpcentry$(EXE): rpcentry.cpp rpcentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ rpcentry.$(OBJ) $(ALLSTATIC) $(LIBS)

semaphoreset1$(EXE): semaphoreset1.cpp semaphoreset1.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ semaphoreset1.$(OBJ) $(ALLSTATIC) $(LIBS)

semaphoreset2$(EXE): semaphoreset2.cpp semaphoreset2.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ semaphoreset2.$(OBJ) $(ALLSTATIC) $(LIBS)

sharedmemory1$(EXE): sharedmemory1.cpp sharedmemory1.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ sharedmemory1.$(OBJ) $(ALLSTATIC) $(LIBS)

sharedmemory2$(EXE): sharedmemory2.cpp sharedmemory2.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ sharedmemory2.$(OBJ) $(ALLSTATIC) $(LIBS)

charstring$(EXE): charstring.cpp charstring.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ charstring.$(OBJ) $(ALLSTATIC) $(LIBS)

datetime$(EXE): datetime.cpp datetime.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ datetime.$(OBJ) $(ALLSTATIC) $(LIBS)

dictionary$(EXE): dictionary.cpp dictionary.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ dictionary.$(OBJ) $(ALLSTATIC) $(LIBS)

dtd$(EXE): dtd.cpp dtd.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ dtd.$(OBJ) $(ALLSTATIC) $(LIBS)

randomnumber$(EXE): randomnumber.cpp randomnumber.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ randomnumber.$(OBJ) $(ALLSTATIC) $(LIBS)

signal$(EXE): signal.cpp signal.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ signal.$(OBJ) $(ALLSTATIC) $(LIBS)

kill$(EXE): kill.cpp kill.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ kill.$(OBJ) $(ALLSTATIC) $(LIBS)

stringbuffer$(EXE): stringbuffer.cpp stringbuffer.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ stringbuffer.$(OBJ) $(ALLSTATIC) $(LIBS)

handoff1$(EXE): handoff1.cpp handoff1.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ handoff1.$(OBJ) $(ALLSTATIC) $(LIBS)

handoff2$(EXE): handoff2.cpp handoff2.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ handoff2.$(OBJ) $(ALLSTATIC) $(LIBS)

handoffclient$(EXE): handoffclient.cpp handoffclient.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ handoffclient.$(OBJ) $(ALLSTATIC) $(LIBS)

nonhandoff1$(EXE): nonhandoff1.cpp nonhandoff1.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ nonhandoff1.$(OBJ) $(ALLSTATIC) $(LIBS)

nonhandoff2$(EXE): nonhandoff2.cpp nonhandoff2.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ nonhandoff2.$(OBJ) $(ALLSTATIC) $(LIBS)

nonhandoffclient$(EXE): nonhandoffclient.cpp nonhandoffclient.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ nonhandoffclient.$(OBJ) $(ALLSTATIC) $(LIBS)

bytebuffer$(EXE): bytebuffer.cpp bytebuffer.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ bytebuffer.$(OBJ) $(ALLSTATIC) $(LIBS)

xmls$(EXE): xmls.cpp xmls.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ xmls.$(OBJ) $(ALLSTATIC) $(LIBS)

xmld$(EXE): xmld.cpp xmld.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ xmld.$(OBJ) $(ALLSTATIC) $(LIBS)

xmlentities$(EXE): xmlentities.cpp xmlentities.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ xmlentities.$(OBJ) $(ALLSTATIC) $(LIBS)

intervaltimer$(EXE): intervaltimer.cpp intervaltimer.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ intervaltimer.$(OBJ) $(ALLSTATIC) $(LIBS)

inetclntbench$(EXE): inetclntbench.cpp inetclntbench.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ inetclntbench.$(OBJ) $(ALLSTATIC) $(LIBS)

inetsvrbench$(EXE): inetsvrbench.cpp inetsvrbench.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ inetsvrbench.$(OBJ) $(ALLSTATIC) $(LIBS)

device$(EXE): device.cpp device.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ device.$(OBJ) $(ALLSTATIC) $(LIBS)

serialport$(EXE): serialport.cpp serialport.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ serialport.$(OBJ) $(ALLSTATIC) $(LIBS)

shmfile$(EXE): shmfile.cpp shmfile.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ shmfile.$(OBJ) $(ALLSTATIC) $(LIBS)

chat$(EXE): chat.cpp chat.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ chat.$(OBJ) $(ALLSTATIC) $(LIBS)

crypt$(EXE): crypt.cpp crypt.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ crypt.$(OBJ) $(ALLSTATIC) $(LIBS)

dynamicarray$(EXE): dynamicarray.cpp dynamicarray.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ dynamicarray.$(OBJ) $(ALLSTATIC) $(LIBS)

staticarray$(EXE): staticarray.cpp staticarray.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ staticarray.$(OBJ) $(ALLSTATIC) $(LIBS)

dynamiclib$(EXE): dynamiclib.cpp dynamiclib.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ dynamiclib.$(OBJ) $(ALLSTATIC) $(LIBS)

error$(EXE): error.cpp error.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ error.$(OBJ) $(ALLSTATIC) $(LIBS)

math$(EXE): math.cpp math.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ math.$(OBJ) $(ALLSTATIC) $(LIBS)

thread$(EXE): thread.cpp thread.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ thread.$(OBJ) $(ALLSTATIC) $(LIBS)

threadmutex$(EXE): threadmutex.cpp threadmutex.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ threadmutex.$(OBJ) $(ALLSTATIC) $(LIBS)

process$(EXE): process.cpp process.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ process.$(OBJ) $(ALLSTATIC) $(LIBS)

serialportprofile$(EXE): serialportprofile.cpp serialportprofile.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ serialportprofile.$(OBJ) $(ALLSTATIC) $(LIBS)

snooze$(EXE): snooze.cpp snooze.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ snooze.$(OBJ) $(ALLSTATIC) $(LIBS)

memorymap$(EXE): memorymap.cpp memorymap.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ memorymap.$(OBJ) $(ALLSTATIC) $(LIBS)

character$(EXE): character.cpp character.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ character.$(OBJ) $(ALLSTATIC) $(LIBS)

bytestring$(EXE): bytestring.cpp bytestring.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@ bytestring.$(OBJ) $(ALLSTATIC) $(LIBS)

sys$(EXE): sys.cpp sys.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ sys.$(OBJ) $(ALLSTATIC) $(LIBS)

reboot$(EXE): reboot.cpp reboot.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ reboot.$(OBJ) $(ALLSTATIC) $(LIBS)

halt$(EXE): halt.cpp halt.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ halt.$(OBJ) $(ALLSTATIC) $(LIBS)

shutdown$(EXE): shutdown.cpp shutdown.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ shutdown.$(OBJ) $(ALLSTATIC) $(LIBS)

server.pem:
	cd certificates $(AND) $(MAKE) clean all
	cp certificates/certificates/ca.pem .
	cp certificates/certificatechains/server.pem .
	cp certificates/certificatechains/client.pem .
