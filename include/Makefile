include ../config.mk
include ../features.mk

top_srcdir = ..
INSTALL = $(top_srcdir)/install-sh -c

HEADERS =
PRIVATEHEADERS = config.h rudimentsinlines.h

ifeq ($(INCLUDE_CLIENT),1)
	HEADERS += client.h
	PRIVATEHEADERS += client.h
endif
ifeq ($(INCLUDE_COMMANDLINE),1)
	HEADERS += commandline.h
	PRIVATEHEADERS += commandline.h commandlineinlines.h
endif
ifeq ($(INCLUDE_DAEMONPROCESS),1)
	HEADERS += daemonprocess.h
	PRIVATEHEADERS += daemonprocess.h daemonprocessinlines.h
endif
ifeq ($(INCLUDE_DATATRANSPORT),1)
	HEADERS += datatransportfactory.h datatransport.h
	PRIVATEHEADERS += datatransport.h datatransportinlines.h
endif
ifeq ($(INCLUDE_DATETIME),1)
	HEADERS += datetime.h
	PRIVATEHEADERS += datetime.h datetimeinlines.h
endif
#ifeq ($(INCLUDE_DICTIONARY),1)
	HEADERS += dictionary.h
	PRIVATEHEADERS += dictionary.h dictionaryinlines.h \
				dictionarynode.h dictionarynodeinlines.h
#endif
ifeq ($(INCLUDE_DTD),1)
	HEADERS += dtd.h
	PRIVATEHEADERS += dtd.h dtdinlines.h
endif
ifeq ($(INCLUDE_ERRORHANDLER),1)
	HEADERS += errorhandler.h
	PRIVATEHEADERS += errorhandler.h errorhandlerinlines.h
endif
ifeq ($(INCLUDE_FILEDESCRIPTOR),1)
	HEADERS += filedescriptor.h
	PRIVATEHEADERS += filedescriptor.h filedescriptorinlines.h
endif
ifeq ($(INCLUDE_FILE),1)
	HEADERS += file.h
	PRIVATEHEADERS += file.h fileinlines.h
endif
ifeq ($(INCLUDE_GROUPENTRY),1)
	HEADERS += groupentry.h
	PRIVATEHEADERS += groupentry.h groupentryinlines.h
endif
ifeq ($(INCLUDE_HOSTENTRY),1)
	HEADERS += hostentry.h
	PRIVATEHEADERS += hostentry.h hostentryinlines.h
endif
ifeq ($(INCLUDE_INETCLIENT),1)
	HEADERS += inetclientsocket.h
	PRIVATEHEADERS += inetclientsocketinlines.h
endif
ifeq ($(INCLUDE_INETSERVER),1)
	HEADERS += inetserversocket.h
	PRIVATEHEADERS += inetserversocketinlines.h \
				inetsocket.h inetsocketinlines.h
endif
ifeq ($(INCLUDE_LISTENER),1)
	HEADERS += listener.h
	PRIVATEHEADERS += listener.h listenerinlines.h
endif
#ifeq ($(INCLUDE_LIST),1)
	HEADERS += list.h
	PRIVATEHEADERS += list.h listinlines.h listnode.h listnodeinlines.h \
				listutil.h listutilinlines.h
#endif
ifeq ($(INCLUDE_LOGGER),1)
	HEADERS += logger.h
	PRIVATEHEADERS += filedestination.h \
				logdestination.h logger.h loggerinlines.h \
				stderrdestination.h stdoutdestination.h \
				syslogdestination.h
endif
ifeq ($(INCLUDE_MEMORYPOOL),1)
	HEADERS += memorypool.h
	PRIVATEHEADERS += memorypool.h memorypoolinlines.h memorypoolnode.h
endif
ifeq ($(INCLUDE_PASSWDENTRY),1)
	HEADERS += passwdentry.h
	PRIVATEHEADERS += passwdentry.h passwdentryinlines.h
endif
ifeq ($(INCLUDE_PERMISSIONS),1)
	HEADERS += permissions.h
	PRIVATEHEADERS += permissionsinlines.h
endif
ifeq ($(INCLUDE_PROTOCOLENTRY),1)
	HEADERS += protocolentry.h
	PRIVATEHEADERS += protocolentry.h protocolentryinlines.h
endif
ifeq ($(INCLUDE_RANDOMNUMBER),1)
	HEADERS += randomnumber.h
	PRIVATEHEADERS += randomnumberinlines.h
endif
ifeq ($(INCLUDE_REGULAREXPRESSION),1)
	HEADERS += regularexpression.h
	PRIVATEHEADERS += regularexpression.h regularexpressioninlines.h
endif
ifeq ($(INCLUDE_SEMAPHORESET),1)
	HEADERS += semaphoreset.h
	PRIVATEHEADERS += semaphoreset.h semaphoresetinlines.h
endif
ifeq ($(INCLUDE_SERVER),1)
	HEADERS += server.h serversocket.h
	PRIVATEHEADERS += serverinlines.h serversocket.h serversocketinlines.h
endif
ifeq ($(INCLUDE_SHAREDMEMORY),1)
	HEADERS += sharedmemory.h
	PRIVATEHEADERS += sharedmemory.h sharedmemoryinlines.h
endif
ifeq ($(INCLUDE_SIGNALCLASSES),1)
	HEADERS += signalclasses.h
	PRIVATEHEADERS += signalclassesinlines.h signalhandler.h signalset.h
endif
ifeq ($(INCLUDE_STRINGBUFFER),1)
	HEADERS += stringbuffer.h
	PRIVATEHEADERS += stringbuffer.h stringbufferinlines.h
endif
ifeq ($(INCLUDE_STRING),1)
	HEADERS += string.h
	PRIVATEHEADERS += string.h stringinlines.h
endif
ifeq ($(INCLUDE_UNIXCLIENT),1)
	HEADERS += unixclientsocket.h
	PRIVATEHEADERS += unixclientsocketinlines.h
endif
ifeq ($(INCLUDE_UNIXSERVER),1)
	HEADERS += unixserversocket.h
	PRIVATEHEADERS += unixserversocket.h unixserversocketinlines.h \
				unixsocket.h unixsocketinlines.h
endif
ifeq ($(INCLUDE_VARIABLEBUFFER),1)
	HEADERS += variablebuffer.h
	PRIVATEHEADERS += variablebuffer.h variablebufferinlines.h
endif
ifeq ($(INCLUDE_XMLDOM),1)
	HEADERS += xmldom.h xmldomnode.h
	PRIVATEHEADERS += xmldom.h xmldominlines.h \
				xmldomnode.h xmldomnodeinlines.h
endif
ifeq ($(INCLUDE_XMLSAX),1)
	HEADERS += xmlsax.h
	PRIVATEHEADERS += xmlsax.h xmlsaxinlines.h
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
