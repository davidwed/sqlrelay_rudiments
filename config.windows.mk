RUDIMENTS_VERSION = 0.50
SONAME_VERSION_INFO =

prefix = C:\\Program Files\\Firstworks
exec_prefix = $(prefix)\\bin
bindir = $(prefix)\\bin
includedir = $(prefix)\\include
libdir = $(prefix)\\lib
mandir = $(prefix)\\share\\man
datadir = $(prefix)\\share
docdir = $(datadir)\\doc\\rudiments

# command separator
AND = &

# compile flags
BASECPPFLAGS = /O2 /DWIN32 /DLIBRUDIMENTS_EXPORTS
EXTRACPPFLAGS =
CXXFLAGS =
INC = /I

# linker flags
LDFLAGS =
LINKFLAGS =

# libtool command
LIBTOOL =

# compile commands
LTCOMPILE =
LTLINK =
CXX = cl
LINK = link
AR =
COMPILE = /c
OUT = -out:
LIBEXT = dll

# install commands
LTINSTALL =
CP = copy
CHMOD = echo
INSTALL = copy
MKINSTALLDIRS = mkdir
LTFINISH = echo

# uninstall/clean commands
LTUNINSTALL =
LTCLEAN =
RM = del
RMTREE = rmdir /s /q

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
NETAPI32LIB =

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
