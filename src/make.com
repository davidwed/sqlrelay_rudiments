$ del *.obj;*
$ del *.olb;*
$ purge *.*
$ define rudiments DNA_DIR:[FIRSTWO5.firstworks.rudiments.include.rudiments]
$ define rudiments.private DNA_DIR:[FIRSTWO5.firstworks.rudiments.include.rudime
nts.private]
$ define sys SYS$SYSROOT:[000000.SYSCOMMON.DECC$LIB.REFERENCE.DECC$RTLDEF]
$ define arpa SYS$SYSROOT:[000000.SYSCOMMON.DECC$LIB.REFERENCE.DECC$RTLDEF]
$ define netinet SYS$SYSROOT:[000000.SYSCOMMON.DECC$LIB.REFERENCE.DECC$RTLDEF]
$ cxx /define=(__CRTL_VER_OVERRIDE=70320000,_USE_STD_STAT,_SOCKADDR_LEN) -
 winsock.cpp, character.cpp, charstring.cpp, chat.cpp, client.cpp,-
 commandline.cpp, datetime.cpp, device.cpp, directory.cpp, dtd.cpp,-
 dynamiclib.cpp, environment.cpp, error.cpp, file.cpp, filedescriptor.cpp,-
 stdio.cpp, filesystem.cpp, groupentry.cpp, hostentry.cpp,-
 inetsocketclient.cpp, inetsocketserver.cpp, intervaltimer.cpp, listener.cpp,-
 logger.cpp, memorymap.cpp, memorypool.cpp, parameterstring.cpp,-
 userentry.cpp, permissions.cpp, process.cpp, protocolentry.cpp,-
 randomnumber.cpp, bytestring.cpp, rpcentry.cpp,-
 semaphoreset.cpp, server.cpp, serviceentry.cpp, sharedmemory.cpp,-
 shmfile.cpp, signalclasses.cpp, snooze.cpp, stringbuffer.cpp, sys.cpp,-
 unixsocketclient.cpp, unixsocketserver.cpp, bytebuffer.cpp, xmldom.cpp,-
 xmldomnode.cpp, xmlsax.cpp, inetsocketutil.cpp,-
 socketserver.cpp, socketclient.cpp
$ purge *.*
$ library /create /insert librudiments.olb *.obj
