top_builddir = ..

include ../config.mk
include ../features.mk

HEADERS = resultcodes.h
PRIVATEHEADERS = config.h rudimentsinlines.h

ifeq ($(INCLUDE_CLIENT),1)
	HEADERS += client.h
	PRIVATEHEADERS += client.h clientincludes.h
endif
ifeq ($(INCLUDE_COMMANDLINE),1)
	HEADERS += commandline.h
	PRIVATEHEADERS += commandline.h commandlineincludes.h
endif
ifeq ($(INCLUDE_PARAMETERSTRING),1)
	HEADERS += parameterstring.h
	PRIVATEHEADERS += parameterstring.h parameterstringincludes.h
endif
ifeq ($(INCLUDE_DAEMONPROCESS),1)
	HEADERS += daemonprocess.h
	PRIVATEHEADERS += daemonprocess.h daemonprocessincludes.h
endif
ifeq ($(INCLUDE_DATETIME),1)
	HEADERS += datetime.h
	PRIVATEHEADERS += datetime.h datetimeincludes.h
endif
ifeq ($(INCLUDE_TIMEZONEFILE),1)
	HEADERS += timezonefile.h
	PRIVATEHEADERS += timezonefile.h
endif
#ifeq ($(INCLUDE_DICTIONARY),1)
	HEADERS += dictionary.h
	PRIVATEHEADERS += dictionary.h dictionaryinlines.h \
				dictionarynode.h dictionarynodeinlines.h \
				dictionaryincludes.h
#endif
ifeq ($(INCLUDE_DTD),1)
	HEADERS += dtd.h
	PRIVATEHEADERS += dtd.h dtdincludes.h
endif
ifeq ($(INCLUDE_ENVIRONMENT),1)
	HEADERS += environment.h
	PRIVATEHEADERS += environment.h environmentincludes.h
endif
ifeq ($(INCLUDE_ERRORHANDLER),1)
	HEADERS += errorhandler.h
	PRIVATEHEADERS += errorhandler.h errorhandlerincludes.h
endif
ifeq ($(INCLUDE_FILEDESCRIPTOR),1)
	HEADERS += filedescriptor.h
	PRIVATEHEADERS += filedescriptor.h filedescriptorincludes.h
endif
ifeq ($(INCLUDE_FILE),1)
	HEADERS += file.h
	PRIVATEHEADERS += file.h fileincludes.h
endif
ifeq ($(INCLUDE_DEVICE),1)
	HEADERS += device.h
	PRIVATEHEADERS += deviceincludes.h
endif
ifeq ($(INCLUDE_FILESYSTEM),1)
	HEADERS += filesystem.h
	PRIVATEHEADERS += filesystem.h filesystemincludes.h
endif
ifeq ($(INCLUDE_DIRECTORY),1)
	HEADERS += directory.h
	PRIVATEHEADERS += directory.h directoryincludes.h
endif
ifeq ($(INCLUDE_GROUPENTRY),1)
	HEADERS += groupentry.h
	PRIVATEHEADERS += groupentry.h groupentryincludes.h
endif
ifeq ($(INCLUDE_HOSTENTRY),1)
	HEADERS += hostentry.h
	PRIVATEHEADERS += hostentry.h hostentryincludes.h
endif
ifeq ($(INCLUDE_INETCLIENT),1)
	HEADERS += inetclientsocket.h
	PRIVATEHEADERS += inetclientsocket.h inetclientsocketincludes.h
endif
ifeq ($(INCLUDE_INETSERVER),1)
	HEADERS += inetserversocket.h
	PRIVATEHEADERS += inetserversocketincludes.h
endif
ifeq ($(INCLUDE_LISTENER),1)
	HEADERS += listener.h
	PRIVATEHEADERS += listener.h listenerincludes.h
endif
#ifeq ($(INCLUDE_LINKEDLIST),1)
	HEADERS += linkedlist.h
	PRIVATEHEADERS += linkedlist.h linkedlistinlines.h linkedlistnode.h \
				linkedlistnodeinlines.h linkedlistutil.h \
				linkedlistutilinlines.h linkedlistincludes.h
