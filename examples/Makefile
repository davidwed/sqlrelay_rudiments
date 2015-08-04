top_builddir = ..

include ../config.mk

CPPFLAGS = $(BASECPPFLAGS) $(INC)../ $(INC)../include
LIBS=$(RUDIMENTSLIB) $(SOCKETLIBS) $(PTHREADLIB) $(SSLLIBS) $(PCRELIBS) $(NANOSLEEPLIB) $(CLOCKNANOSLEEPLIB) $(CRYPTLIB) $(INETATONLIB) $(BELIB) $(GNULIB) $(CRTLIB) $(DLLIB)

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
	datetime-basic$(EXE) \
	datetime-init$(EXE) \
	datetime-math$(EXE) \
	datetime-tz$(EXE) \
	device$(EXE) \
	dictionary-const$(EXE) \
	dictionary-nonconst$(EXE) \
	directory$(EXE) \
	dtd$(EXE) \
	dynamiclib$(EXE) \
	environment$(EXE) \
	error$(EXE) \
	file$(EXE) \
	filesystem$(EXE) \
	groupentry$(EXE) \
	hostentry-hostname$(EXE) \
	hostentry-address$(EXE) \
	inetsocketclient$(EXE) \
	inetsocketserver$(EXE) \

PROGS1 = \
	linkedlist-values$(EXE) \
	linkedlist-nodes$(EXE) \
	linkedlist-sort$(EXE) \
	linkedlist-iterate$(EXE) \
	linkedlist-types$(EXE) \
	listener$(EXE) \
	logger$(EXE) \
	memorymap$(EXE) \
	memorypool$(EXE) \
	modemclient$(EXE) \
	modemserver$(EXE) \
	thread-join$(EXE) \
	thread-detach$(EXE) \
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
	protocolentry-name$(EXE) \
	protocolentry-number$(EXE) \
	randomnumber-static$(EXE) \
	randomnumber-multiple$(EXE) \
	bytestring-copy$(EXE) \
	bytestring-compare$(EXE) \
	bytestring-find$(EXE) \
	regularexpression-static$(EXE) \
	regularexpression-compile$(EXE) \
	regularexpression-multiple$(EXE) \
	serialport$(EXE) \
	serialportprofile$(EXE) \

PROGS2 = \
	serviceentry-name$(EXE) \
	serviceentry-number$(EXE) \
	sharedmemory1$(EXE) \
	sharedmemory2$(EXE) \
	semaphoreset1$(EXE) \
	semaphoreset2$(EXE) \
	signal$(EXE) \
	kill$(EXE) \
	singlylinkedlist-values$(EXE) \
	singlylinkedlist-nodes$(EXE) \
	singlylinkedlist-sort$(EXE) \
	singlylinkedlist-iterate$(EXE) \
	singlylinkedlist-types$(EXE) \
	snooze$(EXE) \
	stdio$(EXE) \
	stringbuffer$(EXE) \
	sys$(EXE) \
	reboot$(EXE) \
	shutdown$(EXE) \
	halt$(EXE) \
	unixsocketclient$(EXE) \
	unixsocketserver$(EXE) \
	bytebuffer$(EXE) \
	xmldom$(EXE) \
	xmlsax$(EXE) \
	codetree$(EXE) \
	compiler$(EXE)

PROGS = $(PROGS0) $(PROGS1) $(PROGS2)

.SUFFIXES: .lo .obj

.cpp.lo:
	$(LTCOMPILE) $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $< $(OUT)$@

.cpp.obj:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $<

all: $(PROGS)

clean:
	$(LTCLEAN) $(RM) $(PROGS0)
	$(LTCLEAN) $(RM) $(PROGS1)
	$(LTCLEAN) $(RM) $(PROGS2)
	$(LTCLEAN) $(RM) *.lo *.o *.obj shm sem

character-isxxx$(EXE): character-isxxx.cpp character-isxxx.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ character-isxxx.$(OBJ) $(LIBS)

character-toxxx$(EXE): character-toxxx.cpp character-toxxx.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ character-toxxx.$(OBJ) $(LIBS)

character-inset$(EXE): character-inset.cpp character-inset.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ character-inset.$(OBJ) $(LIBS)

charstring-copy$(EXE): charstring-copy.cpp charstring-copy.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ charstring-copy.$(OBJ) $(LIBS)

