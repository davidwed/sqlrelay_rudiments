AC_DEFUN([FW_INCLUDES],
[
if ( test -n "$2" )
then
	echo "$1 includes... $2"
fi
])

AC_DEFUN([FW_LIBS],
[
if ( test -n "$2" )
then
	echo "$1 libs... $2"
fi
])

AC_DEFUN([FW_CHECK_FILE],
[
if ( test -r "$1" )
then
	eval "$2"
fi
])

AC_DEFUN([FW_TRY_LINK],
[
SAVECPPFLAGS="$CPPFLAGS"
SAVELIBS="$LIBS"
SAVE_LD_LIBRARY_PATH="$LD_LIBRARY_PATH"
CPPFLAGS="$3"
LIBS="$HACKLIBS $4"
LD_LIBRARY_PATH="$5"
export LD_LIBRARY_PATH
AC_TRY_LINK([$1],[$2],[$6],[$7])
CPPFLAGS="$SAVECPPFLAGS"
LIBS="$SAVELIBS"
LD_LIBRARY_PATH="$SAVE_LD_LIBRARY_PATH"
export LD_LIBRARY_PATH
])


AC_DEFUN([FW_CHECK_LIB],
[
FOUNDLIB=""
FW_CHECK_FILE($1, FOUNDLIB="yes")
if ( test -n "$FOUNDLIB" )
then
	eval "$2"
else
	if ( test -n "$3" )
	then
		FW_CHECK_FILE($3, FOUNDLIB="yes")
		if ( test -n "$FOUNDLIB" )
		then
			eval "$4"
		fi
	fi
fi
])


AC_DEFUN([FW_CHECK_HEADER_LIB],
[
FOUNDHEADER=""
FOUNDLIB=""
FW_CHECK_FILE($1, FOUNDHEADER="yes")
FW_CHECK_FILE($3, FOUNDLIB="yes")
if ( test -n "$FOUNDLIB" )
then
	if ( test -n "$FOUNDHEADER" -a -n "$FOUNDLIB" )
	then
		eval "$2"
		eval "$4"
	fi
else
	if ( test -n "$5" -a -n "$6" )
	then
		FW_CHECK_FILE($5, FOUNDLIB="yes")
		if ( test -n "$FOUNDHEADER" -a -n "$FOUNDLIB" )
		then
			eval "$2"
			eval "$6"
		fi
	fi
fi
])


