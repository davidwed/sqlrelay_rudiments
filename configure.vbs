' command line arguments
OPTCPPFLAGS="/O2"
DEBUGCPPFLAGS="/MD"
DEBUGLDFLAGS=""
hexversion=""

for i=0 to WScript.Arguments.Count-1

	arg=Wscript.Arguments.Item(i)

	if arg="--enable-small-code" then
		OPTCPPFLAGS="/O1"
	elseif arg="--enable-debug" then
		OPTCPPFLAGS=""
		DEBUGCPPFLAGS="/Zi /MDd /D _DEBUG"
		DEBUGLDFLAGS="/debug"
	elseif mid(arg,1,23)="--with-windows-version=" then
		hexversion=mid(arg,24)
	end if
next


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
	"dynamiclib.cpp "+_
	"environment.cpp "+_
	"error.cpp "+_
	"file.cpp "+_
	"filedescriptor.cpp "+_
	"gss.cpp "+_
	"tls.cpp "+_
	"stdio.cpp "+_
	"filesystem.cpp "+_
	"groupentry.cpp "+_
	"hostentry.cpp "+_
	"inetsocketclient.cpp "+_
	"inetsocketserver.cpp "+_
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
	"semaphoreset.cpp "+_
	"server.cpp "+_
	"serviceentry.cpp "+_
	"sharedmemory.cpp "+_
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
	"xmldomevents.cpp "+_
	"codetree.cpp "+_
	"compiler.cpp "+_
	"inetsocketutil.cpp "+_
	"unixsocketutil.cpp "+_
	"socketserver.cpp "+_
	"socketclient.cpp "+_
	"url.cpp "+_
	"sha1.cpp "+_
	"md5.cpp "+_
	"prompt.cpp"

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
	"dynamiclib.obj "+_
	"environment.obj "+_
	"error.obj "+_
	"file.obj "+_
	"filedescriptor.obj "+_
	"gss.obj "+_
	"tls.obj "+_
	"stdio.obj "+_
	"filesystem.obj "+_
	"groupentry.obj "+_
	"hostentry.obj "+_
	"inetsocketclient.obj "+_
	"inetsocketserver.obj "+_
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
	"semaphoreset.obj "+_
	"server.obj "+_
	"serviceentry.obj "+_
	"sharedmemory.obj "+_
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
	"xmldomevents.obj "+_
	"codetree.obj "+_
	"compiler.obj "+_
	"inetsocketutil.obj "+_
	"unixsocketutil.obj "+_
	"socketserver.obj "+_
	"socketclient.obj "+_
	"url.obj "+_
	"sha1.obj "+_
	"md5.obj "+_
	"prompt.obj"

headers0="resultcodes.h "+_
	"debugprint.h "+_
	"linkedlist.h "+_
	"singlylinkedlist.h "+_
	"avltree.h "+_
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
	"dynamiclib.h "+_
	"environment.h "+_
	"error.h "+_
	"file.h "
headers1="filedescriptor.h "+_
	"stdio.h "+_
	"filesystem.h "+_
	"groupentry.h "+_
	"hostentry.h "+_
	"inetsocketclient.h "+_
	"inetsocketserver.h "+_
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
	"process.h "
headers2="protocolentry.h "+_
	"randomnumber.h "+_
	"bytestring.h "+_
	"regularexpression.h "+_
	"semaphoreset.h "+_
	"serialport.h "+_
	"serialportprofile.h "+_
	"server.h "+_
	"serviceentry.h "+_
	"sharedmemory.h "+_
	"signalclasses.h "+_
	"snooze.h "+_
	"stringbuffer.h "+_
	"sys.h "+_
	"unixsocketclient.h "+_
	"unixsocketserver.h "+_
	"bytebuffer.h "
headers3="xmldom.h "+_
	"xmldomnode.h "+_
	"xmlsax.h "+_
	"xmldomevents.h "+_
	"codetree.h "+_
	"compiler.h "+_
	"inetsocketutil.h "+_
	"unixsocketutil.h "+_
	"socketserver.h "+_
	"socketclient.h "+_
	"url.h "+_
	"gss.h "+_
	"tls.h "+_
	"security.h "+_
	"hash.h "+_
	"sha1.h "+_
	"md5.h "+_
	"prompt.h"