charstring-compare$(EXE): charstring-compare.cpp charstring-compare.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ charstring-compare.$(OBJ) $(LIBS)

charstring-find$(EXE): charstring-find.cpp charstring-find.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ charstring-find.$(OBJ) $(LIBS)

charstring-transform$(EXE): charstring-transform.cpp charstring-transform.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ charstring-transform.$(OBJ) $(LIBS)

charstring-split$(EXE): charstring-split.cpp charstring-split.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ charstring-split.$(OBJ) $(LIBS)

charstring-numbers$(EXE): charstring-numbers.cpp charstring-numbers.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ charstring-numbers.$(OBJ) $(LIBS)

charstring-encode$(EXE): charstring-encode.cpp charstring-encode.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ charstring-encode.$(OBJ) $(LIBS)

chat$(EXE): chat.cpp chat.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ chat.$(OBJ) $(LIBS)

clientserverfactory$(EXE): clientserverfactory.cpp clientserverfactory.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ clientserverfactory.$(OBJ) $(LIBS)

commandline$(EXE): commandline.cpp commandline.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ commandline.$(OBJ) $(LIBS)

complexinetsocketserver$(EXE): complexinetsocketserver.cpp complexinetsocketserver.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ complexinetsocketserver.$(OBJ) $(LIBS)

crypt$(EXE): crypt.cpp crypt.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ crypt.$(OBJ) $(LIBS)

datetime-basic$(EXE): datetime-basic.cpp datetime-basic.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ datetime-basic.$(OBJ) $(LIBS)

datetime-init$(EXE): datetime-init.cpp datetime-init.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ datetime-init.$(OBJ) $(LIBS)

datetime-math$(EXE): datetime-math.cpp datetime-math.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ datetime-math.$(OBJ) $(LIBS)

datetime-tz$(EXE): datetime-tz.cpp datetime-tz.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ datetime-tz.$(OBJ) $(LIBS)

device$(EXE): device.cpp device.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ device.$(OBJ) $(LIBS)

dictionary-const$(EXE): dictionary-const.cpp dictionary-const.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ dictionary-const.$(OBJ) $(LIBS)

dictionary-nonconst$(EXE): dictionary-nonconst.cpp dictionary-nonconst.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ dictionary-nonconst.$(OBJ) $(LIBS)

directory$(EXE): directory.cpp directory.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ directory.$(OBJ) $(LIBS)

dtd$(EXE): dtd.cpp dtd.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ dtd.$(OBJ) $(LIBS)

dynamiclib$(EXE): dynamiclib.cpp dynamiclib.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ dynamiclib.$(OBJ) $(LIBS)

environment$(EXE): environment.cpp environment.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ environment.$(OBJ) $(LIBS)

error$(EXE): error.cpp error.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ error.$(OBJ) $(LIBS)

file$(EXE): file.cpp file.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ file.$(OBJ) $(LIBS)

filesystem$(EXE): filesystem.cpp filesystem.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ filesystem.$(OBJ) $(LIBS)

groupentry$(EXE): groupentry.cpp groupentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ groupentry.$(OBJ) $(LIBS)

hostentry-hostname$(EXE): hostentry-hostname.cpp hostentry-hostname.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ hostentry-hostname.$(OBJ) $(LIBS)

hostentry-address$(EXE): hostentry-address.cpp hostentry-address.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ hostentry-address.$(OBJ) $(LIBS)

inetsocketclient$(EXE): inetsocketclient.cpp inetsocketclient.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ inetsocketclient.$(OBJ) $(LIBS)

inetsocketserver$(EXE): inetsocketserver.cpp inetsocketserver.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ inetsocketserver.$(OBJ) $(LIBS)

linkedlist-values$(EXE): linkedlist-values.cpp linkedlist-values.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ linkedlist-values.$(OBJ) $(LIBS)

linkedlist-nodes$(EXE): linkedlist-nodes.cpp linkedlist-nodes.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ linkedlist-nodes.$(OBJ) $(LIBS)

linkedlist-sort$(EXE): linkedlist-sort.cpp linkedlist-sort.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ linkedlist-sort.$(OBJ) $(LIBS)

linkedlist-iterate$(EXE): linkedlist-iterate.cpp linkedlist-iterate.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ linkedlist-iterate.$(OBJ) $(LIBS)

