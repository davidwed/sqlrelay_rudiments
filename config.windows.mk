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
CP = copy
CHMOD = echo
MKINSTALLDIRS = md
LTFINISH = echo

# uninstall/clean commands
LTUNINSTALL =
LTCLEAN =
RM = del
RMTREE = rd /s /q

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
NETAPI32LIB = ws2_32.lib netapi32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib

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
