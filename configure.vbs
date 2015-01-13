srcs="character.cpp charstring.cpp chat.cpp client.cpp commandline.cpp crypt.cpp datetime.cpp device.cpp directory.cpp dtd.cpp dynamiclib.cpp environment.cpp error.cpp file.cpp filedescriptor.cpp stdio.cpp filesystem.cpp groupentry.cpp hostentry.cpp inetsocketclient.cpp inetsocketserver.cpp intervaltimer.cpp listener.cpp logger.cpp memorymap.cpp memorypool.cpp modemclient.cpp modemserver.cpp thread.cpp threadmutex.cpp parameterstring.cpp userentry.cpp permissions.cpp process.cpp protocolentry.cpp randomnumber.cpp bytestring.cpp regularexpression.cpp rpcentry.cpp semaphoreset.cpp serialport.cpp serialportprofile.cpp server.cpp serviceentry.cpp sharedmemory.cpp shmfile.cpp signalclasses.cpp snooze.cpp stringbuffer.cpp sys.cpp unixsocketclient.cpp unixsocketserver.cpp bytebuffer.cpp xmldom.cpp xmldomnode.cpp xmlsax.cpp codetree.cpp inetsocketutil.cpp unixsocketutil.cpp socketserver.cpp socketclient.cpp modemutil.cpp"
lobjs="character.lo charstring.lo chat.lo client.lo commandline.lo crypt.lo datetime.lo device.lo directory.lo dtd.lo dynamiclib.lo environment.lo error.lo file.lo filedescriptor.lo stdio.lo filesystem.lo groupentry.lo hostentry.lo inetsocketclient.lo inetsocketserver.lo intervaltimer.lo listener.lo logger.lo memorymap.lo memorypool.lo modemclient.lo modemserver.lo thread.lo threadmutex.lo parameterstring.lo userentry.lo permissions.lo process.lo protocolentry.lo randomnumber.lo bytestring.lo regularexpression.lo rpcentry.lo semaphoreset.lo serialport.lo serialportprofile.lo server.lo serviceentry.lo sharedmemory.lo shmfile.lo signalclasses.lo snooze.lo stringbuffer.lo sys.lo unixsocketclient.lo unixsocketserver.lo bytebuffer.lo xmldom.lo xmldomnode.lo xmlsax.lo codetree.lo inetsocketutil.lo unixsocketutil.lo socketserver.lo socketclient.lo modemutil.lo"

set fso=CreateObject("Scripting.FileSystemObject")
call fso.CopyFile("config.windows.mk","config.mk")
call fso.CopyFile("include\\rudiments\\private\\config.windows.h","include\\rudiments\\private\\config.h")

set makefilein=fso.OpenTextFile("src\\Makefile.in",1)
content=makefilein.ReadAll()
content=replace(content,"@SRCS@",srcs,1,-1,0)
content=replace(content,"@LOBJS@",objs,1,-1,0)
call makefilein.Close()

set makefileout=fso.OpenTextFile("src\\Makefile",2,true)
call makefileout.Write(content)
call makefileout.Close()
