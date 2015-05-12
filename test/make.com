$ del *.exe;*
$ del *.obj;*
$ purge *.*
$ define rudiments DNA_DIR:[FIRSTWO5.firstworks.rudiments.include.rudiments]
$ define rudiments.private DNA_DIR:[FIRSTWO5.firstworks.rudiments.include.rudime
nts.private]
$ define sys SYS$SYSROOT:[000000.SYSCOMMON.DECC$LIB.REFERENCE.DECC$RTLDEF]
$ define arpa SYS$SYSROOT:[000000.SYSCOMMON.DECC$LIB.REFERENCE.DECC$RTLDEF]
$ define netinet SYS$SYSROOT:[000000.SYSCOMMON.DECC$LIB.REFERENCE.DECC$RTLDEF]
$ cxx /define=(__CRTL_VER_OVERRIDE=70201000,_SOCKADDR_LEN) -
 bytebuffer.cpp, bytestring.cpp, character.cpp, charstring.cpp,-
 chat.cpp, commandline.cpp, daemon.cpp, datetime.cpp, device.cpp,-
 dictionary.cpp, directory.cpp, dtd.cpp, dynamicarray.cpp, dynamiclib.cpp,-
 environment.cpp, error.cpp, file.cpp, filesystem.cpp, groupentry.cpp,-
 hostentry.cpp, inetclntbench.cpp, inetsocketclient.cpp, inetsvrbench.cpp,-
 intervaltimer.cpp, kill.cpp, linkedlist.cpp, listener.cpp, listmetrics.cpp,-
 lock1.cpp, lock2.cpp, logger.cpp, memorymap.cpp, memorypool.cpp,-
 nonhandoff1.cpp, nonhandoff2.cpp, nonhandoffclient.cpp, nullserver.cpp,-
 oneshotclnt.cpp, oneshotsvr.cpp, parameterstring.cpp, permissions.cpp,-
 process.cpp, protocolentry.cpp, randomnumber.cpp, semaphoreset1.cpp,-
 semaphoreset2.cpp, server.cpp, serviceentry.cpp, sharedmemory1.cpp,-
 sharedmemory2.cpp, shmfile.cpp, signal.cpp, singlylinkedlist.cpp,-
 snooze.cpp, staticarray.cpp, stringbuffer.cpp, sys.cpp, test.cpp, text.cpp,-
 unixsocketclient.cpp, userentry.cpp, xmld.cpp, xmlentities.cpp, xmls.cpp
$ purge *.*