privateheaders0="config.h "+_
	"debugprint.h "+_
	"rudimentsinlines.h "+_
	"inttypes.h "+_
	"new.h "+_
	"winsock.h "+_
	"dll.h "+_
	"linkedlist.h "+_
	"linkedlistincludes.h "+_
	"linkedlistinlines.h "+_
	"linkedlistnode.h "+_
	"nodeinlines.h "+_
	"avltree.h "+_
	"avltreeincludes.h "+_
	"avltreeinlines.h "+_
	"avltreenode.h "+_
	"singlylinkedlist.h "+_
	"singlylinkedlistincludes.h "+_
	"singlylinkedlistinlines.h "+_
	"singlylinkedlistnode.h "+_
	"dictionary.h "+_
	"dictionaryincludes.h "+_
	"dictionaryinlines.h "+_
	"dictionarynode.h "
privateheaders1="dynamicarray.h "+_
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
	"deviceincludes.h "
privateheaders2="directory.h "+_
	"directoryincludes.h "+_
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
	"hostentryincludes.h "
privateheaders3="inetsocketclient.h "+_
	"inetsocketclientincludes.h "+_
	"inetsocketserver.h "+_
	"inetsocketserverincludes.h "+_
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
	"memorypoolincludes.h "
privateheaders4="modemclient.h "+_
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
	"randomnumberincludes.h "
privateheaders5="bytestringincludes.h "+_
	"regularexpression.h "+_
	"regularexpressionincludes.h "+_
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
	"sharedmemoryincludes.h "
privateheaders6="signalhandler.h "+_
	"signalset.h "+_
	"signalclassesincludes.h "+_
	"snooze.h "+_
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
	"xmldomevents.h "+_
	"xmldomeventsincludes.h"
privateheaders7="codetree.h "+_
	"codetreeincludes.h "+_
	"codetreegrammar.h "+_
	"compiler.h "+_
	"compilerincludes.h "+_
	"inetsocketutil.h "+_
	"inetsocketutilincludes.h "+_
	"unixsocketutil.h "+_
	"unixsocketutilincludes.h "+_
	"socketserver.h "+_
	"socketserverincludes.h "+_
	"socketclient.h "+_
	"socketclientincludes.h "+_
	"modemutil.h "+_
	"url.h "+_
	"urlincludes.h "+_
	"gss.h "+_
	"gssincludes.h "+_
	"gssmechanism.h "+_
	"gsscredentials.h "+_
	"gsscontext.h "+_
	"tls.h "+_
	"tlsincludes.h "+_
	"tlscontext.h "+_
	"tlscertificate.h "+_
	"hashincludes.h "+_
	"sha1.h "+_
	"sha1includes.h "+_
	"md5.h "+_
	"md5includes.h "+_
	"prompt.h "+_
	"promptincludes.h"

' version
RUDIMENTS_VERSION="1.0.6"

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
SERVERPEM=""
GSSINCLUDES=""
GSSLIBS="secur32.lib crypt32.lib" 
PCRELIBS=""
'LIBCURLINCLUDES="/I""C:\Program Files\cURL\include"""
'LIBCURLLIBS="/LIBPATH:""C:\Program Files\cURL\lib"" libcurl.lib"
LIBCURLINCLUDES=""
LIBCURLLIBS=""
NANOSLEEPLIB=""
CLOCKNANOSLEEPLIB=""
CRYPTLIB=""
INETATONLIB=""
NETAPI32LIB="ws2_32.lib netapi32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib uuid.lib dbghelp.lib psapi.lib"
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


' determine VC++ version and architecture
set WshShell=WScript.CreateObject("WScript.Shell")
set cmd=WshShell.exec("cl")
stdout=cmd.StdOut.ReadAll()
stderr=cmd.StdErr.ReadLine()
parts=split(stderr)
arch=parts(ubound(parts))
if arch="80x86" then
	arch="x86"
end if
version=""
for i=lbound(parts) to ubound(parts)
	if parts(i)="Version" then
		version=parts(i+1)
	end if
next
parts=split(version,".")
version=parts(0)

' determine VC++ architecture
USE_32BIT_TIME_T=""
if arch="x86" then
	USE_32BIT_TIME_T="/D _USE_32BIT_TIME_T"