#endif
ifeq ($(INCLUDE_LOGGER),1)
	HEADERS += logger.h
	PRIVATEHEADERS += filedestination.h \
				logdestination.h logger.h \
				stderrdestination.h stdoutdestination.h \
				syslogdestination.h loggerincludes.h
endif
ifeq ($(INCLUDE_MEMORYPOOL),1)
	HEADERS += memorypool.h
	PRIVATEHEADERS += memorypool.h memorypoolnode.h memorypoolincludes.h
endif
ifeq ($(INCLUDE_PASSWDENTRY),1)
	HEADERS += passwdentry.h
	PRIVATEHEADERS += passwdentry.h passwdentryincludes.h
endif
ifeq ($(INCLUDE_PERMISSIONS),1)
	HEADERS += permissions.h
	PRIVATEHEADERS += permissionsincludes.h
endif
ifeq ($(INCLUDE_PROTOCOLENTRY),1)
	HEADERS += protocolentry.h
	PRIVATEHEADERS += protocolentry.h protocolentryincludes.h
endif
ifeq ($(INCLUDE_RANDOMNUMBER),1)
	HEADERS += randomnumber.h
	PRIVATEHEADERS += randomnumber.h randomnumberincludes.h
endif
ifeq ($(INCLUDE_CONTAINER),1)
	HEADERS += container.h
	PRIVATEHEADERS += container.h containerincludes.h
endif
ifeq ($(INCLUDE_REGULAREXPRESSION),1)
	HEADERS += regularexpression.h
	PRIVATEHEADERS += regularexpression.h regularexpressionincludes.h
endif
ifeq ($(INCLUDE_RPCENTRY),1)
	HEADERS += rpcentry.h
	PRIVATEHEADERS += rpcentry.h rpcentryincludes.h
endif
ifeq ($(INCLUDE_SEMAPHORESET),1)
	HEADERS += semaphoreset.h
	PRIVATEHEADERS += semaphoreset.h semaphoresetincludes.h
endif
ifeq ($(INCLUDE_SERVER),1)
	HEADERS += server.h
	PRIVATEHEADERS += serverincludes.h
endif
ifeq ($(INCLUDE_SERVICEENTRY),1)
	HEADERS += serviceentry.h
	PRIVATEHEADERS += serviceentry.h serviceentryincludes.h
endif
ifeq ($(INCLUDE_SHADOWENTRY),1)
	HEADERS += shadowentry.h
	PRIVATEHEADERS += shadowentry.h shadowentryincludes.h
endif
ifeq ($(INCLUDE_SHAREDMEMORY),1)
	HEADERS += sharedmemory.h
	PRIVATEHEADERS += sharedmemory.h sharedmemoryincludes.h
endif
ifeq ($(INCLUDE_SIGNALCLASSES),1)
	HEADERS += signalclasses.h
	PRIVATEHEADERS += signalhandler.h signalset.h signalclassesincludes.h
endif
ifeq ($(INCLUDE_STRINGBUFFER),1)
	HEADERS += stringbuffer.h
	PRIVATEHEADERS += stringbuffer.h stringbufferincludes.h
endif
ifeq ($(INCLUDE_CHARSTRING),1)
	HEADERS += charstring.h
	PRIVATEHEADERS += charstring.h charstringincludes.h
endif
ifeq ($(INCLUDE_CHARACTER),1)
	HEADERS += character.h
	PRIVATEHEADERS += characterincludes.h
endif
ifeq ($(INCLUDE_UNIXCLIENT),1)
	HEADERS += unixclientsocket.h
	PRIVATEHEADERS += unixclientsocket.h unixclientsocketincludes.h
endif
ifeq ($(INCLUDE_MODEMCLIENT),1)
	HEADERS += modemclient.h
	PRIVATEHEADERS += modemclient.h modemclientincludes.h
endif
ifeq ($(INCLUDE_MODEMSERVER),1)
	HEADERS += modemserver.h
	PRIVATEHEADERS += modemserver.h modemserverincludes.h
