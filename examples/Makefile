top_builddir = ..

include ../config.mk

CPPFLAGS = $(BASECPPFLAGS) $(INC)../ $(INC)../include
LIBS=-L../src -lrudiments $(SOCKETLIBS) $(PTHREADLIB) $(SSLLIBS) $(PCRELIBS) $(NANOSLEEPLIB) $(CLOCKNANOSLEEPLIB) $(SHMOPENLIB) $(CRYPTLIB) $(INETATONLIB) $(BELIB) $(GNULIB) $(CRTLIB) $(DLLIB)

PROGS0 = \
	character-isxxx$(EXE) \
	character-toxxx$(EXE) \
	character-inset$(EXE) \
	charstring-copy$(EXE) \
	charstring-compare$(EXE) \
	charstring-find$(EXE) \
	charstring-transform$(EXE) \
	charstring-split$(EXE) \
	charstring-numbers$(EXE) \
	charstring-encode$(EXE) \
	chat$(EXE) \
	commandline$(EXE) \
	complexinetsocketserver$(EXE) \
	crypt$(EXE) \
	datetime$(EXE) \
	device$(EXE) \
	dictionary$(EXE) \
	directory$(EXE) \
	dynamiclib$(EXE) \
	environment$(EXE) \
	error$(EXE) \
	file$(EXE) \
	filesystem$(EXE) \
	groupentry$(EXE) \
	hostentry$(EXE) \
	inetsocketclient$(EXE) \
	inetsocketserver$(EXE) \

PROGS1 = \
	linkedlist$(EXE) \
	listener$(EXE) \
	logger$(EXE) \
	memorymap$(EXE) \
	memorypool$(EXE) \
	modemclient$(EXE) \
	modemserver$(EXE) \
	thread$(EXE) \
	threadmutex$(EXE) \
	parameterstring$(EXE) \
	userentry$(EXE) \
	permissions$(EXE) \
	process-info$(EXE) \
	process-exit$(EXE) \
	process-atexit$(EXE) \
	process-exitimmediately$(EXE) \
	process-exitoncrash$(EXE) \
	process-handlecrash$(EXE) \
	process-exitonshutdown$(EXE) \
	process-handleshutdown$(EXE) \
	process-setuidgid$(EXE) \
	process-seteuidgid$(EXE) \
	process-spawn$(EXE) \
	process-fork$(EXE) \
	process-forkexec$(EXE) \
	process-zombie$(EXE) \
	process-detach$(EXE) \
	protocolentry$(EXE) \
	randomnumber$(EXE) \
	bytestring-copy$(EXE) \
	bytestring-compare$(EXE) \
	bytestring-find$(EXE) \
	regularexpression$(EXE) \
	rpcentry$(EXE) \
	serialport$(EXE) \
	serialportprofile$(EXE) \

PROGS2 = \
	serviceentry$(EXE) \
	sharedmemory1$(EXE) \
	sharedmemory2$(EXE) \
	shmfile$(EXE) \
	signal$(EXE) \
	snooze$(EXE) \
	stdio$(EXE) \
	stringbuffer$(EXE) \
	sys$(EXE) \
	unixsocketclient$(EXE) \
	unixsocketserver$(EXE) \
	bytebuffer$(EXE) \
	xmldom$(EXE) \
	xmlsax$(EXE) \
	codetree$(EXE)

PROGS = $(PROGS0) $(PROGS1) $(PROGS2)

.SUFFIXES: .$(OBJ) .obj

.cpp.lo:
	$(LTCOMPILE) $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $< $(OUT)$@

.cpp.obj:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $<

all: $(PROGS)

clean:
	$(LTCLEAN) $(RM) $(PROGS0)
	$(LTCLEAN) $(RM) $(PROGS1)
	$(LTCLEAN) $(RM) $(PROGS2)
	$(LTCLEAN) $(RM) *.lo *.o *.obj

character-isxxx$(EXE): character-isxxx.cpp character-isxxx.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ character-isxxx.$(OBJ) $(LIBS)

character-toxxx$(EXE): character-toxxx.cpp character-toxxx.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ character-toxxx.$(OBJ) $(LIBS)

character-inset$(EXE): character-inset.cpp character-inset.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ character-inset.$(OBJ) $(LIBS)

charstring-copy$(EXE): charstring-copy.cpp charstring-copy.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ charstring-copy.$(OBJ) $(LIBS)

charstring-compare$(EXE): charstring-compare.cpp charstring-compare.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ charstring-compare.$(OBJ) $(LIBS)

