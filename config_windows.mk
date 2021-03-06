RUDIMENTS_VERSION = @RUDIMENTS_VERSION@

# installation directories
prefix = @prefix@
exec_prefix = @exec_prefix@
bindir = @bindir@
includedir = @includedir@
libdir = @libdir@
mandir = @mandir@
datadir = @datadir@
docdir = $(datadir)\doc\rudiments
licensedir = $(datadir)\licenses\rudiments

# command separator
AND = &

# "this directory" prefix
THISDIR =

# shell
SHELL =

# libtool command
LIBTOOL =

# compile commands
LTCOMPILE =
CXX = cl
COMPILE = /c
OUT = -out:
BASECPPFLAGS = /nologo @OPTCPPFLAGS@ @DEBUGCPPFLAGS@ @WINVER@ @WIN32WINDOWS@ @WIN32WINNT@ @_USE_32BIT_TIME_T@ @SDKINCLUDES@ @GSSINCLUDES@ @LIBCURLINCLUDES@
EXPORTSCPPFLAGS = /D LIBRUDIMENTS_EXPORTS
EXTRACPPFLAGS =
CXXFLAGS =
INC = /I
OBJ = obj

# link commands
LTLINK =
LINK = link
AR =
LDFLAGS = /nologo @DEBUGLDFLAGS@ @SDKLIBS@
LINKFLAGS = /dll
INSTALLLIB = installdll
UNINSTALLLIB = uninstalldll
LIBEXT = dll

# install commands
LTINSTALL =
CP = cscript /nologo @top_builddir@\cp.vbs
CHMOD = echo
MKINSTALLDIRS = cscript /nologo @top_builddir@\mkinstalldirs.vbs
LTFINISH = echo

# uninstall/clean commands
LTUNINSTALL =
LTCLEAN =
RM = cscript /nologo @top_builddir@\rm.vbs
RMTREE = cscript /nologo @top_builddir@\rmtree.vbs

# ssl library
SSLLIBS = @SSLLIBS@

# test cert target
SERVERPEM = @SERVERPEM@

# GSS library
GSSLIBS = @GSSLIBS@

# regular expression library
PCRELIBS = @PCRELIBS@

# libcurl library
LIBCURLLIBS = @LIBCURLLIBS@

# socket library
SOCKETLIBS = @SOCKETLIBS@

# pthreads library
PTHREADLIB = @PTHREADLIB@

# nanosleep library
NANOSLEEPLIB = @NANOSLEEPLIB@

# clock_nanosleep library
CLOCKNANOSLEEPLIB = @CLOCKNANOSLEEPLIB@

# crypt library
CRYPTLIB = @CRYPTLIB@

# inet_aton library
INETATONLIB = @INETATONLIB@

# netapi32 library
NETAPI32LIB = @NETAPI32LIB@

# dl library
DLLIB = @DLLIB@

# be library
BELIB = @BELIB@

# gnu library
GNULIB = @GNULIB@

# crt library
CRTLIB = @CRTLIB@

# getpgid library
GETPGIDLIB = @GETPGIDLIB@

# rudiments library (for tests)
RUDIMENTSLIB = /LIBPATH:@top_builddir@\src librudiments.lib

# Microsoft-specific stuff
EXE = @EXE@
