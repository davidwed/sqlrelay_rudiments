top_builddir = ..

include ../config.mk

.PHONY: all clean rebuild

CPPFLAGS = $(BASECPPFLAGS) -I../ -I../include
LIBS=-L../src -lrudiments $(SOCKETLIBS) $(PTHREADLIB) $(SSLLIBS) $(PCRELIBS) $(NANOSLEEPLIB) $(CLOCKNANOSLEEPLIB) $(SHMOPENLIB) $(CRYPTLIB) $(INETATONLIB) $(DLLIB)

#ALLSTATIC=-all-static

.SUFFIXES: .lo

all: server listener inetclientsocket unixclientsocket directory daemonprocess groupentry commandline parameterstring hostentry logger passwdentry serviceentry shadowentry permissions protocolentry regularexpression rpcentry semaphoreset2 semaphoreset2 charstring datetime timezonefile dictionary dtd environment file filesystem linkedlist memorypool randomnumber sharedmemory1 sharedmemory1 signal stringbuffer variablebuffer intervaltimer device serialport shmfile chat crypt dynamiclib error math mutex process serialportprofile snooze memorymap character rawbuffer sslsvr sslclnt oneshotclnt oneshotsvr lock1 lock2 handoff1 handoff2 handoffclient nonhandoff1 nonhandoff2 nonhandoffclient xattr xmls xmld inetclntbench inetsvrbench server.pem nullserver

clean:
	$(LIBTOOL) --mode=clean $(RM) ca.pem
	$(LIBTOOL) --mode=clean $(RM) server.pem
	$(LIBTOOL) --mode=clean $(RM) client.pem
	$(LIBTOOL) --mode=clean $(RM) *.lo
	$(LIBTOOL) --mode=clean $(RM) *.o
	$(LIBTOOL) --mode=clean $(RM) *.exe
	$(LIBTOOL) --mode=clean $(RM) listener server sslsvr inetclientsocket sslclnt unixclientsocket directory daemonprocess groupentry commandline parameterstring hostentry logger oneshotclnt oneshotsvr passwdentry serviceentry shadowentry permissions protocolentry regularexpression rpcentry semaphoreset1 semaphoreset2 sharedmemory1 sharedmemory2 charstring datetime timezonefile dictionary dtd environment file filesystem linkedlist lock1 lock2 memorypool randomnumber signal stringbuffer handoff1 handoff2 handoffclient nonhandoff1 nonhandoff2 nonhandoffclient variablebuffer xattr xmls xmld intervaltimer inetclntbench inetsvrbench device serialport shmfile chat crypt dynamiclib error math mutex process serialportprofile snooze memorymap character rawbuffer nullserver
	$(RM) test.log
	$(RMTREE) .libs
	$(RM) testfile
	$(MAKE) -C certificates clean
	$(RM) test.xml

rebuild: clean all

.C.lo:
	$(LIBTOOL) --mode=compile $(CXX) $(CXXFLAGS) $(CPPFLAGS) -g -c $< -o $@

server: server.C server.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ server.lo $(ALLSTATIC) $(LIBS)

nullserver: nullserver.C nullserver.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ nullserver.lo $(ALLSTATIC) $(LIBS)

sslsvr: sslsvr.C sslsvr.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ sslsvr.lo $(ALLSTATIC) $(LIBS)

listener: listener.C listener.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ listener.lo $(ALLSTATIC) $(LIBS)

inetclientsocket: inetclientsocket.C inetclientsocket.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ inetclientsocket.lo $(ALLSTATIC) $(LIBS)

sslclnt: sslclnt.C sslclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sslclnt.lo $(ALLSTATIC) $(LIBS)

unixclientsocket: unixclientsocket.C unixclientsocket.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ unixclientsocket.lo $(ALLSTATIC) $(LIBS)

directory: directory.C directory.lo 
	#$(LIBTOOL) --mode=link $(CXX) -o $@ directory.lo $(ALLSTATIC) $(LIBS)
	$(LIBTOOL) --mode=link $(CXX) -o $@ directory.lo $(LIBS) -lefence

daemonprocess: daemonprocess.C daemonprocess.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ daemonprocess.lo $(ALLSTATIC) $(LIBS)

commandline: commandline.C commandline.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ commandline.lo $(ALLSTATIC) $(LIBS)

parameterstring: parameterstring.C parameterstring.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ parameterstring.lo $(ALLSTATIC) $(LIBS)

logger: logger.C logger.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ logger.lo $(ALLSTATIC) $(LIBS)

environment: environment.C environment.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ environment.lo $(ALLSTATIC) $(LIBS)

file: file.C file.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ file.lo $(ALLSTATIC) $(LIBS)

filesystem: filesystem.C filesystem.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ filesystem.lo $(ALLSTATIC) $(LIBS)

groupentry: groupentry.C groupentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ groupentry.lo $(ALLSTATIC) $(LIBS)

hostentry: hostentry.C hostentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ hostentry.lo $(ALLSTATIC) $(LIBS)

linkedlist: linkedlist.C linkedlist.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ linkedlist.lo $(ALLSTATIC) $(LIBS)

lock1: lock1.C lock1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ lock1.lo $(ALLSTATIC) $(LIBS)

lock2: lock2.C lock2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ lock2.lo $(ALLSTATIC) $(LIBS)

memorypool: memorypool.C memorypool.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ memorypool.lo $(ALLSTATIC) $(LIBS)

oneshotclnt: oneshotclnt.C oneshotclnt.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ oneshotclnt.lo $(ALLSTATIC) $(LIBS)

oneshotsvr: oneshotsvr.C oneshotsvr.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ oneshotsvr.lo $(ALLSTATIC) $(LIBS)

