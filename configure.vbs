' source, object and header files
srcs="winsock.cpp "+_
	"character.cpp "+_
	"charstring.cpp "+_
	"chat.cpp "+_
	"client.cpp "+_
	"commandline.cpp "+_
	"crypt.cpp "+_
	"datetime.cpp "+_
	"device.cpp "+_
	"directory.cpp "+_
	"dtd.cpp "+_
	"dynamiclib.cpp "+_
	"environment.cpp "+_
	"error.cpp "+_
	"file.cpp "+_
	"filedescriptor.cpp "+_
	"stdio.cpp "+_
	"filesystem.cpp "+_
	"groupentry.cpp "+_
	"hostentry.cpp "+_
	"inetsocketclient.cpp "+_
	"inetsocketserver.cpp "+_
	"intervaltimer.cpp "+_
	"listener.cpp "+_
	"logger.cpp "+_
	"memorymap.cpp "+_
	"memorypool.cpp "+_
	"thread.cpp "+_
	"threadmutex.cpp "+_
	"parameterstring.cpp "+_
	"userentry.cpp "+_
	"permissions.cpp "+_
	"process.cpp "+_
	"protocolentry.cpp "+_
	"randomnumber.cpp "+_
	"bytestring.cpp "+_
	"regex.cpp "+_
	"regularexpression.cpp "+_
	"rpcentry.cpp "+_
	"semaphoreset.cpp "+_
	"server.cpp "+_
	"serviceentry.cpp "+_
	"sharedmemory.cpp "+_
	"shmfile.cpp "+_
	"signalclasses.cpp "+_
	"snooze.cpp "+_
	"stringbuffer.cpp "+_
	"sys.cpp "+_
	"unixsocketclient.cpp "+_
	"unixsocketserver.cpp "+_
	"bytebuffer.cpp "+_
	"xmldom.cpp "+_
	"xmldomnode.cpp "+_
	"xmlsax.cpp "+_
	"codetree.cpp "+_
	"inetsocketutil.cpp "+_
	"unixsocketutil.cpp "+_
	"socketserver.cpp "+_
	"socketclient.cpp"

lobjs="winsock.obj "+_
	"character.obj "+_
	"charstring.obj "+_
	"chat.obj "+_
	"client.obj "+_
	"commandline.obj "+_
	"crypt.obj "+_
	"datetime.obj "+_
	"device.obj "+_
	"directory.obj "+_
	"dtd.obj "+_
	"dynamiclib.obj "+_
	"environment.obj "+_
	"error.obj "+_
	"file.obj "+_
	"filedescriptor.obj "+_
	"stdio.obj "+_
	"filesystem.obj "+_
	"groupentry.obj "+_
	"hostentry.obj "+_
	"inetsocketclient.obj "+_
	"inetsocketserver.obj "+_
	"intervaltimer.obj "+_
	"listener.obj "+_
	"logger.obj "+_
	"memorymap.obj "+_
	"memorypool.obj "+_
	"thread.obj "+_
	"threadmutex.obj "+_
	"parameterstring.obj "+_
	"userentry.obj "+_
	"permissions.obj "+_
	"process.obj "+_
	"protocolentry.obj "+_
	"randomnumber.obj "+_
	"bytestring.obj "+_
	"regex.obj "+_
	"regularexpression.obj "+_
	"rpcentry.obj "+_
	"semaphoreset.obj "+_
	"server.obj "+_
	"serviceentry.obj "+_
	"sharedmemory.obj "+_
	"shmfile.obj "+_
	"signalclasses.obj "+_
	"snooze.obj "+_
	"stringbuffer.obj "+_
	"sys.obj "+_
	"unixsocketclient.obj "+_
	"unixsocketserver.obj "+_
	"bytebuffer.obj "+_
	"xmldom.obj "+_
	"xmldomnode.obj "+_
	"xmlsax.obj "+_
	"codetree.obj "+_
	"inetsocketutil.obj "+_
	"unixsocketutil.obj "+_
	"socketserver.obj "+_
	"socketclient.obj"

