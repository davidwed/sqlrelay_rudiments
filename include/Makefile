include ../config.mk
include ../features.mk

top_srcdir = ..
INSTALL = $(top_srcdir)/install-sh -c

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
ifeq ($(INCLUDE_DATATRANSPORT),1)
	HEADERS += datatransportfactory.h datatransport.h
	PRIVATEHEADERS += datatransport.h datatransportincludes.h \
					datatransportfactoryincludes.h
endif
ifeq ($(INCLUDE_DATETIME),1)
	HEADERS += datetime.h timezonefile.h
	PRIVATEHEADERS += datetime.h datetimeincludes.h \
				timezonefile.h 
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
ifeq ($(INCLUDE_FILESYSTEM),1)
	HEADERS += filesystem.h
	PRIVATEHEADERS += filesystem.h filesystemincludes.h
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
	PRIVATEHEADERS += inetclientsocketincludes.h
endif
ifeq ($(INCLUDE_INETSERVER),1)
	HEADERS += inetserversocket.h
	PRIVATEHEADERS += inetsocket.h inetserversocketincludes.h
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
	PRIVATEHEADERS += randomnumberincludes.h
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
	HEADERS += server.h serversocket.h
	PRIVATEHEADERS += serversocket.h serverincludes.h serversocketincludes.h
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
ifeq ($(INCLUDE_UNIXCLIENT),1)
	HEADERS += unixclientsocket.h
	PRIVATEHEADERS += unixclientsocketincludes.h
endif
ifeq ($(INCLUDE_UNIXSERVER),1)
	HEADERS += unixserversocket.h
	PRIVATEHEADERS += unixserversocket.h unixsocket.h \
					unixserversocketincludes.h
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


install:
	$(top_srcdir)/mkinstalldirs $(incdir)/rudiments
	$(top_srcdir)/mkinstalldirs $(incdir)/rudiments/private
	for i in $(HEADERS); do $(INSTALL) -m 644 rudiments/$$i $(incdir)/rudiments; done
	for i in $(PRIVATEHEADERS); do $(INSTALL) -m 644 rudiments/private/$$i $(incdir)/rudiments/private; done

uninstall:
	$(RM) $(incdir)/client.h
	$(RM) $(incdir)/clientsocket.h
	$(RM) $(incdir)/commandline.h
	$(RM) $(incdir)/daemon.h
	$(RM) $(incdir)/daemonprocess.h
	$(RM) $(incdir)/datetime.h
	$(RM) $(incdir)/genericsocket.h
	$(RM) $(incdir)/permissions.h
	$(RM) $(incdir)/randomnumber.h
	$(RM) $(incdir)/regularexpression.h
	$(RM) $(incdir)/semaphoreset.h
	$(RM) $(incdir)/server.h
	$(RM) $(incdir)/serverpool.h
	$(RM) $(incdir)/serversocketpool.h
	$(RM) $(incdir)/serversocket.h
	$(RM) $(incdir)/sharedmemory.h
	$(RM) $(incdir)/signalclasses.h
	$(RM) $(incdir)/text.h
	$(RM) $(incdir)/trim.h
	$(RM) $(incdir)/encoding.h
	$(RM) $(incdir)/casechange.h
	$(RM) $(incdir)/private/client.h
	$(RM) $(incdir)/private/clientsocket.h
	$(RM) $(incdir)/private/commandline.h
	$(RM) $(incdir)/private/daemon.h
	$(RM) $(incdir)/private/daemonprocess.h
	$(RM) $(incdir)/private/genericsocket.h
	$(RM) $(incdir)/private/randomnumber.h
	$(RM) $(incdir)/private/regularexpression.h
	$(RM) $(incdir)/private/semaphoreset.h
	$(RM) $(incdir)/private/sharedmemory.h
	$(RM) $(incdir)/private/server.h
	$(RM) $(incdir)/private/serverpool.h
	$(RM) $(incdir)/private/serversocketpool.h
	$(RM) $(incdir)/private/serversocket.h
	$(RM) $(incdir)/private/signalhandler.h
	$(RM) $(incdir)/private/signalset.h
	$(RM) $(incdir)/private/datetime.h
	$(RMTREE) $(incdir)/rudiments
