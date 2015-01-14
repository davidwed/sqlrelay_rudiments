RUDIMENTS_VERSION = @RUDIMENTS_VERSION@
SONAME_VERSION_INFO =

prefix = @prefix@
exec_prefix = @exec_prefix@
bindir = @bindir@
includedir = @includedir@
libdir = @libdir@
mandir = @mandir@
datadir = @datadir@
docdir = $(datadir)\doc\rudiments

# command separator
AND = &

# libtool command
LIBTOOL =

# compile commands
LTCOMPILE =
CXX = cl
COMPILE = /c
OUT = -out:
BASECPPFLAGS = /nologo /O2 /D LIBRUDIMENTS_EXPORTS
EXTRACPPFLAGS =
CXXFLAGS =
INC = /I
OBJ = obj

# link commands
LTLINK =
LINK = link
AR =
LDFLAGS = /nologo
LINKFLAGS = /dll
INSTALLLIB = installdll
UNINSTALLLIB = uninstalldll
LIBEXT = dll

# install commands
LTINSTALL =
CP = @top_builddir@\cp.vbs
CHMOD = echo
MKINSTALLDIRS = @top_builddir@\mkinstalldirs.vbs
LTFINISH = echo

# uninstall/clean commands
LTUNINSTALL =
LTCLEAN =
RM = @top_builddir@\rm.vbs
RMTREE = @top_builddir@\rmtree.vbs

# ssl library
SSLLIBS = @SSLLIBS@

# regular expression library
PCRELIBS = @PCRELIBS@

# socket library
SOCKETLIBS = @SOCKETLIBS@

# pthreads library
PTHREADLIB = @PTHREADLIB@

# nanosleep library
NANOSLEEPLIB = @NANOSLEEPLIB@

# clock_nanosleep library
CLOCKNANOSLEEPLIB = @CLOCKNANOSLEEPLIB@

# shmopen library
SHMOPENLIB = @SHMOPENLIB@

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