headers="resultcodes.h "+_
	"linkedlist.h "+_
	"singlylinkedlist.h "+_
	"dictionary.h "+_
	"dynamicarray.h "+_
	"staticarray.h "+_
	"character.h "+_
	"charstring.h "+_
	"chat.h "+_
	"client.h "+_
	"commandline.h "+_
	"crypt.h "+_
	"datetime.h "+_
	"device.h "+_
	"directory.h "+_
	"dtd.h "+_
	"dynamiclib.h "+_
	"environment.h "+_
	"error.h "+_
	"file.h "+_
	"filedescriptor.h "+_
	"stdio.h "+_
	"filesystem.h "+_
	"groupentry.h "+_
	"hostentry.h "+_
	"inetsocketclient.h "+_
	"inetsocketserver.h "+_
	"intervaltimer.h "+_
	"listener.h "+_
	"logger.h "+_
	"memorymap.h "+_
	"memorypool.h "+_
	"modemclient.h "+_
	"modemserver.h "+_
	"thread.h "+_
	"threadmutex.h "+_
	"parameterstring.h "+_
	"userentry.h "+_
	"permissions.h "+_
	"process.h "+_
	"protocolentry.h "+_
	"randomnumber.h "+_
	"bytestring.h "+_
	"regularexpression.h "+_
	"rpcentry.h "+_
	"semaphoreset.h "+_
	"serialport.h "+_
	"serialportprofile.h "+_
	"server.h "+_
	"serviceentry.h "+_
	"sharedmemory.h "+_
	"shmfile.h "+_
	"signalclasses.h "+_
	"snooze.h "+_
	"stringbuffer.h "+_
	"sys.h "+_
	"unixsocketclient.h "+_
	"unixsocketserver.h "+_
	"bytebuffer.h "+_
	"xmldom.h "+_
	"xmldomnode.h "+_
	"xmlsax.h "+_
	"codetree.h "+_
	"inetsocketutil.h "+_
	"unixsocketutil.h "+_
	"socketserver.h "+_
	"socketclient.h"

privateheaders="config.h "+_
	"rudimentsinlines.h "+_
	"inttypes.h "+_
	"new.h "+_
	"winsock.h "+_
	"dll.h "+_
	"linkedlist.h "+_
	"linkedlistincludes.h "+_
	"linkedlistinlines.h "+_
	"linkedlistnode.h "+_
	"linkedlistutilinlines.h "+_
	"singlylinkedlist.h "+_
	"singlylinkedlistincludes.h "+_
	"singlylinkedlistinlines.h "+_
	"singlylinkedlistnode.h "+_
	"dictionary.h "+_
	"dictionaryincludes.h "+_
	"dictionaryinlines.h "+_
	"dictionarynode.h "+_
	"dynamicarray.h "+_
	"dynamicarrayincludes.h "+_
	"dynamicarrayinlines.h "+_
	"staticarray.h "+_
	"staticarrayinlines.h "+_
	"characterincludes.h "+_
	"charstring.h "+_
	"charstringincludes.h "+_
	"chat.h "+_
	"chatincludes.h "+_
	"client.h "+_
	"clientincludes.h "+_
	"commandline.h "+_
	"commandlineincludes.h "+_
	"cryptincludes.h "+_
	"datetime.h "+_
	"datetimeincludes.h "+_
	"device.h "+_
	"deviceincludes.h "+_
	"directory.h "+_
	"directoryincludes.h "+_
	"dtd.h "+_
	"dtdincludes.h "+_
	"dynamiclib.h "+_
	"dynamiclibincludes.h "+_
	"environment.h "+_
	"environmentincludes.h "+_
	"errorincludes.h "+_
	"file.h "+_
	"fileincludes.h "+_
	"filedescriptor.h "+_
	"filedescriptorincludes.h "+_
	"stdio.h "+_
	"filesystem.h "+_
	"filesystemincludes.h "+_
	"groupentry.h "+_
	"groupentryincludes.h "+_
	"hostentry.h "+_
	"hostentryincludes.h "+_
	"inetsocketclient.h "+_
	"inetsocketclientincludes.h "+_
	"inetsocketserver.h "+_
	"inetsocketserverincludes.h "+_
	"intervaltimer.h "+_
	"intervaltimerincludes.h "+_
	"listener.h "+_
	"listenerincludes.h "+_
	"filedestination.h "+_
	"logdestination.h "+_
	"logger.h "+_
	"stderrdestination.h "+_
	"stdoutdestination.h "+_
	"syslogdestination.h "+_
	"loggerincludes.h "+_
	"memorymap.h "+_
	"memorymapincludes.h "+_
	"memorypool.h "+_
	"memorypoolincludes.h "+_
	"modemclient.h "+_
	"modemclientincludes.h "+_
	"modemserver.h "+_
	"modemserverincludes.h "+_
	"thread.h "+_
	"threadincludes.h "+_
	"threadmutex.h "+_
	"threadmutexincludes.h "+_
	"parameterstring.h "+_
	"parameterstringincludes.h "+_
	"userentry.h "+_
	"userentryincludes.h "+_
	"permissions.h "+_
	"permissionsincludes.h "+_
	"process.h "+_
	"processincludes.h "+_
	"protocolentry.h "+_
	"protocolentryincludes.h "+_
	"randomnumber.h "+_
	"randomnumberincludes.h "+_
	"bytestringincludes.h "+_
	"regularexpression.h "+_
	"regularexpressionincludes.h "+_
	"rpcentry.h "+_
	"rpcentryincludes.h "+_
	"semaphoreset.h "+_
	"semaphoresetincludes.h "+_
	"serialport.h "+_
	"serialportincludes.h "+_
	"serialportprofile.h "+_
	"serialportprofileincludes.h "+_
	"server.h "+_
	"serverincludes.h "+_
	"serviceentry.h "+_
	"serviceentryincludes.h "+_
	"sharedmemory.h "+_
	"sharedmemoryincludes.h "+_
	"shmfile.h "+_
	"shmfileincludes.h "+_
	"signalhandler.h "+_
	"signalset.h "+_
	"signalclassesincludes.h "+_
	"snoozeincludes.h "+_
	"stringbuffer.h "+_
	"stringbufferincludes.h "+_
	"sysincludes.h "+_
	"unixsocketclient.h "+_
	"unixsocketclientincludes.h "+_
	"unixsocketserver.h "+_
	"unixsocketserverincludes.h "+_
	"bytebuffer.h "+_
	"bytebufferincludes.h "+_
	"xmldom.h "+_
	"xmldomincludes.h "+_
	"xmldomnode.h "+_
	"xmldomnodeincludes.h "+_
	"xmlsax.h "+_
	"xmlsaxincludes.h "+_
	"codetree.h "+_
	"codetreeincludes.h "+_
	"inetsocketutil.h "+_
	"inetsocketutilincludes.h "+_
	"unixsocketutil.h "+_
	"unixsocketutilincludes.h "+_
	"socketserver.h "+_
	"socketserverincludes.h "+_
	"socketclient.h "+_
	"socketclientincludes.h "+_
	"modemutil.h"