passwdentry: passwdentry.C passwdentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ passwdentry.lo $(ALLSTATIC) $(LIBS)

serviceentry: serviceentry.C serviceentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ serviceentry.lo $(ALLSTATIC) $(LIBS)

shadowentry: shadowentry.C shadowentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ shadowentry.lo $(ALLSTATIC) $(LIBS)

permissions: permissions.C permissions.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ permissions.lo $(ALLSTATIC) $(LIBS)

protocolentry: protocolentry.C protocolentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ protocolentry.lo $(ALLSTATIC) $(LIBS)

regularexpression: regularexpression.C regularexpression.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ regularexpression.lo $(ALLSTATIC) $(LIBS)

rpcentry: rpcentry.C rpcentry.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ rpcentry.lo $(ALLSTATIC) $(LIBS)

semaphoreset1: semaphoreset1.C semaphoreset1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ semaphoreset1.lo $(ALLSTATIC) $(LIBS)

semaphoreset2: semaphoreset2.C semaphoreset2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ semaphoreset2.lo $(ALLSTATIC) $(LIBS)

sharedmemory1: sharedmemory1.C sharedmemory1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sharedmemory1.lo $(ALLSTATIC) $(LIBS)

sharedmemory2: sharedmemory2.C sharedmemory2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ sharedmemory2.lo $(ALLSTATIC) $(LIBS)

charstring: charstring.C charstring.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ charstring.lo $(ALLSTATIC) $(LIBS)

datetime: datetime.C datetime.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ datetime.lo $(ALLSTATIC) $(LIBS)

timezonefile: timezonefile.C timezonefile.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ timezonefile.lo $(ALLSTATIC) $(LIBS)

dictionary: dictionary.C dictionary.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ dictionary.lo $(ALLSTATIC) $(LIBS)

dtd: dtd.C dtd.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ dtd.lo $(ALLSTATIC) $(LIBS)

randomnumber: randomnumber.C randomnumber.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ randomnumber.lo $(ALLSTATIC) $(LIBS)

signal: signal.C signal.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ signal.lo $(ALLSTATIC) $(LIBS)

stringbuffer: stringbuffer.C stringbuffer.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ stringbuffer.lo $(ALLSTATIC) $(LIBS)

handoff1: handoff1.C handoff1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ handoff1.lo $(ALLSTATIC) $(LIBS)

handoff2: handoff2.C handoff2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ handoff2.lo $(ALLSTATIC) $(LIBS)

handoffclient: handoffclient.C handoffclient.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ handoffclient.lo $(ALLSTATIC) $(LIBS)

nonhandoff1: nonhandoff1.C nonhandoff1.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ nonhandoff1.lo $(ALLSTATIC) $(LIBS)

nonhandoff2: nonhandoff2.C nonhandoff2.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ nonhandoff2.lo $(ALLSTATIC) $(LIBS)

nonhandoffclient: nonhandoffclient.C nonhandoffclient.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ nonhandoffclient.lo $(ALLSTATIC) $(LIBS)

variablebuffer: variablebuffer.C variablebuffer.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ variablebuffer.lo $(ALLSTATIC) $(LIBS)

xattr: xattr.C xattr.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ xattr.lo $(ALLSTATIC) $(LIBS)

xmls: xmls.C xmls.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ xmls.lo $(ALLSTATIC) $(LIBS)

xmld: xmld.C xmld.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ xmld.lo $(ALLSTATIC) $(LIBS)

intervaltimer: intervaltimer.C intervaltimer.lo
	$(LIBTOOL) --mode=link $(CXX) -o $@ intervaltimer.lo $(ALLSTATIC) $(LIBS)

inetclntbench: inetclntbench.C inetclntbench.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ inetclntbench.lo $(ALLSTATIC) $(LIBS)

inetsvrbench: inetsvrbench.C inetsvrbench.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ inetsvrbench.lo $(ALLSTATIC) $(LIBS)

device: device.C device.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ device.lo $(ALLSTATIC) $(LIBS)

serialport: serialport.C serialport.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ serialport.lo $(ALLSTATIC) $(LIBS)

shmfile: shmfile.C shmfile.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ shmfile.lo $(ALLSTATIC) $(LIBS)

chat: chat.C chat.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ chat.lo $(ALLSTATIC) $(LIBS)

crypt: crypt.C crypt.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ crypt.lo $(ALLSTATIC) $(LIBS)

dynamiclib: dynamiclib.C dynamiclib.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ dynamiclib.lo $(ALLSTATIC) $(LIBS)

error: error.C error.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ error.lo $(ALLSTATIC) $(LIBS)

math: math.C math.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ math.lo $(ALLSTATIC) $(LIBS)

mutex: mutex.C mutex.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ mutex.lo $(ALLSTATIC) $(LIBS)

process: process.C process.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ process.lo $(ALLSTATIC) $(LIBS)

serialportprofile: serialportprofile.C serialportprofile.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ serialportprofile.lo $(ALLSTATIC) $(LIBS)

snooze: snooze.C snooze.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ snooze.lo $(ALLSTATIC) $(LIBS)

memorymap: memorymap.C memorymap.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ memorymap.lo $(ALLSTATIC) $(LIBS)

character: character.C character.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ character.lo $(ALLSTATIC) $(LIBS)

rawbuffer: rawbuffer.C rawbuffer.lo 
	$(LIBTOOL) --mode=link $(CXX) -o $@ rawbuffer.lo $(ALLSTATIC) $(LIBS)

server.pem:
	$(MAKE) -C certificates
	cp certificates/certificates/ca.pem .
	cp certificates/certificatechains/server.pem .
	cp certificates/certificatechains/client.pem .
