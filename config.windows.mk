RUDIMENTS_VERSION = 0.50
SONAME_VERSION_INFO =

prefix = C:\\Program Files\\Firstworks
exec_prefix = $(prefix)\\bin
bindir = $(prefix)\\bin
includedir = $(prefix)\\include
libdir = $(prefix)\\lib
datadir = $(prefix)\\share
docdir = ${datadir}\\doc\\rudiments

# includes and defines
BASECPPFLAGS = /Wall

# debug and optimization flags (-g, -O2, etc.)
# NOTE: -FPIC gets added if we're not using libtool
CXXFLAGS = /O2

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

# install commands
LTINSTALL =
CP = cp
CHMOD = chmod
INSTALL = cp
MKINSTALLDIRS = mkdir
LTFINISH = echo

# commands to erase files and directories
LTCLEAN =
LTUNINSTALL =
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