linkedlist-types$(EXE): linkedlist-types.cpp linkedlist-types.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ linkedlist-types.$(OBJ) $(LIBS)

listener$(EXE): listener.cpp listener.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ listener.$(OBJ) $(LIBS)

logger$(EXE): logger.cpp logger.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ logger.$(OBJ) $(LIBS)

memorymap$(EXE): memorymap.cpp memorymap.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ memorymap.$(OBJ) $(LIBS)

memorypool$(EXE): memorypool.cpp memorypool.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ memorypool.$(OBJ) $(LIBS)

modemclient$(EXE): modemclient.cpp modemclient.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ modemclient.$(OBJ) $(LIBS)

modemserver$(EXE): modemserver.cpp modemserver.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ modemserver.$(OBJ) $(LIBS)

thread-join$(EXE): thread-join.cpp thread-join.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ thread-join.$(OBJ) $(LIBS)

thread-detach$(EXE): thread-detach.cpp thread-detach.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ thread-detach.$(OBJ) $(LIBS)

threadmutex$(EXE): threadmutex.cpp threadmutex.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ threadmutex.$(OBJ) $(LIBS)

parameterstring$(EXE): parameterstring.cpp parameterstring.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ parameterstring.$(OBJ) $(LIBS)

userentry$(EXE): userentry.cpp userentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ userentry.$(OBJ) $(LIBS)

permissions$(EXE): permissions.cpp permissions.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ permissions.$(OBJ) $(LIBS)

process-info$(EXE): process-info.cpp process-info.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ process-info.$(OBJ) $(LIBS)

process-exit$(EXE): process-exit.cpp process-exit.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ process-exit.$(OBJ) $(LIBS)

process-atexit$(EXE): process-atexit.cpp process-atexit.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ process-atexit.$(OBJ) $(LIBS)

process-exitimmediately$(EXE): process-exitimmediately.cpp process-exitimmediately.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ process-exitimmediately.$(OBJ) $(LIBS)

process-exitoncrash$(EXE): process-exitoncrash.cpp process-exitoncrash.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ process-exitoncrash.$(OBJ) $(LIBS)

process-handlecrash$(EXE): process-handlecrash.cpp process-handlecrash.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ process-handlecrash.$(OBJ) $(LIBS)

process-exitonshutdown$(EXE): process-exitonshutdown.cpp process-exitonshutdown.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ process-exitonshutdown.$(OBJ) $(LIBS)

process-handleshutdown$(EXE): process-handleshutdown.cpp process-handleshutdown.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ process-handleshutdown.$(OBJ) $(LIBS)

process-setuidgid$(EXE): process-setuidgid.cpp process-setuidgid.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ process-setuidgid.$(OBJ) $(LIBS)

process-seteuidgid$(EXE): process-seteuidgid.cpp process-seteuidgid.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ process-seteuidgid.$(OBJ) $(LIBS)

process-spawn$(EXE): process-spawn.cpp process-spawn.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ process-spawn.$(OBJ) $(LIBS)

process-fork$(EXE): process-fork.cpp process-fork.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ process-fork.$(OBJ) $(LIBS)

process-forkexec$(EXE): process-forkexec.cpp process-forkexec.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ process-forkexec.$(OBJ) $(LIBS)

process-zombie$(EXE): process-zombie.cpp process-zombie.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ process-zombie.$(OBJ) $(LIBS)

process-detach$(EXE): process-detach.cpp process-detach.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ process-detach.$(OBJ) $(LIBS)

protocolentry-name$(EXE): protocolentry-name.cpp protocolentry-name.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ protocolentry-name.$(OBJ) $(LIBS)

protocolentry-number$(EXE): protocolentry-number.cpp protocolentry-number.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ protocolentry-number.$(OBJ) $(LIBS)

randomnumber-static$(EXE): randomnumber-static.cpp randomnumber-static.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ randomnumber-static.$(OBJ) $(LIBS)

randomnumber-multiple$(EXE): randomnumber-multiple.cpp randomnumber-multiple.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ randomnumber-multiple.$(OBJ) $(LIBS)

bytestring-copy$(EXE): bytestring-copy.cpp bytestring-copy.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ bytestring-copy.$(OBJ) $(LIBS)

bytestring-compare$(EXE): bytestring-compare.cpp bytestring-compare.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ bytestring-compare.$(OBJ) $(LIBS)

