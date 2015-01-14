' source, object and header files
srcs="winsock.cpp character.cpp charstring.cpp chat.cpp client.cpp commandline.cpp crypt.cpp datetime.cpp device.cpp directory.cpp dtd.cpp dynamiclib.cpp environment.cpp error.cpp file.cpp filedescriptor.cpp stdio.cpp filesystem.cpp groupentry.cpp hostentry.cpp inetsocketclient.cpp inetsocketserver.cpp intervaltimer.cpp listener.cpp logger.cpp memorymap.cpp memorypool.cpp thread.cpp threadmutex.cpp parameterstring.cpp userentry.cpp permissions.cpp process.cpp protocolentry.cpp randomnumber.cpp bytestring.cpp regex.cpp regularexpression.cpp rpcentry.cpp semaphoreset.cpp server.cpp serviceentry.cpp sharedmemory.cpp shmfile.cpp signalclasses.cpp snooze.cpp stringbuffer.cpp sys.cpp unixsocketclient.cpp unixsocketserver.cpp bytebuffer.cpp xmldom.cpp xmldomnode.cpp xmlsax.cpp codetree.cpp inetsocketutil.cpp unixsocketutil.cpp socketserver.cpp socketclient.cpp"

lobjs="winsock.obj character.obj charstring.obj chat.obj client.obj commandline.obj crypt.obj datetime.obj device.obj directory.obj dtd.obj dynamiclib.obj environment.obj error.obj file.obj filedescriptor.obj stdio.obj filesystem.obj groupentry.obj hostentry.obj inetsocketclient.obj inetsocketserver.obj intervaltimer.obj listener.obj logger.obj memorymap.obj memorypool.obj thread.obj threadmutex.obj parameterstring.obj userentry.obj permissions.obj process.obj protocolentry.obj randomnumber.obj bytestring.obj regex.obj regularexpression.obj rpcentry.obj semaphoreset.obj server.obj serviceentry.obj sharedmemory.obj shmfile.obj signalclasses.obj snooze.obj stringbuffer.obj sys.obj unixsocketclient.obj unixsocketserver.obj bytebuffer.obj xmldom.obj xmldomnode.obj xmlsax.obj codetree.obj inetsocketutil.obj unixsocketutil.obj socketserver.obj socketclient.obj"

headers="resultcodes.h linkedlist.h singlylinkedlist.h dictionary.h dynamicarray.h staticarray.h character.h charstring.h chat.h client.h commandline.h crypt.h datetime.h device.h directory.h dtd.h dynamiclib.h environment.h error.h file.h filedescriptor.h stdio.h filesystem.h groupentry.h hostentry.h inetsocketclient.h inetsocketserver.h intervaltimer.h listener.h logger.h memorymap.h memorypool.h modemclient.h modemserver.h thread.h threadmutex.h parameterstring.h userentry.h permissions.h process.h protocolentry.h randomnumber.h bytestring.h regularexpression.h rpcentry.h semaphoreset.h serialport.h serialportprofile.h server.h serviceentry.h sharedmemory.h shmfile.h signalclasses.h snooze.h stringbuffer.h sys.h unixsocketclient.h unixsocketserver.h bytebuffer.h xmldom.h xmldomnode.h xmlsax.h codetree.h inetsocketutil.h unixsocketutil.h socketserver.h socketclient.h"
privateheaders="config.h rudimentsinlines.h inttypes.h new.h winsock.h dll.h linkedlist.h linkedlistincludes.h linkedlistinlines.h linkedlistnode.h linkedlistutilinlines.h singlylinkedlist.h singlylinkedlistincludes.h singlylinkedlistinlines.h singlylinkedlistnode.h dictionary.h dictionaryincludes.h dictionaryinlines.h dictionarynode.h dynamicarray.h dynamicarrayincludes.h dynamicarrayinlines.h staticarray.h staticarrayinlines.h characterincludes.h charstring.h charstringincludes.h chat.h chatincludes.h client.h clientincludes.h commandline.h commandlineincludes.h cryptincludes.h datetime.h datetimeincludes.h device.h deviceincludes.h directory.h directoryincludes.h dtd.h dtdincludes.h dynamiclib.h dynamiclibincludes.h environment.h environmentincludes.h errorincludes.h file.h fileincludes.h filedescriptor.h filedescriptorincludes.h stdio.h filesystem.h filesystemincludes.h groupentry.h groupentryincludes.h hostentry.h hostentryincludes.h inetsocketclient.h inetsocketclientincludes.h inetsocketserver.h inetsocketserverincludes.h intervaltimer.h intervaltimerincludes.h listener.h listenerincludes.h filedestination.h logdestination.h logger.h stderrdestination.h stdoutdestination.h syslogdestination.h loggerincludes.h memorymap.h memorymapincludes.h memorypool.h memorypoolincludes.h modemclient.h modemclientincludes.h modemserver.h modemserverincludes.h thread.h threadincludes.h threadmutex.h threadmutexincludes.h parameterstring.h parameterstringincludes.h userentry.h userentryincludes.h permissions.h permissionsincludes.h process.h processincludes.h protocolentry.h protocolentryincludes.h randomnumber.h randomnumberincludes.h bytestringincludes.h regularexpression.h regularexpressionincludes.h rpcentry.h rpcentryincludes.h semaphoreset.h semaphoresetincludes.h serialport.h serialportincludes.h serialportprofile.h serialportprofileincludes.h server.h serverincludes.h serviceentry.h serviceentryincludes.h sharedmemory.h sharedmemoryincludes.h shmfile.h shmfileincludes.h signalhandler.h signalset.h signalclassesincludes.h snoozeincludes.h stringbuffer.h stringbufferincludes.h sysincludes.h unixsocketclient.h unixsocketclientincludes.h unixsocketserver.h unixsocketserverincludes.h bytebuffer.h bytebufferincludes.h xmldom.h xmldomincludes.h xmldomnode.h xmldomnodeincludes.h xmlsax.h xmlsaxincludes.h codetree.h codetreeincludes.h inetsocketutil.h inetsocketutilincludes.h unixsocketutil.h unixsocketutilincludes.h socketserver.h socketserverincludes.h socketclient.h socketclientincludes.h modemutil.h"