endif
ifeq ($(INCLUDE_UNIXSERVER),1)
	HEADERS += unixserversocket.h
	PRIVATEHEADERS += unixserversocket.h unixserversocketincludes.h
endif
ifeq ($(INCLUDE_VARIABLEBUFFER),1)
	HEADERS += variablebuffer.h
	PRIVATEHEADERS += variablebuffer.h variablebufferincludes.h
endif
ifeq ($(INCLUDE_XMLDOM),1)
	HEADERS += xmldom.h xmldomnode.h
	PRIVATEHEADERS += xmldom.h xmldomnode.h \
					xmldomincludes.h xmldomnodeincludes.h
endif
ifeq ($(INCLUDE_XMLSAX),1)
	HEADERS += xmlsax.h
	PRIVATEHEADERS += xmlsax.h xmlsaxincludes.h
endif
ifeq ($(INCLUDE_INTERVALTIMER),1)
	HEADERS += intervaltimer.h
	PRIVATEHEADERS += intervaltimer.h intervaltimerincludes.h
endif
ifeq ($(INCLUDE_DYNAMICLIB),1)
	HEADERS += dynamiclib.h
	PRIVATEHEADERS += dynamiclib.h dynamiclibincludes.h
endif
ifeq ($(INCLUDE_CRYPT),1)
	HEADERS += crypt.h
	PRIVATEHEADERS += crypt.h cryptincludes.h
endif
ifeq ($(INCLUDE_MATH),1)
	HEADERS += math.h
	PRIVATEHEADERS += mathincludes.h mathinlines.h
endif
ifeq ($(INCLUDE_RAWBUFFER),1)
	HEADERS += rawbuffer.h
	PRIVATEHEADERS += rawbufferincludes.h
endif
ifeq ($(INCLUDE_MEMORYMAP),1)
	HEADERS += memorymap.h
	PRIVATEHEADERS += memorymapincludes.h memorymap.h
endif
ifeq ($(INCLUDE_MUTEX),1)
	HEADERS += mutex.h
	PRIVATEHEADERS += mutexincludes.h mutex.h
endif
ifeq ($(INCLUDE_SERIALPORTPROFILE),1)
	HEADERS += serialportprofile.h
	PRIVATEHEADERS += serialportprofileincludes.h \
				serialportprofile.h
endif
ifeq ($(INCLUDE_SERIALPORT),1)
	HEADERS += serialport.h
	PRIVATEHEADERS += serialportincludes.h
endif
ifeq ($(INCLUDE_CHAT),1)
	HEADERS += chat.h
	PRIVATEHEADERS += chatincludes.h
endif

# if either modemserver or modemclient are defined, include modemutil.h
ifeq ($(INCLUDE_MODEMSERVER),1)
	HEADERS += modemutil.h
	PRIVATEHEADERS += modemutil.h modemutilincludes.h
else
	ifeq ($(INCLUDE_MODEMCLIENT),1)
		HEADERS += modemutil.h
		PRIVATEHEADERS += modemutil.h modemutilincludes.h
	endif
endif

# if either inetserver or inetclient are defined, include inetsocketutil.h
ifeq ($(INCLUDE_INETSERVER),1)
	HEADERS += inetsocketutil.h
	PRIVATEHEADERS += inetsocketutil.h inetsocketutilincludes.h
else
	ifeq ($(INCLUDE_INETCLIENT),1)
		HEADERS += inetsocketutil.h
		PRIVATEHEADERS += inetsocketutil.h inetsocketutilincludes.h
	endif
endif

# if either unixserver or unixclient are defined, include unixsocketutil.h
ifeq ($(INCLUDE_UNIXSERVER),1)
	HEADERS += unixsocketutil.h
	PRIVATEHEADERS += unixsocketutil.h unixsocketutilincludes.h
else
	ifeq ($(INCLUDE_UNIXCLIENT),1)
		HEADERS += unixsocketutil.h
		PRIVATEHEADERS += unixsocketutil.h unixsocketutilincludes.h
	endif
endif

# if inet/unix server are defined, include serversocket.h
ifeq ($(INCLUDE_INETSERVER),1)
	HEADERS += serversocket.h
	PRIVATEHEADERS += serversocket.h serversocketincludes.h
