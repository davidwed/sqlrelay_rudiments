$ del *.exe;*
$ del *.obj;*
$ purge *.*
$ define rudiments DNA_DIR:[FIRSTWO5.firstworks.rudiments.include.rudiments]
$ define rudiments.private DNA_DIR:[FIRSTWO5.firstworks.rudiments.include.rudiments.private]
$ define sys SYS$SYSROOT:[000000.SYSCOMMON.DECC$LIB.REFERENCE.DECC$RTLDEF]
$ define arpa SYS$SYSROOT:[000000.SYSCOMMON.DECC$LIB.REFERENCE.DECC$RTLDEF]
$ define netinet SYS$SYSROOT:[000000.SYSCOMMON.DECC$LIB.REFERENCE.DECC$RTLDEF]
$ cxx /define=(__CRTL_VER_OVERRIDE=70201000,_SOCKADDR_LEN) -
 bytebuffer.cpp, bytestring.cpp, character.cpp, charstring.cpp,-
 commandline.cpp, daemon.cpp, datetime.cpp,-
 dictionary.cpp, directory.cpp, dynamicarray.cpp,-
 environment.cpp, error.cpp, file.cpp, filesystem.cpp, groupentry.cpp,-
 hostentry.cpp, inetclntbench.cpp, inetsocketclient.cpp, inetsvrbench.cpp,-
 kill.cpp, linkedlist.cpp, listener.cpp, listmetrics.cpp,-
 logger.cpp, memorymap.cpp, memorypool.cpp,-
 nonhandoff1.cpp, nonhandoff2.cpp, nonhandoffclient.cpp, nullserver.cpp,-
 oneshotclnt.cpp, oneshotsvr.cpp, parameterstring.cpp, permissions.cpp,-
 process.cpp, protocolentry.cpp, randomnumber.cpp,-
 server.cpp, serviceentry.cpp, signal.cpp, singlylinkedlist.cpp,-
 snooze.cpp, staticarray.cpp, stringbuffer.cpp, sys.cpp, text.cpp,-
 unixsocketclient.cpp, userentry.cpp, xmld.cpp, xmlentities.cpp, xmls.cpp
$ purge *.*
$ cxxlink /exe=bytebuffer.exe bytebuffer.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=bytestring.exe bytestring.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=character.exe character.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=charstring.exe charstring.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=commandline.exe commandline.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=daemon.exe daemon.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=datetime.exe datetime.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=device.exe device.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=dictionary.exe dictionary.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=directory.exe directory.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=dynamicarray.exe dynamicarray.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=environment.exe environment.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=error.exe error.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=file.exe file.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=filesystem.exe filesystem.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=groupentry.exe groupentry.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=hostentry.exe hostentry.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=inetclntbench.exe inetclntbench.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=inetsocketclient.exe inetsocketclient.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=inetsvrbench.exe inetsvrbench.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=kill.exe kill.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=linkedlist.exe linkedlist.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=listener.exe listener.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=listmetrics.exe listmetrics.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=logger.exe logger.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=memorymap.exe memorymap.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=memorypool.exe memorypool.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=nonhandoff1.exe nonhandoff1.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=nonhandoff2.exe nonhandoff2.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=nonhandoffclient.exe nonhandoffclient.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=nullserver.exe nullserver.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=oneshotclnt.exe oneshotclnt.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=oneshotsvr.exe oneshotsvr.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=parameterstring.exe parameterstring.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=permissions.exe permissions.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=process.exe process.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=protocolentry.exe protocolentry.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=randomnumber.exe randomnumber.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=server.exe server.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=serviceentry.exe serviceentry.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=signal.exe signal.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=singlylinkedlist.exe singlylinkedlist.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=snooze.exe snooze.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=staticarray.exe staticarray.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=stringbuffer.exe stringbuffer.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=sys.exe sys.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=text.exe text.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=unixsocketclient.exe unixsocketclient.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=userentry.exe userentry.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=xmld.exe xmld.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=xmlentities.exe xmlentities.obj,-
 [-.src]librudiments.olb/library
$ cxxlink /exe=xmls.exe xmls.obj,-
 [-.src]librudiments.olb/library