end if



' determine OS Version number
if len(hexversion)=0 then
	set cmd=WshShell.exec("%comspec% /c ver")
	stdout=cmd.StdOut.ReadAll()
	stderr=cmd.StdErr.ReadLine()
	if instr(stdout,"Windows NT Version 4.0")>0 then
		hexversion="0x0400"
	else
		parts0=split(stdout,"[")
		parts1=split(parts0(1)," ")
		parts2=split(parts1(1),"]")
		parts3=split(parts2(0),".")
		if parts3(1)="00" then
			parts3(1)="0"
		end if
		hexversion="0x0"&parts3(0)&"0"&parts3(1)
	end if
end if

' in general, we need to set WIN32WINNT to the hexversion
WINVER=""
WIN32WINDOWS=""
WIN32WINNT=hexversion

' but, for OS'es older than WinXP we have to do some special things...

' for Win2k and WinNT4, set WINVER also
if hexversion="0x0500" or hexversion="0x0400" then
	WINVER=hexversion

' for WinME, set WIN32WINDOWS and unset WIN32WINNT
elseif hexversion="0x0490" then
	WIN32WINDOWS=hexversion
	WIN32WINNT=""

' for Win98, set WIN32WINDOWS and WINVER and unset WIN32WINNT
elseif hexversion="0x0410" then
	WIN32WINDOWS=hexversion
	WINVER=hexversion
	WIN32WINNT=""

' for Win95, set WINVER and unset WIN32WINNT
elseif hexversion="0x0400" then
	WINVER=hexversion
	WIN32WINNT=""

' FIXME: not sure about WinNT3X, Win3X or below
end if

' add /D and macro name
if WINVER<>"" then
	WINVER="/D WINVER="&WINVER
end if
if WIN32WINDOWS<>"" then
	WIN32WINDOWS="/D _WIN32_WINDOWS="&WIN32WINDOWS
end if
if WIN32WINNT<>"" then
	WIN32WINNT="/D _WIN32_WINNT="&WIN32WINNT
end if



' determine config.h template...

' default to VS2015
configwindowsh="include\\rudiments\\private\\config_vs2015.h"

' VS2013
if version=18 then
	configwindowsh="include\\rudiments\\private\\config_vs2013.h"

' VS2010 and VS2012
elseif version<=17 and version>15 then
	configwindowsh="include\\rudiments\\private\\config_vs2010.h"

' VS2005 and VS2008
elseif version<=15 and version>13 then
	configwindowsh="include\\rudiments\\private\\config_vs2005.h"

' VC6, VS2002 and VS2003 (.NET)
elseif version=13 or version=12 then
	configwindowsh="include\\rudiments\\private\\config_vs6.h"

' VS5 and lower
elseif version<=11 then
	configwindowsh="include\\rudiments\\private\\config_vs5.h"
end if



' determine SDK headers and libs... (FIXME: make this configurable)

' VS2002, VS2003 and VS2008 and up come with a platform SDK
SDKINCLUDES=""
SDKLIBS=""

' VS2005 doesn't come with an SDK and there are several that are compatible
if version=14 then

	' older SDK's have various issues

	' 5.2.3700.0 - Microsoft Platform SDK February 2003
	'SDKINCLUDES="/I""C:\Program Files\Microsoft SDK\include"""
	'SDKLIBS="/LIBPATH:""C:\Program Files\Microsoft SDK\Lib"""

	' 5.2.3790.1830.15 - Windows Server 2003 SP1 Platform SDK
	SDKINCLUDES="/I""C:\Program Files\Microsoft Platform SDK\Include"""
	SDKLIBS="/LIBPATH:""C:\Program Files\Microsoft Platform SDK\Lib"""

	' 5.2.3790.2075.51 - Windows Server 2003 R2 Platform SDK
	'SDKINCLUDES="/I""C:\Program Files\Microsoft Platform SDK for Windows Server 2003 R2\Include"""
	'SDKLIBS="/LIBPATH:""C:\Program Files\Microsoft Platform SDK for Windows Server 2003 R2\Lib"""

	' 6.0A (comes with VC2008)
	'SDKINCLUDES="/I""C:\Program Files\Microsoft SDKs\Windows\v6.0A\Include"""
	'SDKLIBS="/LIBPATH:""C:\Program Files\Microsoft SDKs\Windows\v6.0A\Lib"""

	' not sure about newer SDK's

