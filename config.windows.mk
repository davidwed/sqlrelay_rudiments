RUDIMENTS_VERSION = 0.50
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

# slash
SLASH = \\

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

# link commands
LTLINK =
LINK = link
AR =
LDFLAGS = /nologo /dll
LINKFLAGS =
INSTALLLIB = installdll
UNINSTALLLIB = uninstalldll
LIBEXT = dll
OBJEXT = obj

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
SSLLIBS =

# regular expression library
PCRELIBS =

# socket library
SOCKETLIBS =

# pthreads library
PTHREADLIB =

# nanosleep library
NANOSLEEPLIB =

# clock_nanosleep library
CLOCKNANOSLEEPLIB =

# shmopen library
SHMOPENLIB =

# crypt library
CRYPTLIB =

# inet_aton library
INETATONLIB =

# netapi32 library
NETAPI32LIB = @NETAPI32LIB@

# dl library
DLLIB =

# be library
BELIB =

# gnu library
GNULIB =

# crt library
CRTLIB =

# getpgid library
GETPGIDLIB =

# Microsoft-specific stuff
EXE = .exe