else
	ifeq ($(INCLUDE_UNIXSERVER),1)
		HEADERS += serversocket.h
		PRIVATEHEADERS += serversocket.h serversocketincludes.h
	endif
endif

# if inet/unix client are defined, include clientsocket.h
ifeq ($(INCLUDE_INETCLIENT),1)
	HEADERS += clientsocket.h
	PRIVATEHEADERS += clientsocket.h clientsocketincludes.h
else
	ifeq ($(INCLUDE_UNIXCLIENT),1)
		HEADERS += clientsocket.h
		PRIVATEHEADERS += clientsocket.h clientsocketincludes.h
	endif
endif

# if any server and client are defined, include clientserverfactory
ifeq ($(INCLUDE_INETSERVER),1)
	ifeq ($(INCLUDE_INETCLIENT),1)
		HEADERS += clientserverfactory.h
		PRIVATEHEADERS += clientserverfactoryincludes.h
	else
		ifeq ($(INCLUDE_UNIXCLIENT),1)
			HEADERS += clientserverfactory.h
			PRIVATEHEADERS += clientserverfactoryincludes.h
		endif
	endif
else
	ifeq ($(INCLUDE_UNIXSERVER),1)
		ifeq ($(INCLUDE_INETCLIENT),1)
			HEADERS += clientserverfactory.h
			PRIVATEHEADERS += clientserverfactoryincludes.h
		else
			ifeq ($(INCLUDE_UNIXCLIENT),1)
				HEADERS += clientserverfactory.h
				PRIVATEHEADERS += clientserverfactoryincludes.h
			endif
		endif
	endif
endif


install:
	$(MKINSTALLDIRS) $(includedir)/rudiments
	$(MKINSTALLDIRS) $(includedir)/rudiments/private
	for i in $(HEADERS); do $(INSTALL) -m 644 rudiments/$$i $(includedir)/rudiments; done
	for i in $(PRIVATEHEADERS); do $(INSTALL) -m 644 rudiments/private/$$i $(includedir)/rudiments/private; done

uninstall:
	$(RM) $(includedir)/client.h
	$(RM) $(includedir)/clientsocket.h
	$(RM) $(includedir)/commandline.h
	$(RM) $(includedir)/daemon.h
	$(RM) $(includedir)/daemonprocess.h
	$(RM) $(includedir)/datetime.h
	$(RM) $(includedir)/genericsocket.h
	$(RM) $(includedir)/permissions.h
	$(RM) $(includedir)/randomnumber.h
	$(RM) $(includedir)/regularexpression.h
	$(RM) $(includedir)/semaphoreset.h
	$(RM) $(includedir)/server.h
	$(RM) $(includedir)/serverpool.h
	$(RM) $(includedir)/serversocketpool.h
	$(RM) $(includedir)/sharedmemory.h
	$(RM) $(includedir)/signalclasses.h
	$(RM) $(includedir)/text.h
	$(RM) $(includedir)/trim.h
	$(RM) $(includedir)/encoding.h
	$(RM) $(includedir)/casechange.h
	$(RM) $(includedir)/private/client.h
	$(RM) $(includedir)/private/clientsocket.h
	$(RM) $(includedir)/private/commandline.h
	$(RM) $(includedir)/private/daemon.h
	$(RM) $(includedir)/private/daemonprocess.h
	$(RM) $(includedir)/private/genericsocket.h
	$(RM) $(includedir)/private/randomnumber.h
	$(RM) $(includedir)/private/regularexpression.h
	$(RM) $(includedir)/private/semaphoreset.h
	$(RM) $(includedir)/private/sharedmemory.h
	$(RM) $(includedir)/private/server.h
	$(RM) $(includedir)/private/serverpool.h
	$(RM) $(includedir)/private/serversocketpool.h
	$(RM) $(includedir)/private/signalhandler.h
	$(RM) $(includedir)/private/signalset.h
	$(RM) $(includedir)/private/datetime.h
	$(RMTREE) $(includedir)/rudiments
