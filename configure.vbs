srcs="winsock.cpp character.cpp charstring.cpp chat.cpp client.cpp commandline.cpp crypt.cpp datetime.cpp device.cpp directory.cpp dtd.cpp dynamiclib.cpp environment.cpp error.cpp file.cpp filedescriptor.cpp stdio.cpp filesystem.cpp groupentry.cpp hostentry.cpp inetsocketclient.cpp inetsocketserver.cpp intervaltimer.cpp listener.cpp logger.cpp memorymap.cpp memorypool.cpp thread.cpp threadmutex.cpp parameterstring.cpp userentry.cpp permissions.cpp process.cpp protocolentry.cpp randomnumber.cpp bytestring.cpp regex.cpp regularexpression.cpp rpcentry.cpp semaphoreset.cpp server.cpp serviceentry.cpp sharedmemory.cpp shmfile.cpp signalclasses.cpp snooze.cpp stringbuffer.cpp sys.cpp unixsocketclient.cpp unixsocketserver.cpp bytebuffer.cpp xmldom.cpp xmldomnode.cpp xmlsax.cpp codetree.cpp inetsocketutil.cpp unixsocketutil.cpp socketserver.cpp socketclient.cpp"
lobjs="winsock.obj character.obj charstring.obj chat.obj client.obj commandline.obj crypt.obj datetime.obj device.obj directory.obj dtd.obj dynamiclib.obj environment.obj error.obj file.obj filedescriptor.obj stdio.obj filesystem.obj groupentry.obj hostentry.obj inetsocketclient.obj inetsocketserver.obj intervaltimer.obj listener.obj logger.obj memorymap.obj memorypool.obj thread.obj threadmutex.obj parameterstring.obj userentry.obj permissions.obj process.obj protocolentry.obj randomnumber.obj bytestring.obj regex.obj regularexpression.obj rpcentry.obj semaphoreset.obj server.obj serviceentry.obj sharedmemory.obj shmfile.obj signalclasses.obj snooze.obj stringbuffer.obj sys.obj unixsocketclient.obj unixsocketserver.obj bytebuffer.obj xmldom.obj xmldomnode.obj xmlsax.obj codetree.obj inetsocketutil.obj unixsocketutil.obj socketserver.obj socketclient.obj"

set fso=CreateObject("Scripting.FileSystemObject")
call fso.CopyFile("config.windows.mk","config.mk")
call fso.CopyFile("include\\rudiments\\private\\config.windows.h","include\\rudiments\\private\\config.h")

set makefilein=fso.OpenTextFile("src\\Makefile.in",1)
content=makefilein.ReadAll()
content=replace(content,"@SRCS@",srcs,1,-1,0)
content=replace(content,"@LOBJS@",lobjs,1,-1,0)
call makefilein.Close()

set makefileout=fso.OpenTextFile("src\\Makefile",2,true)
call makefileout.Write(content)
call makefileout.Close()