charstring-find$(EXE): charstring-find.cpp charstring-find.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ charstring-find.$(OBJ) $(LIBS)

charstring-transform$(EXE): charstring-transform.cpp charstring-transform.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ charstring-transform.$(OBJ) $(LIBS)

charstring-split$(EXE): charstring-split.cpp charstring-split.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ charstring-split.$(OBJ) $(LIBS)

charstring-numbers$(EXE): charstring-numbers.cpp charstring-numbers.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ charstring-numbers.$(OBJ) $(LIBS)

charstring-encode$(EXE): charstring-encode.cpp charstring-encode.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ charstring-encode.$(OBJ) $(LIBS)

chat$(EXE): chat.cpp chat.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ chat.$(OBJ) $(LIBS)

clientserverfactory$(EXE): clientserverfactory.cpp clientserverfactory.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ clientserverfactory.$(OBJ) $(LIBS)

commandline$(EXE): commandline.cpp commandline.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ commandline.$(OBJ) $(LIBS)

complexinetsocketserver$(EXE): complexinetsocketserver.cpp complexinetsocketserver.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ complexinetsocketserver.$(OBJ) $(LIBS)

crypt$(EXE): crypt.cpp crypt.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ crypt.$(OBJ) $(LIBS)

datetime$(EXE): datetime.cpp datetime.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ datetime.$(OBJ) $(LIBS)

device$(EXE): device.cpp device.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ device.$(OBJ) $(LIBS)

dictionary$(EXE): dictionary.cpp dictionary.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ dictionary.$(OBJ) $(LIBS)

dynamiclib$(EXE): dynamiclib.cpp dynamiclib.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ dynamiclib.$(OBJ) $(LIBS)

environment$(EXE): environment.cpp environment.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ environment.$(OBJ) $(LIBS)

error$(EXE): error.cpp error.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ error.$(OBJ) $(LIBS)

file$(EXE): file.cpp file.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ file.$(OBJ) $(LIBS)

filesystem$(EXE): filesystem.cpp filesystem.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ filesystem.$(OBJ) $(LIBS)

groupentry$(EXE): groupentry.cpp groupentry.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ groupentry.$(OBJ) $(LIBS)

hostentry$(EXE): hostentry.cpp hostentry.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ hostentry.$(OBJ) $(LIBS)

inetsocketclient$(EXE): inetsocketclient.cpp inetsocketclient.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ inetsocketclient.$(OBJ) $(LIBS)

inetsocketserver$(EXE): inetsocketserver.cpp inetsocketserver.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ inetsocketserver.$(OBJ) $(LIBS)

linkedlist$(EXE): linkedlist.cpp linkedlist.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ linkedlist.$(OBJ) $(LIBS)

listener$(EXE): listener.cpp listener.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ listener.$(OBJ) $(LIBS)

logger$(EXE): logger.cpp logger.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ logger.$(OBJ) $(LIBS)

memorymap$(EXE): memorymap.cpp memorymap.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ memorymap.$(OBJ) $(LIBS)

memorypool$(EXE): memorypool.cpp memorypool.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ memorypool.$(OBJ) $(LIBS)

modemclient$(EXE): modemclient.cpp modemclient.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ modemclient.$(OBJ) $(LIBS)

modemserver$(EXE): modemserver.cpp modemserver.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ modemserver.$(OBJ) $(LIBS)

thread$(EXE): thread.cpp thread.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ thread.$(OBJ) $(LIBS)

threadmutex$(EXE): threadmutex.cpp threadmutex.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ threadmutex.$(OBJ) $(LIBS)

parameterstring$(EXE): parameterstring.cpp parameterstring.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ parameterstring.$(OBJ) $(LIBS)

userentry$(EXE): userentry.cpp userentry.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ userentry.$(OBJ) $(LIBS)

permissions$(EXE): permissions.cpp permissions.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ permissions.$(OBJ) $(LIBS)

process-info$(EXE): process-info.cpp process-info.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ process-info.$(OBJ) $(LIBS)

process-exit$(EXE): process-exit.cpp process-exit.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ process-exit.$(OBJ) $(LIBS)

process-atexit$(EXE): process-atexit.cpp process-atexit.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ process-atexit.$(OBJ) $(LIBS)

process-exitimmediately$(EXE): process-exitimmediately.cpp process-exitimmediately.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ process-exitimmediately.$(OBJ) $(LIBS)