' version
RUDIMENTS_VERSION=0.50

' paths
prefix="C:\""Program Files""\Firstworks"
exec_prefix=prefix
bindir = prefix+"\bin"
includedir = prefix+"\include"
libdir = prefix+"\lib"
mandir = prefix+"\share\man"
datadir = prefix+"\share"

' libraries
SOCKETLIBS=""
PTHREADLIB=""
SSLLIBS=""
PCRELIBS=""
NANOSLEEPLIB=""
CLOCKNANOSLEEPLIB=""
SHMOPENLIB=""
CRYPTLIB=""
INETATONLIB=""
NETAPI32LIB="ws2_32.lib netapi32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib"
BELIB=""
GNULIB=""
CRTLIB=""
DLLIB=""
GETPGIDLIB=""


' create file system object
set fso=CreateObject("Scripting.FileSystemObject")


' get top_builddir
top_builddir=fso.GetAbsolutePathName(".")


' input and output files
infiles=Array(_
	"config.windows.mk",_
	"include\\rudiments\\private\\config.windows.h",_
	"src\\Makefile.in",_
	"include\\Makefile.in",_
	"bin\\rudiments-config.in",_
	"rudiments.pc.in"_
	)
outfiles=Array(_
	"config.mk",_
	"include\\rudiments\\private\\config.h",_
	"src\\Makefile",_
	"include\\Makefile",_
	"bin\\rudiments-config",_
	"rudiments.pc"_
	)


' create output files
for i=lbound(infiles) to ubound(infiles)

	' read input file
	set infile=fso.OpenTextFile(infiles(i))
	content=infile.ReadAll()

	' source, object and header files
	content=replace(content,"@SRCS@",srcs,1,-1,0)
	content=replace(content,"@LOBJS@",lobjs,1,-1,0)
	content=replace(content,"@HEADERS@",headers,1,-1,0)
	content=replace(content,"@PRIVATEHEADERS@",privateheaders,1,-1,0)

	' version
	content=replace(content,"@RUDIMENTS_VERSION@",RUDIMENTS_VERSION,1,-1,0)

	' paths
	content=replace(content,"@prefix@",prefix,1,-1,0)
	content=replace(content,"@exec_prefix@",exec_prefix,1,-1,0)
	content=replace(content,"@bindir@",bindir,1,-1,0)
	content=replace(content,"@includedir@",includedir,1,-1,0)
	content=replace(content,"@libdir@",libdir,1,-1,0)
	content=replace(content,"@mandir@",mandir,1,-1,0)
	content=replace(content,"@datadir@",datadir,1,-1,0)

	' libraries
	content=replace(content,"@SOCKETLIBS@",SOCKETLIBS,1,-1,0)
	content=replace(content,"@PTHREADLIB@",PTHREADLIB,1,-1,0)
	content=replace(content,"@SSLLIBS@",SSLLIBS,1,-1,0)
	content=replace(content,"@PCRELIBS@",PCRELIBS,1,-1,0)
	content=replace(content,"@NANOSLEEPLIB@",NANOSLEEPLIB,1,-1,0)
	content=replace(content,"@CLOCKNANOSLEEPLIB@",CLOCKNANOSLEEPLIB,1,-1,0)
	content=replace(content,"@SHMOPENLIB@",SHMOPENLIB,1,-1,0)
	content=replace(content,"@CRYPTLIB@",CRYPTLIB,1,-1,0)
	content=replace(content,"@INETATONLIB@",INETATONLIB,1,-1,0)
	content=replace(content,"@NETAPI32LIB@",NETAPI32LIB,1,-1,0)
	content=replace(content,"@BELIB@",BELIB,1,-1,0)
	content=replace(content,"@GNULIB@",GNULIB,1,-1,0)
	content=replace(content,"@CRTLIB@",CRTLIB,1,-1,0)
	content=replace(content,"@DLLIB@",DLLIB,1,-1,0)
	content=replace(content,"@GETPGIDLIB@",GETPGIDLIB,1,-1,0)

	' top_builddir
	content=replace(content,"@top_builddir@",top_builddir,1,-1,0)

	' write output file
	set outfile=fso.OpenTextFile(outfiles(i),2,true)
	call outfile.Write(content)
	call outfile.Close()
next
