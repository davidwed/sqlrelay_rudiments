$ del *.olb;*
$ del *.obj;*
$ purge *.*
$ define rudiments DNA_DIR:[FIRSTWO5.firstworks.rudiments.include.rudiments]
$ define rudiments.private DNA_DIR:[FIRSTWO5.firstworks.rudiments.include.rudime
nts.private]
$ define sys SYS$SYSROOT:[000000.SYSCOMMON.DECC$LIB.REFERENCE.DECC$RTLDEF]
$ define arpa SYS$SYSROOT:[000000.SYSCOMMON.DECC$LIB.REFERENCE.DECC$RTLDEF]
$ define netinet SYS$SYSROOT:[000000.SYSCOMMON.DECC$LIB.REFERENCE.DECC$RTLDEF]
$ cxx /define=(__CRTL_VER_OVERRIDE=70320000,_USE_STD_STAT,_SOCKADDR_LEN) -
 winsock.cpp, character.cpp, charstring.cpp, client.cpp,-
 commandline.cpp, datetime.cpp, device.cpp, directory.cpp,-
 environment.cpp, error.cpp, file.cpp, filedescriptor.cpp,-
 stdio.cpp, filesystem.cpp, groupentry.cpp, hostentry.cpp,-
 inetsocketclient.cpp, inetsocketserver.cpp, listener.cpp,-
 logger.cpp, memorymap.cpp, memorypool.cpp, threadmutex.cpp,-
 parameterstring.cpp, userentry.cpp, permissions.cpp, process.cpp,-
 protocolentry.cpp, randomnumber.cpp, bytestring.cpp,-
 server.cpp, serviceentry.cpp,-
 signalclasses.cpp, snooze.cpp, stringbuffer.cpp, sys.cpp,-
 unixsocketclient.cpp, unixsocketserver.cpp, bytebuffer.cpp, xmldom.cpp,-
 xmldomnode.cpp, xmlsax.cpp, inetsocketutil.cpp, unixsocketutil.cpp,-
 socketserver.cpp, socketclient.cpp
$ purge *.*
$ library /create /insert librudiments.olb *.obj