' version
RUDIMENTS_VERSION="0.50"

' paths
prefix="C:\""Program Files""\Firstworks"
exec_prefix=prefix
bindir=prefix+"\bin"
includedir=prefix+"\include"
libdir=prefix+"\lib"
mandir=prefix+"\share\man"
datadir=prefix+"\share"

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

' extension
EXE=".exe"

' create file system object
set fso=CreateObject("Scripting.FileSystemObject")


' get top_builddir
top_builddir=fso.GetAbsolutePathName(".")


' determine VC++ version
set WshShell=WScript.CreateObject("WScript.Shell")
set cmd=WshShell.exec("cl")
stdout=cmd.StdOut.ReadAll()
stderr=cmd.StdErr.ReadLine()
parts=split(stderr)
version=""
for i=lbound(parts) to ubound(parts)
	if parts(i)="Version" then
		version=parts(i+1)
	end if
next
parts=split(version,".")
version=parts(0)



' determine config.h template...

' default to VS2008 and up
configwindowsh="include\\rudiments\\private\\config.vs2010.h"

' VS2002 - VS2008
if version<=15 and version>12 then
	configwindowsh="include\\rudiments\\private\\config.vs2005.h"

' VS6 and lower
elseif version<=12 then
	configwindowsh="include\\rudiments\\private\\config.vs5.h"
end if



' determine SDK headers and libs... (FIXME: make this configurable)

' VS2008 and up come with an SDK and are pre-configured to use them
SDKINCLUDES=""
SDKLIBS=""

' VS2002 - VS2005 don't come with an SDK
' assume v6.0A (which comes with VS2008)
if version<=14 and version>12 then
	SDKINCLUDES="/I""C:\\Program Files\\Microsoft SDKs\\Windows\\v6.0A\\Include"""
	SDKLIBS="/LIBPATH:""C:\\Program Files\\Microsoft SDKs\\Windows\\v6.0A\\Lib"""

' VS6 and lower don't come with an SDK and don't support SDK v6.X
' assume that a version that they do support is installed in the
' default location
elseif version<=12 then
	SDKINCLUDES="/I""C:\\Program Files\\Microsoft SDK\\include"""
	SDKLIBS="/LIBPATH:""C:\\Program Files\\Microsoft SDK\\Lib"""
end if



' get 32/64-bit operating system
' FIXME: this needs to determine whether the compiler generates
' 32-bit or 64-bit code rather than the architecture of the OS
USE_32BIT_TIME_T=""
set network=WScript.CreateObject("WScript.Network") 
computername=Trim(network.ComputerName) 
set network=Nothing 
set wmi=GetObject("WINMGMTS:" & "{ImpersonationLevel=Impersonate,AuthenticationLevel=Pkt}!\\" & computername & "\Root\CIMV2") 
set ColSettings=wmi.ExecQuery ("SELECT * FROM Win32_Processor") 
for each processor In ColSettings 
	if processor.AddressWidth=32 then
		USE_32BIT_TIME_T="/D _USE_32BIT_TIME_T"
	end if
next 


' input and output files
infiles=Array(_
	"config.windows.mk",_
	configwindowsh,_
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

	' flags
	content=replace(content,"@_USE_32BIT_TIME_T@",USE_32BIT_TIME_T,1,-1,0)
	content=replace(content,"@SDKINCLUDES@",SDKINCLUDES,1,-1,0)

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
	content=replace(content,"@SDKLIBS@",SDKLIBS,1,-1,0)

	' extension
	content=replace(content,"@EXE@",EXE,1,-1,0)


	' top_builddir
	content=replace(content,"@top_builddir@",top_builddir,1,-1,0)

	' write output file
	set outfile=fso.OpenTextFile(outfiles(i),2,true)
	call outfile.Write(content)
	call outfile.Close()
next