' VS6 doesn't come with a platform SDK
elseif version=12 then

	' older SDK's might work too

	' 5.2.3700.0 - Microsoft Platform SDK February 2003
	SDKINCLUDES="/I""C:\Program Files\Microsoft SDK\include"""
	SDKLIBS="/LIBPATH:""C:\Program Files\Microsoft SDK\Lib"""

	' not sure about newer SDK's

' VS5 and lower don't come with a platform SDK
elseif version<=11 then

	' older SDK's might work too

	' 5.1.2600.2180 - Microsoft Platform SDK for Windows XP SP2
	' (this doesn't actually work)
	'SDKINCLUDES="/I""C:\Program Files\Microsoft Platform SDK for Windows XP SP2\Include"""
	'SDKLIBS="/LIBPATH:""C:\Program Files\Microsoft Platform SDK for Windows XP SP2\Lib"""

	' newer SDK's give link errors

end if



' input and output files
infiles=Array(_
	"config_windows.mk",_
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
	content=replace(content,"@HEADERS0@",headers0,1,-1,0)
	content=replace(content,"@HEADERS1@",headers1,1,-1,0)
	content=replace(content,"@HEADERS2@",headers2,1,-1,0)
	content=replace(content,"@HEADERS3@",headers3,1,-1,0)
	content=replace(content,"@PRIVATEHEADERS0@",privateheaders0,1,-1,0)
	content=replace(content,"@PRIVATEHEADERS1@",privateheaders1,1,-1,0)
	content=replace(content,"@PRIVATEHEADERS2@",privateheaders2,1,-1,0)
	content=replace(content,"@PRIVATEHEADERS3@",privateheaders3,1,-1,0)
	content=replace(content,"@PRIVATEHEADERS4@",privateheaders4,1,-1,0)
	content=replace(content,"@PRIVATEHEADERS5@",privateheaders5,1,-1,0)
	content=replace(content,"@PRIVATEHEADERS6@",privateheaders6,1,-1,0)
	content=replace(content,"@PRIVATEHEADERS7@",privateheaders7,1,-1,0)

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
	content=replace(content,"@OPTCPPFLAGS@",OPTCPPFLAGS,1,-1,0)
	content=replace(content,"@DEBUGCPPFLAGS@",DEBUGCPPFLAGS,1,-1,0)
	content=replace(content,"@DEBUGLDFLAGS@",DEBUGLDFLAGS,1,-1,0)
	content=replace(content,"@_USE_32BIT_TIME_T@",USE_32BIT_TIME_T,1,-1,0)
	content=replace(content,"@SDKINCLUDES@",SDKINCLUDES,1,-1,0)
	content=replace(content,"@WINVER@",WINVER,1,-1,0)
	content=replace(content,"@WIN32WINDOWS@",WIN32WINDOWS,1,-1,0)
	content=replace(content,"@WIN32WINNT@",WIN32WINNT,1,-1,0)

	' libraries
	content=replace(content,"@SOCKETLIBS@",SOCKETLIBS,1,-1,0)
	content=replace(content,"@PTHREADLIB@",PTHREADLIB,1,-1,0)
	content=replace(content,"@SSLLIBS@",SSLLIBS,1,-1,0)
	content=replace(content,"@SERVERPEM@",SERVERPEM,1,-1,0)
	content=replace(content,"@GSSINCLUDES@",GSSINCLUDES,1,-1,0)
	content=replace(content,"@GSSLIBS@",GSSLIBS,1,-1,0)
	content=replace(content,"@PCRELIBS@",PCRELIBS,1,-1,0)
	content=replace(content,"@LIBCURLINCLUDES@",LIBCURLINCLUDES,1,-1,0)
	content=replace(content,"@LIBCURLLIBS@",LIBCURLLIBS,1,-1,0)
	content=replace(content,"@NANOSLEEPLIB@",NANOSLEEPLIB,1,-1,0)
	content=replace(content,"@CLOCKNANOSLEEPLIB@",CLOCKNANOSLEEPLIB,1,-1,0)
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