bytestring-find$(EXE): bytestring-find.cpp bytestring-find.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ bytestring-find.$(OBJ) $(LIBS)

regularexpression-static$(EXE): regularexpression-static.cpp regularexpression-static.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ regularexpression-static.$(OBJ) $(LIBS)

regularexpression-compile$(EXE): regularexpression-compile.cpp regularexpression-compile.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ regularexpression-compile.$(OBJ) $(LIBS)

regularexpression-multiple$(EXE): regularexpression-multiple.cpp regularexpression-multiple.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ regularexpression-multiple.$(OBJ) $(LIBS)

serialport$(EXE): serialport.cpp serialport.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ serialport.$(OBJ) $(LIBS)

serialportprofile$(EXE): serialportprofile.cpp serialportprofile.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ serialportprofile.$(OBJ) $(LIBS)

serviceentry-name$(EXE): serviceentry-name.cpp serviceentry-name.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ serviceentry-name.$(OBJ) $(LIBS)

serviceentry-number$(EXE): serviceentry-number.cpp serviceentry-number.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ serviceentry-number.$(OBJ) $(LIBS)

sharedmemory1$(EXE): sharedmemory1.cpp sharedmemory1.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ sharedmemory1.$(OBJ) $(LIBS)

sharedmemory2$(EXE): sharedmemory2.cpp sharedmemory2.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ sharedmemory2.$(OBJ) $(LIBS)

semaphoreset1$(EXE): semaphoreset1.cpp semaphoreset1.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ semaphoreset1.$(OBJ) $(LIBS)

semaphoreset2$(EXE): semaphoreset2.cpp semaphoreset2.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ semaphoreset2.$(OBJ) $(LIBS)

signal$(EXE): signal.cpp signal.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ signal.$(OBJ) $(LIBS)

kill$(EXE): kill.cpp kill.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ kill.$(OBJ) $(LIBS)

singlylinkedlist-values$(EXE): singlylinkedlist-values.cpp singlylinkedlist-values.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ singlylinkedlist-values.$(OBJ) $(LIBS)

singlylinkedlist-nodes$(EXE): singlylinkedlist-nodes.cpp singlylinkedlist-nodes.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ singlylinkedlist-nodes.$(OBJ) $(LIBS)

singlylinkedlist-sort$(EXE): singlylinkedlist-sort.cpp singlylinkedlist-sort.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ singlylinkedlist-sort.$(OBJ) $(LIBS)

singlylinkedlist-iterate$(EXE): singlylinkedlist-iterate.cpp singlylinkedlist-iterate.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ singlylinkedlist-iterate.$(OBJ) $(LIBS)

singlylinkedlist-types$(EXE): singlylinkedlist-types.cpp singlylinkedlist-types.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ singlylinkedlist-types.$(OBJ) $(LIBS)

snooze$(EXE): snooze.cpp snooze.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ snooze.$(OBJ) $(LIBS)

stdio$(EXE): stdio.cpp stdio.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ stdio.$(OBJ) $(LIBS)

stringbuffer$(EXE): stringbuffer.cpp stringbuffer.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ stringbuffer.$(OBJ) $(LIBS)

sys$(EXE): sys.cpp sys.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ sys.$(OBJ) $(LIBS)

reboot$(EXE): reboot.cpp reboot.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ reboot.$(OBJ) $(LIBS)

shutdown$(EXE): shutdown.cpp shutdown.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ shutdown.$(OBJ) $(LIBS)

halt$(EXE): halt.cpp halt.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ halt.$(OBJ) $(LIBS)

unixsocketclient$(EXE): unixsocketclient.cpp unixsocketclient.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ unixsocketclient.$(OBJ) $(LIBS)

unixsocketserver$(EXE): unixsocketserver.cpp unixsocketserver.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ unixsocketserver.$(OBJ) $(LIBS)

bytebuffer$(EXE): bytebuffer.cpp bytebuffer.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ bytebuffer.$(OBJ) $(LIBS)

xmldom$(EXE): xmldom.cpp xmldom.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ xmldom.$(OBJ) $(LIBS)

xmlsax$(EXE): xmlsax.cpp xmlsax.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ xmlsax.$(OBJ) $(LIBS)

codetree$(EXE): codetree.cpp codetree.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ codetree.$(OBJ) $(LIBS)

compiler$(EXE): compiler.cpp compiler.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@ compiler.$(OBJ) $(LIBS)
