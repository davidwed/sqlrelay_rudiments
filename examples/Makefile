top_builddir = ..

include ../config.mk

.PHONY: all clean

CPPFLAGS = $(BASECPPFLAGS) -I../ -I../include
LIBS=-L../src -lrudiments $(SOCKETLIBS) $(PTHREADLIB) $(SSLLIBS) $(PCRELIBS) $(NANOSLEEPLIB) $(CLOCKNANOSLEEPLIB) $(SHMOPENLIB) $(CRYPTLIB) $(INETATONLIB) $(BELIB) $(GNULIB) $(CRTLIB) $(DLLIB)

PROGS = \
	character$(EXE) \
	charstring$(EXE) \
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
	intervaltimer$(EXE) \
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
	process$(EXE) \
	protocolentry$(EXE) \
	randomnumber$(EXE) \
	bytestring$(EXE) \
	regularexpression$(EXE) \
	rpcentry$(EXE) \
	serialport$(EXE) \
	serialportprofile$(EXE) \
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

.SUFFIXES: .lo

.cpp.lo:
	$(LIBTOOL) --mode=compile $(CXX) $(CXXFLAGS) $(CPPFLAGS) -g -c $< -o $@

all: $(PROGS)

clean:
	$(LIBTOOL) --mode=clean $(RM) $(PROGS) *.lo *.o
	$(RMTREE) .libs

character$(EXE): character.cpp character.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ character.lo $(LIBS)

charstring$(EXE): charstring.cpp charstring.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ charstring.lo $(LIBS)

chat$(EXE): chat.cpp chat.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ chat.lo $(LIBS)

clientserverfactory$(EXE): clientserverfactory.cpp clientserverfactory.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ clientserverfactory.lo $(LIBS)

commandline$(EXE): commandline.cpp commandline.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ commandline.lo $(LIBS)

complexinetsocketserver$(EXE): complexinetsocketserver.cpp complexinetsocketserver.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ complexinetsocketserver.lo $(LIBS)

crypt$(EXE): crypt.cpp crypt.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ crypt.lo $(LIBS)

datetime$(EXE): datetime.cpp datetime.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ datetime.lo $(LIBS)

device$(EXE): device.cpp device.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ device.lo $(LIBS)

dictionary$(EXE): dictionary.cpp dictionary.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ dictionary.lo $(LIBS)

dynamiclib$(EXE): dynamiclib.cpp dynamiclib.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ dynamiclib.lo $(LIBS)

environment$(EXE): environment.cpp environment.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ environment.lo $(LIBS)

error$(EXE): error.cpp error.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ error.lo $(LIBS)

file$(EXE): file.cpp file.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ file.lo $(LIBS)

filesystem$(EXE): filesystem.cpp filesystem.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ filesystem.lo $(LIBS)

groupentry$(EXE): groupentry.cpp groupentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ groupentry.lo $(LIBS)

hostentry$(EXE): hostentry.cpp hostentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ hostentry.lo $(LIBS)

inetsocketclient$(EXE): inetsocketclient.cpp inetsocketclient.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ inetsocketclient.lo $(LIBS)

inetsocketserver$(EXE): inetsocketserver.cpp inetsocketserver.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ inetsocketserver.lo $(LIBS)

intervaltimer$(EXE): intervaltimer.cpp intervaltimer.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ intervaltimer.lo $(LIBS)

linkedlist$(EXE): linkedlist.cpp linkedlist.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ linkedlist.lo $(LIBS)

listener$(EXE): listener.cpp listener.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ listener.lo $(LIBS)

logger$(EXE): logger.cpp logger.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ logger.lo $(LIBS)

memorymap$(EXE): memorymap.cpp memorymap.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ memorymap.lo $(LIBS)

memorypool$(EXE): memorypool.cpp memorypool.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ memorypool.lo $(LIBS)

modemclient$(EXE): modemclient.cpp modemclient.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ modemclient.lo $(LIBS)

modemserver$(EXE): modemserver.cpp modemserver.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ modemserver.lo $(LIBS)

thread$(EXE): thread.cpp thread.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ thread.lo $(LIBS)

threadmutex$(EXE): threadmutex.cpp threadmutex.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ threadmutex.lo $(LIBS)

parameterstring$(EXE): parameterstring.cpp parameterstring.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ parameterstring.lo $(LIBS)

userentry$(EXE): userentry.cpp userentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ userentry.lo $(LIBS)

permissions$(EXE): permissions.cpp permissions.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ permissions.lo $(LIBS)

process$(EXE): process.cpp process.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ process.lo $(LIBS)

protocolentry$(EXE): protocolentry.cpp protocolentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ protocolentry.lo $(LIBS)

randomnumber$(EXE): randomnumber.cpp randomnumber.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ randomnumber.lo $(LIBS)

bytestring$(EXE): bytestring.cpp bytestring.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ bytestring.lo $(LIBS)

regularexpression$(EXE): regularexpression.cpp regularexpression.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ regularexpression.lo $(LIBS)

rpcentry$(EXE): rpcentry.cpp rpcentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ rpcentry.lo $(LIBS)

serialport$(EXE): serialport.cpp serialport.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ serialport.lo $(LIBS)

serialportprofile$(EXE): serialportprofile.cpp serialportprofile.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ serialportprofile.lo $(LIBS)

serviceentry$(EXE): serviceentry.cpp serviceentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ serviceentry.lo $(LIBS)

sharedmemory1$(EXE): sharedmemory1.cpp sharedmemory1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sharedmemory1.lo $(LIBS)

sharedmemory2$(EXE): sharedmemory2.cpp sharedmemory2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sharedmemory2.lo $(LIBS)

shmfile$(EXE): shmfile.cpp shmfile.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ shmfile.lo $(LIBS)

signal$(EXE): signal.cpp signal.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ signal.lo $(LIBS)

snooze$(EXE): snooze.cpp snooze.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ snooze.lo $(LIBS)

stdio$(EXE): stdio.cpp stdio.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ stdio.lo $(LIBS)

stringbuffer$(EXE): stringbuffer.cpp stringbuffer.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ stringbuffer.lo $(LIBS)

sys$(EXE): sys.cpp sys.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sys.lo $(LIBS)

unixsocketclient$(EXE): unixsocketclient.cpp unixsocketclient.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ unixsocketclient.lo $(LIBS)

unixsocketserver$(EXE): unixsocketserver.cpp unixsocketserver.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ unixsocketserver.lo $(LIBS)

bytebuffer$(EXE): bytebuffer.cpp bytebuffer.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ bytebuffer.lo $(LIBS)

xmldom$(EXE): xmldom.cpp xmldom.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ xmldom.lo $(LIBS)

xmlsax$(EXE): xmlsax.cpp xmlsax.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ xmlsax.lo $(LIBS)

codetree$(EXE): codetree.cpp codetree.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ codetree.lo $(LIBS)
