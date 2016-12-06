top_builddir = ..

include ../config.mk

CPPFLAGS = $(BASECPPFLAGS) $(INC)../ $(INC)../include
LIBS=$(RUDIMENTSLIB) $(SOCKETLIBS) $(PTHREADLIB) $(READLINELIBS) $(SSLLIBS) $(GSSLIBS) $(PCRELIBS) $(LIBCURLLIBS) $(NANOSLEEPLIB) $(CLOCKNANOSLEEPLIB) $(CRYPTLIB) $(INETATONLIB) $(BELIB) $(GNULIB) $(CRTLIB) $(DLLIB) $(GETPGIDLIB) $(DEBUGLIB)

PROGS0 = listener \
	directory \
	groupentry \
	commandline \
	parameterstring \
	hostentry \
	userentry \
	serviceentry \
	permissions \
	protocolentry \
	regularexpression \
	semaphoreset \
	charstring \
	datetime

PROGS1= dictionary \
	environment \
	file \
	filesystem \
	linkedlist \
	avltree \
	singlylinkedlist \
	listmetrics \
	memorypool \
	randomnumber \
	sharedmemory \
	signal \
	kill \
	stringbuffer \
	bytebuffer \
	crypt

PROGS2= dynamicarray \
	staticarray \
	dynamiclib \
	error \
	thread \
	threadmutex \
	process \
	snooze \
	memorymap \
	character \
	bytestring \
	tlsserver \
	tlsclient

PROGS3= handoff \
	xmldom \
	logger \
	sys \
	reboot \
	halt \
	shutdown \
	url \
	gssserver \
	gssclient \
	md5sum

PROGS = $(PROGS0) $(PROGS1) $(PROGS2) $(PROGS3)

#ALLSTATIC=-all-static 

.SUFFIXES: .lo .obj

.cpp.lo:
	$(LTCOMPILE) $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $< $(OUT)$@

.cpp.obj:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $<

all: $(PROGS) $(SERVERPEM)

tests: all
	$(THISDIR)bytebuffer$(EXE)
	$(THISDIR)bytestring$(EXE)
	$(THISDIR)character$(EXE)
	$(THISDIR)charstring$(EXE)
	$(THISDIR)commandline$(EXE)
	#$(THISDIR)crypt$(EXE)
	$(THISDIR)datetime$(EXE)
	$(THISDIR)dictionary$(EXE)
	$(THISDIR)directory$(EXE)
	$(THISDIR)dynamicarray$(EXE)
	$(THISDIR)dynamiclib$(EXE)
	$(THISDIR)environment$(EXE)
	$(THISDIR)error$(EXE)
	$(THISDIR)file$(EXE)
	$(THISDIR)filesystem$(EXE)
	$(THISDIR)groupentry$(EXE)
	$(THISDIR)handoff$(EXE)
	$(THISDIR)hostentry$(EXE)
	$(THISDIR)linkedlist$(EXE)
	$(THISDIR)listener$(EXE)
	$(THISDIR)logger$(EXE)
	$(THISDIR)memorymap$(EXE)
	$(THISDIR)memorypool$(EXE)
	$(THISDIR)parameterstring$(EXE)
	$(THISDIR)permissions$(EXE)
	$(THISDIR)process$(EXE)
	$(THISDIR)protocolentry$(EXE)
	$(THISDIR)randomnumber$(EXE)
	$(THISDIR)regularexpression$(EXE)
	$(THISDIR)semaphoreset$(EXE)
	$(THISDIR)serviceentry$(EXE)
	$(THISDIR)sharedmemory$(EXE)
	$(THISDIR)signal$(EXE)
	$(THISDIR)singlylinkedlist$(EXE)
	$(THISDIR)snooze$(EXE)
	$(THISDIR)staticarray$(EXE)
	$(THISDIR)stringbuffer$(EXE)
	$(THISDIR)sys$(EXE)
	$(THISDIR)thread$(EXE)
	$(THISDIR)threadmutex$(EXE)
	$(THISDIR)url$(EXE)
	$(THISDIR)userentry$(EXE)
	$(THISDIR)xmldom$(EXE)
	#$(THISDIR)gssclient$(EXE)
	#$(THISDIR)gssserver$(EXE)
	#$(THISDIR)tlsclient$(EXE)
	#$(THISDIR)tlsserver$(EXE)
	
clean:
	$(LTCLEAN) $(RM) $(PROGS0)
	$(LTCLEAN) $(RM) $(PROGS1)
	$(LTCLEAN) $(RM) $(PROGS2)
	$(LTCLEAN) $(RM) $(PROGS3)
	$(LTCLEAN) $(RM) *.exe *.ilk *.idb *.pdb *.manifest *.lo *.o *.obj test.log testfile *.pem *.pfx *.xml *.sck *.ii
	cd certificates $(AND) $(MAKE) clean

tlsserver: tlsserver.cpp tlsserver.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) tlsserver.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

listener: listener.cpp listener.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) listener.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

tlsclient: tlsclient.cpp tlsclient.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) tlsclient.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

directory: directory.cpp directory.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) directory.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

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

avltree: avltree.cpp avltree.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) avltree.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

singlylinkedlist: singlylinkedlist.cpp singlylinkedlist.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) singlylinkedlist.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

listmetrics: listmetrics.cpp listmetrics.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) listmetrics.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

memorypool: memorypool.cpp memorypool.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) memorypool.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

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

semaphoreset: semaphoreset.cpp semaphoreset.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) semaphoreset.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

sharedmemory: sharedmemory.cpp sharedmemory.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) sharedmemory.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

charstring: charstring.cpp charstring.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) charstring.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

datetime: datetime.cpp datetime.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) datetime.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

dictionary: dictionary.cpp dictionary.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) dictionary.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

randomnumber: randomnumber.cpp randomnumber.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) randomnumber.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

signal: signal.cpp signal.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) signal.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

kill: kill.cpp kill.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) kill.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

stringbuffer: stringbuffer.cpp stringbuffer.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) stringbuffer.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

handoff: handoff.cpp handoff.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) handoff.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

bytebuffer: bytebuffer.cpp bytebuffer.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) bytebuffer.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

xmldom: xmldom.cpp xmldom.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) xmldom.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

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

thread: thread.cpp thread.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) thread.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

threadmutex: threadmutex.cpp threadmutex.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) threadmutex.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

process: process.cpp process.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) process.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

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

url: url.cpp url.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) url.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

gssserver: gssserver.cpp gssserver.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) gssserver.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

gssclient: gssclient.cpp gssclient.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) gssclient.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

md5sum: md5sum.cpp md5sum.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) md5sum.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

server.pem:
	cd certificates $(AND) $(MAKE) clean all
	cp certificates/certificates/ca.pem .
	cp certificates/certificatechains/server.pem .
	cp certificates/certificatechains/client.pem .
	cp certificates/certificates/ca.pfx .
	cp certificates/certificatechains/server.pfx .
	cp certificates/certificatechains/client.pfx .