AC_DEFUN([FW_CHECK_HEADERS_AND_LIBS],
[

eval "$7=\"\""
eval "$8=\"\""
eval "$9=\"\""
eval "$10=\"\""
if ( test -n "$11" )
then
	eval "$11=\"\""
fi

SEARCHPATH=$1
NAME=$2
HEADER=$3
LIBNAME=$4
LINKSTATIC=$5
LINKRPATH=$6
INCLUDESTRING=""
LIBSTRING=""
LIBPATH=""
STATIC=""
HEADERSANDLIBSPATH=""

FW_CHECK_HEADER_LIB([/usr/include/$HEADER],[INCLUDESTRING=\"\"],[/usr/lib/lib$LIBNAME.$SO],[LIBPATH=\"\"; LIBSTRING=\"-l$LIBNAME\"],[/usr/lib/lib$LIBNAME.a],[LIBSTRING=\"-l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
if ( test -z "$LIBSTRING" )
then
	FW_CHECK_HEADER_LIB([/usr/include/$NAME/$HEADER],[INCLUDESTRING=\"-I/usr/include/$NAME\"],[/usr/lib/lib$LIBNAME.$SO],[LIBPATH=\"\"; LIBSTRING=\"-l$LIBNAME\"],[/usr/lib/lib$LIBNAME.a],[LIBSTRING=\"-l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
fi
if ( test -z "$LIBSTRING" )
then
	FW_CHECK_HEADER_LIB([/usr/include/$HEADER],[INCLUDESTRING=\"\"],[/usr/lib/$NAME/lib$LIBNAME.$SO],[LIBPATH=\"/usr/lib/$NAME\"; LIBSTRING=\"-L/usr/lib/$NAME -l$LIBNAME\"],[/usr/lib/$NAME/lib$LIBNAME.a],[LIBSTRING=\"-L/usr/lib/$NAME -l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
fi
if ( test -z "$LIBSTRING" )
then
	FW_CHECK_HEADER_LIB([/usr/include/$NAME/$HEADER],[INCLUDESTRING=\"-I/usr/include/$NAME\"],[/usr/lib/$NAME/lib$LIBNAME.$SO],[LIBPATH=\"/usr/lib/$NAME\"; LIBSTRING=\"-L/usr/lib/$NAME -l$LIBNAME\"],[/usr/lib/$NAME/lib$LIBNAME.a],[LIBSTRING=\"-L/usr/lib/$NAME -l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
fi

if ( test -z "$LIBSTRING" )
then

	for paths in "$SEARCHPATH" "/usr/local/$NAME" "/opt/$NAME" "/usr/$NAME" "/usr/local" "/usr/pkg" "/opt/sfw" "/usr/local/firstworks"
	do
		if ( test -n "$paths" )
		then
			FW_CHECK_HEADER_LIB([$paths/include/$HEADER],[INCLUDESTRING=\"-I$paths/include\"],[$paths/lib/lib$LIBNAME.$SO],[LIBPATH=\"$paths/lib\"; LIBSTRING=\"-L$paths/lib -l$LIBNAME\"],[$paths/lib/lib$LIBNAME.a],[LIBSTRING=\"-L$paths/lib -l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
			if ( test -z "$LIBSTRING" )
			then
				FW_CHECK_HEADER_LIB([$paths/include/$NAME/$HEADER],[INCLUDESTRING=\"-I$paths/include/$NAME\"],[$paths/lib/lib$LIBNAME.$SO],[LIBPATH=\"$paths/lib\"; LIBSTRING=\"-L$paths/lib -l$LIBNAME\"],[$paths/lib/lib$LIBNAME.a],[LIBSTRING=\"-L$paths/lib -l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
			fi
			if ( test -z "$LIBSTRING" )
			then
				FW_CHECK_HEADER_LIB([$paths/include/$HEADER],[INCLUDESTRING=\"-I$paths/include\"],[$paths/lib/$NAME/lib$LIBNAME.$SO],[LIBPATH=\"$paths/lib/$NAME\"; LIBSTRING=\"-L$paths/lib/$NAME -l$LIBNAME\"],[$paths/lib/$NAME/lib$LIBNAME.a],[LIBSTRING=\"-L$paths/lib/$NAME -l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
			fi
			if ( test -z "$LIBSTRING" )
			then
				FW_CHECK_HEADER_LIB([$paths/include/$NAME/$HEADER],[INCLUDESTRING=\"-I$paths/include/$NAME\"],[$paths/lib/$NAME/lib$LIBNAME.$SO],[LIBPATH=\"$paths/lib/$NAME\"; LIBSTRING=\"-L$paths/lib/$NAME -l$LIBNAME\"],[$paths/lib/$NAME/lib$LIBNAME.a],[LIBSTRING=\"-L$paths/lib/$NAME -l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
			fi
			if ( test -n "$LIBSTRING" )
			then
				HEADERSANDLIBSPATH="$paths"
				break
			fi
		fi
	done
else
	HEADERSANDLIBSPATH="/usr"
fi

eval "$7=\"$INCLUDESTRING\""
eval "$8=\"$LIBSTRING\""
eval "$9=\"$LIBPATH\""
eval "$10=\"$STATIC\""
if ( test -n "$11" )
then
	eval "$11=\"$HEADERSANDLIBSPATH\""
fi
])



dnl override libtool if so desired
dnl a bit crude, but AC_PROG_LIBTOOL sets vital
dnl environment variables, it seems
AC_DEFUN([FW_CHECK_USE_SYSTEM_LIBTOOL],
[
if ( test "$USE_SYSTEM_LIBTOOL" = "yes" )
then
  LIBTOOL="libtool"
fi
])



dnl determines whether we're using GNU strip or not
dnl sets the substitution variable STRIP to "touch" if we're not
AC_DEFUN([FW_CHECK_GNU_STRIP],
[
GNUSTRIP=`$STRIP --version 2> /dev/null | grep "GNU strip"`
AC_MSG_CHECKING(for GNU strip)
if ( test -n "$GNUSTRIP" )
then
	AC_MSG_RESULT(yes)
else
	STRIP="touch"
	AC_MSG_RESULT(no)
fi
AC_SUBST(STRIP)
])


dnl checks to see if -pipe option to gcc works or not
AC_DEFUN([FW_CHECK_PIPE],
[
AC_MSG_CHECKING(for -pipe option)
FW_TRY_LINK([#include <stdio.h>],[printf("hello");],[-pipe],[],[],[PIPE="-pipe"],[PIPE=""])
if ( test -n "$PIPE" )
then
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
fi
AC_SUBST(PIPE)
])


dnl Determines what extension shared object files have
AC_DEFUN([FW_CHECK_SO_EXT],
[
AC_MSG_CHECKING(for dynamic library extension)
if ( test "`uname -s`" = "Darwin" )
then
	SO="dylib"
else
	SO="so"
fi
AC_MSG_RESULT($SO)
])


dnl Checks for microsoft platform.
dnl sets the substitution variables MINGW32, CYGWIN and UWIN as appropriate
dnl also moves INSTALL to INSTALL.txt if we're using windows
dnl sets the enviroment variable MICROSOFT
AC_DEFUN([FW_CHECK_MICROSOFT],
[
CYGWIN=""
MINGW32=""
UWIN=""
dnl AC_CANONICAL_HOST gets called when AC_PROG_LIBTOOL is called
case $host_os in
	*cygwin* ) CYGWIN="yes";;
	*mingw32* ) MINGW32="yes";;
	*uwin* ) UWIN="yes";;
esac
AC_SUBST(MINGW32)
AC_SUBST(CYGWIN)
AC_SUBST(UWIN)

dnl Hack so "make install" will work on windows.
MICROSOFT=""
if ( test "$UWIN" = "yes" -o "$MINGW32" = "yes" -o "$CYGWIN" = "yes" )
then
	if ( test -r "INSTALL" )
	then
		mv INSTALL INSTALL.txt
	fi
	MICROSOFT="yes"
fi
])


dnl checks if the compiler supports the inline keyword
dnl defines the macro INLINE
AC_DEFUN([FW_CHECK_INLINE],
[
AC_MSG_CHECKING(inline)
INLINE="inline"
dnl intel optimizing compiler doesn't have inlines, assume that CC doesn't
dnl either even though it might, this test needs to be more robust
if ( test "$CXX" = "icc" -o "$CXX" = "CC" )
then
	INLINE=""
else 
	dnl redhat's gcc 2.96 has problems with inlines
	CXX_VERSION=`$CXX --version`
	if ( test "$CXX_VERSION" = "2.96" )
	then
		INLINE=""
	fi
fi
if ( test "$INLINE" = "inline" )
then
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
fi
AC_DEFINE_UNQUOTED(INLINE,$INLINE,Some compliers don't support the inline keyword)
])


dnl checks for the pthreads library
dnl requires:  PTHREADPATH, RPATHFLAG, cross_compiling
dnl sets the substitution variable PTHREADLIB
AC_DEFUN([FW_CHECK_PTHREAD],
[

HAS_THREADS="no"
if ( test "$ENABLE_RUDIMENTS_THREADS" = "yes" )
then

	HAVE_PTHREAD=""
	PTHREADINCLUDES=""
	PTHREADLIB=""

	if ( test "$cross_compiling" = "yes" )
	then
	
		dnl cross compiling
		echo "cross compiling"
		if ( test -n "$PTHREADPATH" )
		then
			PTHREADINCLUDES="-I$PTHREADPATH/include"
			PTHREADLIB="-L$PTHREADPATH/lib -lpthread"
		else
			PTHREADLIB="-lpthread"
		fi
		HAVE_PTHREAD="yes"

	else

		for i in "pthread" "c_r" "gthreads"
		do
			FW_CHECK_HEADERS_AND_LIBS([$PTHREADPATH],[pthread],[pthread.h],[$i],[""],[""],[PTHREADINCLUDES],[PTHREADLIB],[PTHREADLIBPATH],[PTHREADSTATIC])
			if ( test -n "$PTHREADLIB" )
			then
				if ( test "$i" = "c_r" )
				then
					PTHREADLIB="$PTHREADLIB -pthread"
				fi
				break
			fi
		done
		if ( test -n "$PTHREADLIB" )
		then
			HAVE_PTHREAD="yes"
		fi

		dnl override PTHREADLIB on microsoft platforms
		if ( test -n "$PTHREADINCLUDES" -a "$MICROSOFT" = "yes" )
		then
			PTHREADLIB="-pthread"
		fi
	fi

	FW_INCLUDES(pthreads,[$PTHREADINCLUDES])
	FW_LIBS(pthreads,[$PTHREADLIB])

	if ( test -z "$HAVE_PTHREAD" )
	then
		AC_MSG_ERROR(pthread library not found.  Rudiments requires this package.)
		exit
	fi

	AC_DEFINE(RUDIMENTS_HAS_THREADS,1,Rudiments supports threads)
	HAS_THREADS="yes"

else

	echo "disabled"
	HAS_THREADS="yes"

fi

AC_SUBST(HAS_THREADS)
AC_SUBST(PTHREADINCLUDES)
AC_SUBST(PTHREADLIB)
])



dnl checks for the ssl library
dnl requires:  SSLPATH, RPATHFLAG, cross_compiling
dnl sets the substitution variable SSLLIBS
AC_DEFUN([FW_CHECK_SSL],
[

if ( test "$ENABLE_RUDIMENTS_SSL" = "yes" )
then

	if ( test "$cross_compiling" = "yes" )
	then

		dnl cross compiling
		echo "cross compiling"
		if ( test -n "$SSLLIBS" -o -n "$SSLINCLUDES" )
		then
			AC_DEFINE(RUDIMENTS_HAS_SSL,1,Rudiments supports SSL)
		fi

	else

		AC_MSG_CHECKING(openssl)
		if ( test -z "$SSLLIBS" -a -z "$SSLINCLUDES" )
		then
			SSLLIBS=`pkg-config openssl --libs 2> /dev/null`
			if ( test -n "$SSLLIBS" )
			then
				SSLINCLUDES=`pkg-config openssl --cflags 2> /dev/null`
			fi
		fi

		if ( test -z "$SSLLIBS" -a -z "$SSLINCLUDES" )
		then
			FW_CHECK_HEADERS_AND_LIBS([/usr],[ssl],[openssl/ssl.h],[ssl],[""],[""],[SSLINCLUDES],[SSLLIBS],[SSLLIBPATH],[SSLSTATIC])
		fi

		if ( test -n "$SSLLIBS" )
		then
			AC_DEFINE(RUDIMENTS_HAS_SSL,1,Rudiments supports SSL)
			AC_MSG_RESULT(yes)
		else
			AC_MSG_RESULT(no)
		fi
	fi

	FW_INCLUDES(ssl,[$SSLINCLUDES])
	FW_LIBS(ssl,[$SSLLIBS])

else

	echo "disabled"

fi

AC_SUBST(SSLINCLUDES)
AC_SUBST(SSLLIBS)
])



dnl checks for the pcre library
dnl requires:  cross_compiling
dnl sets the substitution variable PCRELIBS
AC_DEFUN([FW_CHECK_PCRE],
[

HAVE_PCRE=""
if ( test "$ENABLE_RUDIMENTS_PCRE" = "yes" )
then

	if ( test "$cross_compiling" = "yes" )
	then

		dnl cross compiling
		echo "cross compiling"
		if ( test -n "$PCRELIBS" -o -n "$PCREINCLUDES" )
		then
			AC_DEFINE(RUDIMENTS_HAS_PCRE,1,Rudiments supports PCRE)
		fi

	else

		AC_MSG_CHECKING(pcre)
		if ( test -z "$PCRELIBS" -a -z "$PCREINCLUDES" )
		then
			PCRELIBS=`pcre-config --libs 2> /dev/null`
			if ( test -n "$PCRELIBS" )
			then
				PCREINCLUDES=`pcre-config --cflags 2> /dev/null`
			fi
		fi

		if ( test -z "$PCRELIBS" -a -z "$PCREINCLUDES" )
		then
			FW_CHECK_HEADERS_AND_LIBS([/usr],[pcre],[pcre/pcre.h],[pcre],[""],[""],[PCREINCLUDES],[PCRELIBS],[PCRELIBPATH],[PCRESTATIC])
		fi

		if ( test -n "$PCRELIBS" )
		then
			FW_TRY_LINK([#include <stdlib.h>
#include <pcre.h>],[pcre_extra *extra=pcre_study(NULL,0,NULL); delete extra;],[$PCREINCLUDES],[$PCRELIBS],[],[HAVE_PCRE="yes"; AC_DEFINE(RUDIMENTS_HAS_PCRE,1,Rudiments supports PCRE) AC_MSG_RESULT(yes)],[PCREINCLUDES=""; PCRELIBS=""; AC_MSG_RESULT(no)])
		else
			AC_MSG_RESULT(no)
		fi
	fi

	FW_INCLUDES(pcre,[$PCREINCLUDES])
	FW_LIBS(pcre,[$PCRELIBS])

else

	echo "disabled"

fi

AC_SUBST(PCREINCLUDES)
AC_SUBST(PCRELIBS)
])

dnl checks for rpc entry functions and header files
AC_DEFUN([FW_CHECK_RPC],
[

HAVE_GETRPCBYNAME_R="no"
AC_MSG_CHECKING(getrpcbyname_r with 5 parameters in netdb.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>],
getrpcbyname_r(NULL,NULL,NULL,0,NULL);,HAVEGETRPCBYNAME_R="yes"; AC_DEFINE(HAVE_GETRPCBYNAME_R_5,1,Some systems have getrpcbyname_r) AC_DEFINE(HAVE_GETRPCBYNAME_R,1, Some systems have getrpcbyname_r) AC_DEFINE(HAVE_NETDB_H,1, Some systems have netdb.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getrpcbyname_r with 5 parameters in rpc/rpcent.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpcent.h>],
getrpcbyname_r(NULL,NULL,NULL,0,NULL);,HAVEGETRPCBYNAME_R="yes"; AC_DEFINE(HAVE_GETRPCBYNAME_R_5,1,Some systems have getrpcbyname_r) AC_DEFINE(HAVE_GETRPCBYNAME_R,1, Some systems have getrpcbyname_r) AC_DEFINE(HAVE_RPCENT_H,1, Some systems have rpc/rpcent.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getrpcbyname_r with 5 parameters in rpc/rpc.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpc.h>],
getrpcbyname_r(NULL,NULL,NULL,0,NULL);,HAVEGETRPCBYNAME_R="yes"; AC_DEFINE(HAVE_GETRPCBYNAME_R_5,1,Some systems have getrpcbyname_r) AC_DEFINE(HAVE_GETRPCBYNAME_R,1, Some systems have getrpcbyname_r) AC_DEFINE(HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getrpcbyname_r with 4 parameters in netdb.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>],
getrpcbyname_r(NULL,NULL,NULL,0);,HAVEGETRPCBYNAME_R="yes"; AC_DEFINE(HAVE_GETRPCBYNAME_R_4,1,Some systems have getrpcbyname_r) AC_DEFINE(HAVE_GETRPCBYNAME_R,1, Some systems have getrpcbyname_r) AC_DEFINE(HAVE_NETDB_H,1, Some systems have netdb.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getrpcbyname_r with 4 parameters in rpc/rpcent.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpcent.h>],
getrpcbyname_r(NULL,NULL,NULL,0);,HAVEGETRPCBYNAME_R="yes"; AC_DEFINE(HAVE_GETRPCBYNAME_R_4,1,Some systems have getrpcbyname_r) AC_DEFINE(HAVE_GETRPCBYNAME_R,1, Some systems have getrpcbyname_r) AC_DEFINE(HAVE_RPCENT_H,1, Some systems have rpc/rpcent.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getrpcbyname_r with 4 parameters in rpc/rpc.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpc.h>],
getrpcbyname_r(NULL,NULL,NULL,0);,HAVEGETRPCBYNAME_R="yes"; AC_DEFINE(HAVE_GETRPCBYNAME_R_4,1,Some systems have getrpcbyname_r) AC_DEFINE(HAVE_GETRPCBYNAME_R,1, Some systems have getrpcbyname_r) AC_DEFINE(HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

if ( test -z "$HAVEGETRPCBYNAME_R" )
then
	AC_MSG_CHECKING(getrpcbyname in netdb)
	AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>],
getrpcbyname(NULL);,AC_DEFINE(HAVE_GETRPCBYNAME,1, Some systems have getrpcbyname) AC_DEFINE(HAVE_NETDB_H,1, Some systems have netdb.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(getrpcbyname in rpc/rpcent.h)
	AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpcent.h>],
getrpcbyname(NULL);,AC_DEFINE(HAVE_GETRPCBYNAME,1, Some systems have getrpcbyname) AC_DEFINE(HAVE_RPCENT_H,1, Some systems have rpc/rpcent.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(getrpcbyname in rpc/rpc.h)
	AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpc.h>],
getrpcbyname(NULL);,AC_DEFINE(HAVE_GETRPCBYNAME,1, Some systems have getrpcbyname) AC_DEFINE(HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

fi



HAVEGETRPCBYNUMBER_R="no"
AC_MSG_CHECKING(getrpcbynumber_r with 5 parameters in netdb.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>],
getrpcbynumber_r(0,NULL,NULL,0,NULL);,HAVEGETRPCBYNUMBER_R="yes"; AC_DEFINE(HAVE_GETRPCBYNUMBER_R_5,1,Some systems have getrpcbynumber_r) AC_DEFINE(HAVE_GETRPCBYNUMBER_R,1, Some systems have getrpcbynumber_r) AC_DEFINE(HAVE_NETDB_H,1, Some systems have netdb.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getrpcbynumber_r with 5 parameters in rpc/rpcent.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpcent.h>],
getrpcbynumber_r(0,NULL,NULL,0,NULL);,HAVEGETRPCBYNUMBER_R="yes"; AC_DEFINE(HAVE_GETRPCBYNUMBER_R_5,1,Some systems have getrpcbynumber_r) AC_DEFINE(HAVE_GETRPCBYNUMBER_R,1, Some systems have getrpcbynumber_r) AC_DEFINE(HAVE_RPCENT_H,1, Some systems have rpc/rpcent.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getrpcbynumber_r with 5 parameters in rpc/rpc.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpc.h>],
getrpcbynumber_r(0,NULL,NULL,0,NULL);,HAVEGETRPCBYNUMBER_R="yes"; AC_DEFINE(HAVE_GETRPCBYNUMBER_R_5,1,Some systems have getrpcbynumber_r) AC_DEFINE(HAVE_GETRPCBYNUMBER_R,1, Some systems have getrpcbynumber_r) AC_DEFINE(HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getrpcbynumber_r with 4 parameters in netdb.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>],
getrpcbynumber_r(0,NULL,NULL,0);,HAVEGETRPCBYNUMBER_R="yes"; AC_DEFINE(HAVE_GETRPCBYNUMBER_R_4,1,Some systems have getrpcbynumber_r) AC_DEFINE(HAVE_GETRPCBYNUMBER_R,1, Some systems have getrpcbynumber_r) AC_DEFINE(HAVE_NETDB_H,1, Some systems have netdb.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getrpcbynumber_r with 4 parameters in rpc/rpcent.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpcent.h>],
getrpcbynumber_r(0,NULL,NULL,0);,HAVEGETRPCBYNUMBER_R="yes"; AC_DEFINE(HAVE_GETRPCBYNUMBER_R_4,1,Some systems have getrpcbynumber_r) AC_DEFINE(HAVE_GETRPCBYNUMBER_R,1, Some systems have getrpcbynumber_r) AC_DEFINE(HAVE_RPCENT_H,1, Some systems have rpc/rpcent.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getrpcbynumber_r with 4 parameters in rpc/rpc.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpc.h>],
getrpcbynumber_r(0,NULL,NULL,0);,HAVEGETRPCBYNUMBER_R="yes"; AC_DEFINE(HAVE_GETRPCBYNUMBER_R_4,1,Some systems have getrpcbynumber_r) AC_DEFINE(HAVE_GETRPCBYNUMBER_R,1, Some systems have getrpcbynumber_r) AC_DEFINE(HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

if ( test -z "$HAVEGETRPCBYNUMBER_R" )
then
	AC_MSG_CHECKING(getrpcbynumber in netdb)
	AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>],
getrpcbynumber(0);,AC_DEFINE(HAVE_GETRPCBYNUMBER,1, Some systems have getrpcbynumber) AC_DEFINE(HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(getrpcbynumber in rpc/rpcent.h)
	AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpcent.h>],
getrpcbynumber(0);,AC_DEFINE(HAVE_GETRPCBYNUMBER,1, Some systems have getrpcbynumber) AC_DEFINE(HAVE_RPCENT_H,1, Some systems have rpc/rpcent.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(getrpcbynumber in rpc/rpc.h)
	AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpc.h>],
getrpcbynumber(0);,AC_DEFINE(HAVE_GETRPCBYNUMBER,1, Some systems have getrpcbynumber) AC_DEFINE(HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

fi
])

dnl checks for shadow entry functions and header files
AC_DEFUN([FW_CHECK_SHADOW],
[

INCLUDE_SHADOWENTRY="0"

AC_MSG_CHECKING(getspnam_r with 5 parameters)
AC_TRY_LINK([#include <shadow.h>
#include <stdlib.h>],
getspnam_r(NULL,NULL,NULL,0,NULL);,INCLUDE_SHADOWENTRY="1"; AC_DEFINE(HAVE_GETSPNAM_R_5,1,Some systems have getspnam_r) AC_DEFINE(HAVE_GETSPNAM_R,1, Some systems have getspnam_r) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getspnam_r with 4 parameters)
AC_TRY_LINK([#include <shadow.h>
#include <stdlib.h>],
getspnam_r(NULL,NULL,NULL,0);,INCLUDE_SHADOWENTRY="1"; AC_DEFINE(HAVE_GETSPNAM_R_4,1,Some systems have getspnam_r) AC_DEFINE(HAVE_GETSPNAM_R,1, Some systems have getspnam_r) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getspnam)
AC_TRY_LINK([#include <shadow.h>
#include <stdlib.h>],
getspnam(NULL);,INCLUDE_SHADOWENTRY="1"; AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

if ( test "$INCLUDE_SHADOWENTRY" = "1" )
then

AC_MSG_CHECKING(spwd has sp_warn)
AC_TRY_LINK([#include <shadow.h>
#include <stdlib.h>],
struct spwd sp; sp.sp_warn=0;,AC_DEFINE(HAVE_SP_WARN,1,struct spwd has sp_warn) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(spwd has sp_inact)
AC_TRY_LINK([#include <shadow.h>
#include <stdlib.h>],
struct spwd sp; sp.sp_inact=0;,AC_DEFINE(HAVE_SP_INACT,1,struct spwd has sp_inact)
AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(spwd has sp_expire)
AC_TRY_LINK([#include <shadow.h>
#include <stdlib.h>],
struct spwd sp; sp.sp_expire=0;,AC_DEFINE(HAVE_SP_EXPIRE,1,struct spwd has sp_expire) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(spwd has sp_flag)
AC_TRY_LINK([#include <shadow.h>
#include <stdlib.h>],
struct spwd sp; sp.sp_flag=0;,AC_DEFINE(HAVE_SP_FLAG,1,struct spwd has sp_flag) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

fi 

AC_SUBST(INCLUDE_SHADOWENTRY)
])


dnl checks for statfs/statvfs capibilities
AC_DEFUN([FW_STATFS],
[

STATFS_STYLE="unknown"

AC_MSG_CHECKING(statfs/statvfs)

AC_TRY_LINK([#include <sys/vfs.h>],
[struct statfs sfs;
sfs.f_type=0;
sfs.f_bsize=0;
sfs.f_blocks=0;
sfs.f_bfree=0;
sfs.f_bavail=0;
sfs.f_files=0;
sfs.f_ffree=0;
sfs.f_fsid.__val[0]=0;
sfs.f_namelen=0;
statfs("/",&sfs);]
,AC_DEFINE(HAVE_LINUX_STATFS,1,Linux style statfs) STATFS_STYLE="linux style")

dnl cygwin is like linux but f_fsid is just a long
if ( test "$STATFS_STYLE" = "unknown" )
then
AC_TRY_LINK([#include <sys/vfs.h>],
[struct statfs sfs;
sfs.f_type=0;
sfs.f_bsize=0;
sfs.f_blocks=0;
sfs.f_bfree=0;
sfs.f_bavail=0;
sfs.f_files=0;
sfs.f_ffree=0;
sfs.f_fsid=0;
sfs.f_namelen=0;
statfs("/",&sfs);]
,AC_DEFINE(HAVE_CYGWIN_STATFS,1,Cygwin style statfs) STATFS_STYLE="cygwin style")
fi

dnl freebsd is very different from linux
if ( test "$STATFS_STYLE" = "unknown" )
then
AC_TRY_LINK([#include <sys/param.h>
#include <sys/mount.h>],
[struct statfs sfs;
sfs.f_bsize=0;
sfs.f_iosize=0;
sfs.f_blocks=0;
sfs.f_bfree=0;
sfs.f_bavail=0;
sfs.f_files=0;
sfs.f_ffree=0;
sfs.f_fsid.val[0]=0;
sfs.f_owner=0;
sfs.f_type=0;
sfs.f_flags=0;
sfs.f_syncwrites=0;
sfs.f_asyncwrites=0;
sfs.f_fstypename[0]=0;
sfs.f_mntonname[0]=0;
sfs.f_syncreads=0;
sfs.f_asyncreads=0;
sfs.f_mntfromname[0]=0;
statfs("/",&sfs);]
,AC_DEFINE(HAVE_FREEBSD_STATFS,1,FreeBSD style statfs) STATFS_STYLE="freebsd style")
fi

dnl netbsd is like freebsd but lacks a few fields
if ( test "$STATFS_STYLE" = "unknown" )
then
AC_TRY_LINK([#include <sys/param.h>
#include <sys/mount.h>],
[struct statfs sfs;
sfs.f_type=0;
sfs.f_bsize=0;
sfs.f_iosize=0;
sfs.f_blocks=0;
sfs.f_bfree=0;
sfs.f_bavail=0;
sfs.f_files=0;
sfs.f_ffree=0;
sfs.f_fsid.val[0]=0;
sfs.f_owner=0;
sfs.f_flags=0;
sfs.f_syncwrites=0;
sfs.f_asyncwrites=0;
sfs.f_fstypename[0]=0;
sfs.f_mntonname[0]=0;
sfs.f_mntfromname[0]=0;
statfs("/",&sfs);]
,AC_DEFINE(HAVE_NETBSD_STATFS,1,NetBSD style statfs) STATFS_STYLE="netbsd style")
fi

dnl openbsd is like netbsd but with an additional mount_info union and without
dnl an f_type field
if ( test "$STATFS_STYLE" = "unknown" )
then
AC_TRY_LINK([#include <sys/param.h>
#include <sys/mount.h>],
[struct statfs sfs;
sfs.f_flags=0;
sfs.f_bsize=0;
sfs.f_iosize=0;
sfs.f_blocks=0;
sfs.f_bfree=0;
sfs.f_bavail=0;
sfs.f_files=0;
sfs.f_ffree=0;
sfs.f_fsid.val[0]=0;
sfs.f_owner=0;
sfs.f_syncwrites=0;
sfs.f_asyncwrites=0;
sfs.f_fstypename[0]=0;
sfs.f_mntonname[0]=0;
sfs.f_mntfromname[0]=0;
sfs.mount_info.ufs_args.fspec=NULL;
statfs("/",&sfs);]
,AC_DEFINE(HAVE_OPENBSD_STATFS,1,OpenBSD style statfs) STATFS_STYLE="openbsd style")
fi

dnl darwin is similar to the other bsd's but with a few extra and a few missing
dnl fields
if ( test "$STATFS_STYLE" = "unknown" )
then
AC_TRY_LINK([#include <sys/param.h>
#include <sys/mount.h>],
[struct statfs sfs;
sfs.f_otype=0;
sfs.f_oflags=0;
sfs.f_bsize=0;
sfs.f_iosize=0;
sfs.f_blocks=0;
sfs.f_bfree=0;
sfs.f_bavail=0;
sfs.f_files=0;
sfs.f_ffree=0;
sfs.f_fsid.val[0]=0;
sfs.f_owner=0;
sfs.f_type=0;
sfs.f_flags=0;
sfs.f_fstypename[0]=0;
sfs.f_mntonname[0]=0;
sfs.f_mntfromname[0]=0;
statfs("/",&sfs);]
,AC_DEFINE(HAVE_DARWIN_STATFS,1,Darwin style statfs) STATFS_STYLE="darwin style")
fi

dnl SCO and Solaris both have statvfs
if ( test "$STATFS_STYLE" = "unknown" )
then
AC_TRY_LINK([#include <sys/types.h>
#include <sys/statvfs.h>],
[struct statvfs sfs;
sfs.f_bsize=0;
sfs.f_frsize=0;
sfs.f_blocks=0;
sfs.f_bfree=0;
sfs.f_bavail=0;
sfs.f_files=0;
sfs.f_ffree=0;
sfs.f_favail=0;
sfs.f_fsid=0;
sfs.f_basetype[0]=0;
sfs.f_flag=0;
sfs.f_fstr[0]=0;
statvfs("/",&sfs);]
,AC_DEFINE(HAVE_STATVFS,1,statvfs) STATFS_STYLE="statvfs")
fi

AC_MSG_RESULT($STATFS_STYLE)

])


dnl checks for __xnet_*
AC_DEFUN([FW_CHECK_XNET_PROTOTYPES],
[
	if ( test -n "$XNETLIB" )
	then
		AC_MSG_CHECKING(for sendmsg prototype)
		AC_TRY_COMPILE([#include <sys/types.h>
#include <sys/socket.h>],
sendmsg(0,NULL,0);,AC_MSG_RESULT(yes), AC_DEFINE(NEED_XNET_PROTOTYPES, 1, Solaris 2.6 has a few missing function prototypes) AC_MSG_RESULT(no))
	fi
])

dnl check to see if nanosleep is available
AC_DEFUN([FW_CHECK_NANOSLEEP],
[

	AC_MSG_CHECKING(nanosleep)

	DONE=""
	for i in "" "-lrt"
	do
		FW_TRY_LINK([#include <stdlib.h>
#include <time.h>],[nanosleep(NULL,NULL);],[],[$i],[],[NANOSLEEPLIB="$i"; DONE="yes"],[])
		if ( test -n "$DONE" )
		then
			break
		fi
	done

	if ( test -n "$DONE" )
	then
		AC_DEFINE(HAVE_NANOSLEEP,1,Some systems have nanosleep)
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
	fi

	AC_SUBST(NANOSLEEPLIB)
])

dnl check to see which should be used of -lsocket, -lnsl and -lxnet
AC_DEFUN([FW_CHECK_SOCKET_LIBS],
[

	AC_MSG_CHECKING(for socket libraries)

	AC_LANG(C)
	SOCKETLIBS=""
	DONE=""
	for i in "" "-lnsl" "-lsocket" "-lsocket -lnsl" "-lxnet"
	do
		FW_TRY_LINK([#include <stdlib.h>],[connect(0,NULL,0); listen(0,0); bind(0,NULL,0); accept(0,NULL,0); send(0,NULL,0,0); sendto(0,NULL,0,0,NULL,0); sendmsg(0,NULL,0); gethostbyname(NULL);],[],[$i],[],[SOCKETLIBS="$i"; DONE="yes"],[])
		if ( test -n "$DONE" )
		then
			break
		fi
	done
	AC_LANG(C++)

	if ( test -z "$DONE" )
	then
		AC_MSG_ERROR(no combination of networking libraries was found.)
	fi

	if ( test -z "$SOCKETLIBS" )
	then
		AC_MSG_RESULT(none)
	else
		AC_MSG_RESULT($SOCKETLIBS)
	fi

	AC_SUBST(SOCKETLIBS)
])

AC_DEFUN([FW_CHECK_SCO_HACK],
[
	AC_LANG(C)
	LINKOK=""
	AC_TRY_LINK([#include <stdlib.h>],exit(0);,LINKOK="yes")
	if ( test -z "$LINKOK" )
	then
		OLDLIBS="$LIBS"
		LIBS="/usr/ccs/lib/crti.o $LIBS"
		AC_TRY_LINK([#include <stdlib.h>],exit(0);,LINKOK="yes")
		if ( test -n "$LINKOK" )
		then
			HACKLIBS="$HACKLIBS /usr/ccs/lib/crti.o"
		fi
		LIBS="$OLDLIBS"
	fi
])

AC_DEFUN([FW_CHECK_HACKS],
[
HACKLIBS=""
FW_CHECK_SCO_HACK
LIBS="$HACKLIBS $LIBS"
])


AC_DEFUN([FW_CHECK_CRYPT_R],
[

	AC_MSG_CHECKING(crypt)
	HAVE_CRYPT=""
	for i in "" "-lcrypt"
	do
		FW_TRY_LINK([#include <stdlib.h>
#include <crypt.h>],[crypt(NULL,NULL);],[],[$i],[],[HAVE_CRYPT="yes"; CRYPTLIB="$i"; AC_DEFINE(HAVE_CRYPT,1,Some systems have crypt)],[])
		if ( test -n "$HAVE_CRYPT" )
		then
			break
		fi
	done
	if ( test -n "$HAVE_CRYPT" )
	then
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
	fi

	AC_MSG_CHECKING(crypt_r)
	HAVE_CRYPT_R=""
	for i in "" "-lcrypt"
	do
		FW_TRY_LINK([#include <stdlib.h>
#include <crypt.h>],[crypt_data cd; crypt_r(NULL,NULL,NULL);],[],[$i],[],[HAVE_CRYPT_R="yes"; CRYPTLIB="$i"; AC_DEFINE(HAVE_CRYPT_R,1,Some systems have crypt_r)],[])
		if ( test -n "$HAVE_CRYPT_R" )
		then
			break
		fi
	done
	if ( test -n "$HAVE_CRYPT_R" )
	then
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
	fi

	INCLUDE_CRYPTLIB="0"
	if ( test -n "$HAVE_CRYPT" -o -n "$HAVE_CRYPT_R" )
	then
		INCLUDE_CRYPT="1"
	fi

	AC_SUBST(INCLUDE_CRYPT)
	AC_SUBST(CRYPTLIB)
])