process-exitoncrash$(EXE): process-exitoncrash.cpp process-exitoncrash.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ process-exitoncrash.$(OBJ) $(LIBS)

process-handlecrash$(EXE): process-handlecrash.cpp process-handlecrash.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ process-handlecrash.$(OBJ) $(LIBS)

process-exitonshutdown$(EXE): process-exitonshutdown.cpp process-exitonshutdown.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ process-exitonshutdown.$(OBJ) $(LIBS)

process-handleshutdown$(EXE): process-handleshutdown.cpp process-handleshutdown.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ process-handleshutdown.$(OBJ) $(LIBS)

process-setuidgid$(EXE): process-setuidgid.cpp process-setuidgid.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ process-setuidgid.$(OBJ) $(LIBS)

process-seteuidgid$(EXE): process-seteuidgid.cpp process-seteuidgid.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ process-seteuidgid.$(OBJ) $(LIBS)

process-spawn$(EXE): process-spawn.cpp process-spawn.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ process-spawn.$(OBJ) $(LIBS)

process-fork$(EXE): process-fork.cpp process-fork.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ process-fork.$(OBJ) $(LIBS)

process-forkexec$(EXE): process-forkexec.cpp process-forkexec.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ process-forkexec.$(OBJ) $(LIBS)

process-zombie$(EXE): process-zombie.cpp process-zombie.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ process-zombie.$(OBJ) $(LIBS)

process-detach$(EXE): process-detach.cpp process-detach.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ process-detach.$(OBJ) $(LIBS)

protocolentry$(EXE): protocolentry.cpp protocolentry.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ protocolentry.$(OBJ) $(LIBS)

randomnumber$(EXE): randomnumber.cpp randomnumber.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ randomnumber.$(OBJ) $(LIBS)

bytestring-copy$(EXE): bytestring-copy.cpp bytestring-copy.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ bytestring-copy.$(OBJ) $(LIBS)

bytestring-compare$(EXE): bytestring-compare.cpp bytestring-compare.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ bytestring-compare.$(OBJ) $(LIBS)

bytestring-find$(EXE): bytestring-find.cpp bytestring-find.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ bytestring-find.$(OBJ) $(LIBS)

regularexpression$(EXE): regularexpression.cpp regularexpression.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ regularexpression.$(OBJ) $(LIBS)

rpcentry$(EXE): rpcentry.cpp rpcentry.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ rpcentry.$(OBJ) $(LIBS)

serialport$(EXE): serialport.cpp serialport.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ serialport.$(OBJ) $(LIBS)

serialportprofile$(EXE): serialportprofile.cpp serialportprofile.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ serialportprofile.$(OBJ) $(LIBS)

serviceentry$(EXE): serviceentry.cpp serviceentry.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ serviceentry.$(OBJ) $(LIBS)

sharedmemory1$(EXE): sharedmemory1.cpp sharedmemory1.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ sharedmemory1.$(OBJ) $(LIBS)

sharedmemory2$(EXE): sharedmemory2.cpp sharedmemory2.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ sharedmemory2.$(OBJ) $(LIBS)

shmfile$(EXE): shmfile.cpp shmfile.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ shmfile.$(OBJ) $(LIBS)

signal$(EXE): signal.cpp signal.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ signal.$(OBJ) $(LIBS)

snooze$(EXE): snooze.cpp snooze.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ snooze.$(OBJ) $(LIBS)

stdio$(EXE): stdio.cpp stdio.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ stdio.$(OBJ) $(LIBS)

stringbuffer$(EXE): stringbuffer.cpp stringbuffer.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ stringbuffer.$(OBJ) $(LIBS)

sys$(EXE): sys.cpp sys.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ sys.$(OBJ) $(LIBS)

unixsocketclient$(EXE): unixsocketclient.cpp unixsocketclient.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ unixsocketclient.$(OBJ) $(LIBS)

unixsocketserver$(EXE): unixsocketserver.cpp unixsocketserver.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ unixsocketserver.$(OBJ) $(LIBS)

bytebuffer$(EXE): bytebuffer.cpp bytebuffer.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ bytebuffer.$(OBJ) $(LIBS)

xmldom$(EXE): xmldom.cpp xmldom.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ xmldom.$(OBJ) $(LIBS)

xmlsax$(EXE): xmlsax.cpp xmlsax.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ xmlsax.$(OBJ) $(LIBS)

codetree$(EXE): codetree.cpp codetree.$(OBJ)
	$(LTLINK) $(CXX) $(OUT)$@ codetree.$(OBJ) $(LIBS)
