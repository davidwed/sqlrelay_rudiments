AC_DEFUN([FW_TRY_LINK],
[
SAVECPPFLAGS="$CPPFLAGS"
SAVELIBS="$LIBS"
SAVE_LD_LIBRARY_PATH="$LD_LIBRARY_PATH"
CPPFLAGS="$3"
LIBS="$4"
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
AC_CHECK_FILE($1, FOUNDLIB="yes")
if ( test -n "$FOUNDLIB" )
then
	eval "$2"
else
	if ( test -n "$3" )
	then
		AC_CHECK_FILE($3, FOUNDLIB="yes")
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
AC_CHECK_FILE($1, FOUNDHEADER="yes")
AC_CHECK_FILE($3, FOUNDLIB="yes")
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
		AC_CHECK_FILE($5, FOUNDLIB="yes")
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

FW_CHECK_HEADER_LIB([/usr/include/$HEADER],[INCLUDESTRING=\"\"],[/usr/lib/lib$LIBNAME.so],[LIBPATH=\"\"; LIBSTRING=\"-l$LIBNAME\"],[/usr/lib/lib$LIBNAME.a],[LIBSTRING=\"-l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
if ( test -z "$LIBSTRING" )
then
	FW_CHECK_HEADER_LIB([/usr/include/$NAME/$HEADER],[INCLUDESTRING=\"-I/usr/include/$NAME\"],[/usr/lib/lib$LIBNAME.so],[LIBPATH=\"\"; LIBSTRING=\"-l$LIBNAME\"],[/usr/lib/lib$LIBNAME.a],[LIBSTRING=\"-l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
fi
if ( test -z "$LIBSTRING" )
then
	FW_CHECK_HEADER_LIB([/usr/include/$HEADER],[INCLUDESTRING=\"\"],[/usr/lib/$NAME/lib$LIBNAME.so],[LIBPATH=\"/usr/lib/$NAME\"; LIBSTRING=\"-L/usr/lib/$NAME -l$LIBNAME\"],[/usr/lib/$NAME/lib$LIBNAME.a],[LIBSTRING=\"-L/usr/lib/$NAME -l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
fi
if ( test -z "$LIBSTRING" )
then
	FW_CHECK_HEADER_LIB([/usr/include/$NAME/$HEADER],[INCLUDESTRING=\"-I/usr/include/$NAME\"],[/usr/lib/$NAME/lib$LIBNAME.so],[LIBPATH=\"/usr/lib/$NAME\"; LIBSTRING=\"-L/usr/lib/$NAME -l$LIBNAME\"],[/usr/lib/$NAME/lib$LIBNAME.a],[LIBSTRING=\"-L/usr/lib/$NAME -l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
fi

if ( test -z "$LIBSTRING" )
then

	for paths in "$SEARCHPATH" "/usr/local/$NAME" "/opt/$NAME" "/usr/$NAME" "/usr/local" "/usr/pkg" "/opt/sfw" "/usr/local/firstworks"
	do
		if ( test -n "$paths" )
		then
			FW_CHECK_HEADER_LIB([$paths/include/$HEADER],[INCLUDESTRING=\"-I$paths/include\"],[$paths/lib/lib$LIBNAME.so],[LIBPATH=\"$paths/lib\"; LIBSTRING=\"-L$paths/lib -l$LIBNAME\"],[$paths/lib/lib$LIBNAME.a],[LIBSTRING=\"-L$paths/lib -l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
			if ( test -z "$LIBSTRING" )
			then
				FW_CHECK_HEADER_LIB([$paths/include/$NAME/$HEADER],[INCLUDESTRING=\"-I$paths/include/$NAME\"],[$paths/lib/lib$LIBNAME.so],[LIBPATH=\"$paths/lib\"; LIBSTRING=\"-L$paths/lib -l$LIBNAME\"],[$paths/lib/lib$LIBNAME.a],[LIBSTRING=\"-L$paths/lib -l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
			fi
			if ( test -z "$LIBSTRING" )
			then
				FW_CHECK_HEADER_LIB([$paths/include/$HEADER],[INCLUDESTRING=\"-I$paths/include\"],[$paths/lib/$NAME/lib$LIBNAME.so],[LIBPATH=\"$paths/lib/$NAME\"; LIBSTRING=\"-L$paths/lib/$NAME -l$LIBNAME\"],[$paths/lib/$NAME/lib$LIBNAME.a],[LIBSTRING=\"-L$paths/lib/$NAME -l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
			fi
			if ( test -z "$LIBSTRING" )
			then
				FW_CHECK_HEADER_LIB([$paths/include/$NAME/$HEADER],[INCLUDESTRING=\"-I$paths/include/$NAME\"],[$paths/lib/$NAME/lib$LIBNAME.so],[LIBPATH=\"$paths/lib/$NAME\"; LIBSTRING=\"-L$paths/lib/$NAME -l$LIBNAME\"],[$paths/lib/$NAME/lib$LIBNAME.a],[LIBSTRING=\"-L$paths/lib/$NAME -l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
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
dnl requires:  PTHREADSPATH, RPATHFLAG, cross_compiling
dnl sets the substitution variable PTHREADSLIB
dnl sets the environment variable PTHREADSLIBPATH
AC_DEFUN([FW_CHECK_PTHREADS],
[

HAVE_PTHREADS=""
PTHREADSINCLUDES=""
PTHREADSLIB=""

if ( test "$cross_compiling" = "yes" )
then
	
	dnl cross compiling
	echo "cross compiling"
	if ( test -n "$PTHREADSPATH" )
	then
		PTHREADSINCLUDES="-I$PTHREADSPATH/include"
		PTHREADSLIB="-L$PTHREADSPATH/lib -lpthread"
	else
		PTHREADSLIB="-lpthread"
	fi
	HAVE_PTHREADS="yes"

else

	for i in "pthread" "pthreads" "pth" "gthread" "gthreads"
	do
		for j in "pthread" "pth" "c_r" "gthreads"
		do
			FW_CHECK_HEADERS_AND_LIBS([$PTHREADSPATH],[$i],[pthread.h],[$j],[""],[""],[PTHREADSINCLUDES],[PTHREADSLIB],[PTHREADSLIBPATH],[PTHREADSTATIC])
			if ( test -n "$PTHREADSLIB" )
			then
				break
			fi
		done
		if ( test -n "$PTHREADSLIB" )
		then
			HAVE_PTHREADS="yes"
			break
		fi
	done
fi
AC_SUBST(PTHREADSINCLUDES)
AC_SUBST(PTHREADSLIB)
if ( test -z "$HAVE_PTHREADS" )
then
	AC_MSG_ERROR(pthreads not found.  Rudiments requires this package.)
	exit
fi
])

dnl checks for rpc entry functions and header files
AC_DEFUN([FW_CHECK_RPC],
[

HAVE_GETRPCBYNAME_R="no"
AC_MSG_CHECKING(getrpcbyname_r in netdb.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>],
getrpcbyname_r(NULL,NULL,NULL,0,NULL);,HAVEGETRPCBYNAME_R="yes"; AC_DEFINE(HAVE_GETRPCBYNAME_R,1, Some systems have getrpcbyname_r) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getrpcbyname_r in rpc/rpcent.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpcent.h>],
getrpcbyname_r(NULL,NULL,NULL,0,NULL);,HAVEGETRPCBYNAME_R="yes"; AC_DEFINE(HAVE_GETRPCBYNAME_R,1, Some systems have getrpcbyname_r) AC_DEFINE(HAVE_RPCENT_H,1, Some systems have rpc/rpcent.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getrpcbyname_r in rpc/rpc.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpc.h>],
getrpcbyname_r(NULL,NULL,NULL,0,NULL);,HAVEGETRPCBYNAME_R="yes"; AC_DEFINE(HAVE_GETRPCBYNAME_R,1, Some systems have getrpcbyname_r) AC_DEFINE(HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))


AC_MSG_CHECKING(getrpcbyname in netdb)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>],
getrpcbyname(NULL);,AC_DEFINE(HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getrpcbyname in rpc/rpcent.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpcent.h>],
getrpcbyname(NULL);,AC_DEFINE(HAVE_RPCENT_H,1, Some systems have rpc/rpcent.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getrpcbyname in rpc/rpc.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpc.h>],
getrpcbyname(NULL);,AC_DEFINE(HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))




HAVEGETRPCBYNUMBER_R="no"
AC_MSG_CHECKING(getrpcbynumber_r in netdb.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>],
getrpcbynumber_r(0,NULL,NULL,0,NULL);,HAVEGETRPCBYNUMBER_R="yes"; AC_DEFINE(HAVE_GETRPCBYNUMBER_R,1, Some systems have getrpcbynumber_r) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getrpcbynumber_r in rpc/rpcent.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpcent.h>],
getrpcbynumber_r(0,NULL,NULL,0,NULL);,HAVEGETRPCBYNUMBER_R="yes"; AC_DEFINE(HAVE_GETRPCBYNUMBER_R,1, Some systems have getrpcbynumber_r) AC_DEFINE(HAVE_RPCENT_H,1, Some systems have rpc/rpcent.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getrpcbynumber_r in rpc/rpc.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpc.h>],
getrpcbynumber_r(0,NULL,NULL,0,NULL);,HAVEGETRPCBYNUMBER_R="yes"; AC_DEFINE(HAVE_GETRPCBYNUMBER_R,1, Some systems have getrpcbynumber_r) AC_DEFINE(HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))


AC_MSG_CHECKING(getrpcbynumber in netdb)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>],
getrpcbynumber(0);,AC_DEFINE(HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getrpcbynumber in rpc/rpcent.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpcent.h>],
getrpcbynumber(0);,AC_DEFINE(HAVE_RPCENT_H,1, Some systems have rpc/rpcent.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(getrpcbynumber in rpc/rpc.h)
AC_TRY_LINK([#include <netdb.h>
#include <stdlib.h>
#include <rpc/rpc.h>],
getrpcbynumber(0);,AC_DEFINE(HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))
])
