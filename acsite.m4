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
LIBS="$4"
LD_LIBRARY_PATH="$5"
export LD_LIBRARY_PATH
AC_TRY_LINK([$1],[$2],[$6],[$7])
CPPFLAGS="$SAVECPPFLAGS"
LIBS="$SAVELIBS"
LD_LIBRARY_PATH="$SAVE_LD_LIBRARY_PATH"
export LD_LIBRARY_PATH
])

AC_DEFUN([FW_TRY_COMPILE],
[
SAVECPPFLAGS="$CPPFLAGS"
CPPFLAGS="$3"
AC_TRY_COMPILE([$1],[$2],[$4],[$5])
CPPFLAGS="$SAVECPPFLAGS"
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
FW_CHECK_FILE([$1],[FOUNDHEADER=\"yes\"])
FW_CHECK_FILE([$3],[FOUNDLIB=\"yes\"])
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
		FW_CHECK_FILE([$5],[FOUNDLIB=\"yes\"])
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

SEARCHPATH=$1
NAME=$2
HEADER=$3
LIBNAME=$4
LINKSTATIC=$5
LINKRPATH=$6
USEFULLLIBPATH=$12
INCLUDESTRING=""
LIBSTRING=""
LIBPATH=""
STATIC=""
HEADERSANDLIBSPATH=""

eval "$7=\"\""
eval "$8=\"\""
eval "$9=\"\""
eval "$10=\"\""
if ( test -n "$11" )
then
	eval "$11=\"\""
fi


for path in "$SEARCHPATH" "/" "/usr" "/usr/local/$NAME" "/opt/$NAME" "/usr/$NAME" "/usr/local" "/usr/pkg" "/usr/pkg/$NAME" "/opt/sfw" "/opt/sfw/$NAME" "/usr/sfw" "/usr/sfw/$NAME" "/opt/csw" "/sw" "/boot/common" "/Library/$NAME" "/usr/local/firstworks"
do
	if ( test -n "$path" -a -d "$path" )
	then

		if ( test "$path" = "/" )
		then
			dnl look in /usr/include and /$LIBDIR
			if ( test "$USEFULLLIBPATH" = "yes" )
			then
				FW_CHECK_HEADER_LIB([/usr/include/$HEADER],[],[/$LIBDIR/lib$LIBNAME.$SOSUFFIX],[LIBPATH=\"/$LIBDIR\"; LIBSTRING=\"-Wl,/$LIBDIR/lib$LIBNAME.$SOSUFFIX\"],[/$LIBDIR/lib$LIBNAME.a],[LIBSTRING=\"/$LIBDIR/lib$LIBNAME.a\"; STATIC=\"$LINKSTATIC\"])
			else
				FW_CHECK_HEADER_LIB([/usr/include/$HEADER],[],[/$LIBDIR/lib$LIBNAME.$SOSUFFIX],[LIBPATH=\"/$LIBDIR\"; LIBSTRING=\"-l$LIBNAME\"],[/$LIBDIR/lib$LIBNAME.a],[LIBSTRING=\"-l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
			fi

			dnl set path to "" so we won't get //'s from here on
			path=""
		fi


		for libpath in "$path/$LIBDIR" "$path/$LIBDIR/$NAME" "$path/$LIBDIR/opt"
		do

			if ( test -n "$LIBSTRING" )
			then
				break
			fi

			for includepath in "$path/include" "$path/include/$NAME"
			do

				if ( test -n "$LIBSTRING" )
				then
					break
				fi

				dnl look in $path/$LIBDIR
				if ( test "$USEFULLLIBPATH" = "yes" )
				then
					FW_CHECK_HEADER_LIB([$includepath/$HEADER],[INCLUDESTRING=\"-I$includepath\"],[$libpath/lib$LIBNAME.$SOSUFFIX],[LIBPATH=\"$libpath\"; LIBSTRING=\"-Wl,$libpath/lib$LIBNAME.$SOSUFFIX\"],[$libpath/lib$LIBNAME.a],[LIBSTRING=\"$libpath/lib$LIBNAME.a\"; STATIC=\"$LINKSTATIC\"])
				else
					FW_CHECK_HEADER_LIB([$includepath/$HEADER],[INCLUDESTRING=\"-I$includepath\"],[$libpath/lib$LIBNAME.$SOSUFFIX],[LIBPATH=\"$libpath\"; LIBSTRING=\"-L$libpath -l$LIBNAME\"],[$libpath/lib$LIBNAME.a],[LIBSTRING=\"-L$libpath -l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
				fi
			done
		done

		if ( test -n "$LIBSTRING" )
		then
			HEADERSANDLIBSPATH="$path"
			break
		fi
	fi
done

dnl remove -I/usr/include, -L/lib, -L/usr/lib, -L/lib64 and -L/usr/lib64
INCLUDESTRING=`echo $INCLUDESTRING | sed -e "s|-I/usr/include$||g" -e "s|-I/usr/include ||g"`
LIBSTRING=`echo $LIBSTRING | sed -e "s|-L/usr/lib$||g" -e "s|-L/lib$||g" -e "s|-L/usr/lib ||g" -e "s|-L/lib ||g"`
LIBSTRING=`echo $LIBSTRING | sed -e "s|-L/usr/lib64$||g" -e "s|-L/lib64$||g" -e "s|-L/usr/lib64 ||g" -e "s|-L/lib64 ||g"`

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



dnl checks to see if -Wno-long-double option to gcc works or not
AC_DEFUN([FW_CHECK_WNOLONGDOUBLE],
[
AC_MSG_CHECKING(for -Wno-long-double option)
FW_TRY_LINK([#include <stdio.h>],[printf("hello");],[-Wno-long-double],[],[],[WNOLONGDOUBLE="-Wno-long-double"],[WNOLONGDOUBLE=""])
if ( test -n "$WNOLONGDOUBLE" )
then
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
fi
AC_SUBST(WNOLONGDOUBLE)
])



dnl checks to see if -Wall option works or not
AC_DEFUN([FW_CHECK_WALL],
[
AC_MSG_CHECKING(for -Wall)
FW_TRY_LINK([#include <stdio.h>],[printf("hello");],[-Wall],[],[],[WALL="-Wall"],[WALL=""])
if ( test -n "$WALL" )
then
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
fi
AC_SUBST(WALL)
])



dnl checks to see if -Werror option works or not
AC_DEFUN([FW_CHECK_WERROR],
[
AC_MSG_CHECKING(for -Werror)
FW_TRY_LINK([#include <stdio.h>],[printf("hello");],[-Werror],[],[],[WERROR="-Werror"],[WERROR=""])

dnl disable -Werror on Haiku, and Minix as their header files throw warnings
case $host_os in
	*haiku* )
		WERROR=""
		;;
	*minix* )
		WERROR=""
		;;
	*)
		;;
esac
AC_SUBST(WERROR)

if ( test -n "$WERROR" )
then
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
fi
])



dnl checks to see if -g3 option works or not
AC_DEFUN([FW_CHECK_DEBUG],
[
if ( test "$DEBUG" = "yes" )
then
	AC_MSG_CHECKING(for -g3)
	FW_TRY_LINK([#include <stdio.h>],[printf("hello");],[-g3],[],[],[DBG="-g3"],[DBG="-g"])
	if ( test "$DBG" = "-g3" )
	then
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
	fi
	CXXFLAGS="$CXXFLAGS $DBG"
fi
])


dnl Determines what extension shared object files have
AC_DEFUN([FW_CHECK_SO_EXT],
[
AC_MSG_CHECKING(for dynamic library extension)
SOSUFFIX="so"
if ( test -n "$CYGWIN" )
then
	SOSUFFIX="dll.a"
fi
if ( test -n "$DARWIN" )
then
	SOSUFFIX="dylib"
fi
AC_MSG_RESULT($SOSUFFIX)
])

AC_DEFUN([FW_CHECK_LIBDIR],
[
AC_MSG_CHECKING(for library directory)
case $host_cpu in
	x86_64 ) LIBDIR="lib64" ;;
	* ) LIBDIR="lib" ;;
esac
AC_MSG_RESULT($LIBDIR)
])


dnl Checks for microsoft platform.
dnl sets the substitution variables MINGW32, CYGWIN and UWIN as appropriate
dnl sets the enviroment variable MICROSOFT
AC_DEFUN([FW_CHECK_MICROSOFT],
[
AC_MSG_CHECKING(for microsoft platform)
CYGWIN=""
MINGW32=""
UWIN=""
case $host_os in
	*cygwin* )
		CYGWIN="yes"
		AC_MSG_RESULT(cygwin)
		;;
	*mingw32* )
		MINGW32="yes"
		AC_MSG_RESULT(mingw32)
		;;
	*uwin* )
		UWIN="yes"
		AC_MSG_RESULT(uwin)
		;;
	* )
		AC_MSG_RESULT(no)
		;;
esac
EXE=""
AC_SUBST(MINGW32)
AC_SUBST(CYGWIN)
AC_SUBST(UWIN)

MICROSOFT=""
if ( test "$UWIN" = "yes" -o "$MINGW32" = "yes" -o "$CYGWIN" = "yes" )
then
	#CPPFLAGS="$CPPFLAGS -mno-win32"
	MICROSOFT="yes"
	EXE=".exe"
fi

AC_SUBST(EXE)
AC_SUBST(MICROSOFT)

if ( test "$MINGW32" )
then
	AC_DEFINE(MINGW32,1,Mingw32 environment)

	dnl if we're building mingw32, we're cross-compiling by definition
	cross_compiling="yes"
fi
])


AC_DEFUN([FW_CHECK_OSX],
[
DARWIN=""
AC_MSG_CHECKING(for OSX)
case $host_os in
	*darwin* )
		DARWIN="yes"
		AC_MSG_RESULT(yes)
		;;
	* )
		AC_MSG_RESULT(no)
		;;
esac
])

dnl Checks for minix and adds some macros if it is
AC_DEFUN([FW_CHECK_MINIX],
[
AC_MSG_CHECKING(for minix)
case $host_os in
	*minix* )
		CPPFLAGS="$CPPFLAGS -D_MINIX -D_POSIX_SOURCE -D_NETBSD_SOURCE -D_XOPEN_SOURCE -D_XOPEN_SOURCE_EXTENDED"
		AC_MSG_RESULT(yes)
		;;
	* )
		AC_MSG_RESULT(no)
		;;
esac
])

dnl Checks for haiku and adds some macros if it is
AC_DEFUN([FW_CHECK_HAIKU],
[
AC_MSG_CHECKING(for haiku)
BELIB=""
case $host_os in
	*haiku* )
		if ( test "$prefix" = "NONE" )
		then
			prefix="/boot/common"
		fi
		BELIB="-lbe"
		AC_MSG_RESULT(yes)
		;;
	* )
		AC_MSG_RESULT(no)
		;;
esac
AC_SUBST(BELIB)
])

AC_DEFUN([FW_CHECK_SCO_OSR5],
[
AC_MSG_CHECKING(for SCO OSR < 6.0.0)
if ( test "`uname -s`" = "SCO_SV" )
then
  	AC_DEFINE(RUDIMENTS_HAVE_SCO_AVENRUN,1,SCO has /dev/table/avenrun instead of getloadavg)
	if ( test "`uname -v | tr -d '.'`" -lt "600" )
	then
  		AC_DEFINE(RUDIMENTS_HAVE_BAD_SCO_MSGHDR,1,SCO OSR5 has an incorrect struct msghdr definition)
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
	fi
else
	AC_MSG_RESULT(no)
fi
])


dnl checks if the compiler supports the inline keyword
dnl defines the macro INLINE
AC_DEFUN([FW_CHECK_INLINE],
[
AC_MSG_CHECKING(for inline)
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
AC_DEFINE_UNQUOTED(INLINE,$INLINE,Some compliers dont support the inline keyword)
])

AC_DEFUN([FW_CXX_NAMESPACES],
[
	RUDIMENTS_NAMESPACE=""
	AC_MSG_CHECKING(for namespace support)
	AC_TRY_COMPILE([namespace Outer { namespace Inner { int i = 0; }}],[using namespace Outer::Inner; return i;],[RUDIMENTS_NAMESPACE="yes"],[])
	if ( test "$RUDIMENTS_NAMESPACE" = yes )
	then
		AC_MSG_RESULT(yes)
  		AC_DEFINE(RUDIMENTS_NAMESPACE,1,Compiler supports namespaces)
	else
		AC_MSG_RESULT(no)
	fi
])


dnl checks for the pthreads library
dnl requires:  PTHREADPATH, RPATHFLAG, cross_compiling
dnl sets the substitution variable PTHREADLIB
AC_DEFUN([FW_CHECK_PTHREAD],
[

HAS_THREADS="no"
if ( test "$ENABLE_RUDIMENTS_THREADS" = "yes" )
then

	AC_MSG_CHECKING(if -pthread works during compile phase)
	if ( test -n "`$CXX -pthread 2>&1 | grep 'unrecognized option' | grep pthread`" )
	then
		PTHREAD_COMPILE=""
	else
		PTHREAD_COMPILE="-pthread"
	fi
	if ( test -n "$PTHREAD_COMPILE" )
	then
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
	fi


	HAVE_PTHREAD=""
	PTHREADINCLUDES=""
	PTHREADLIB=""

	if ( test "$cross_compiling" = "yes" )
	then

		dnl cross compiling
		echo "cross compiling"

		if ( test -n "$PTHREADPATH" )
		then
			PTHREADINCLUDES="$PTHREAD_COMPILE -I$PTHREADPATH/include"
			PTHREADLIB="-L$PTHREADPATH/lib -lpthread -pthread"
		else
			PTHREADINCLUDES="$PTHREAD_COMPILE"
			PTHREADLIB="-lpthread -pthread"
		fi
		HAVE_PTHREAD="yes"

	else

		dnl check pthread.h and standard thread libraries
		for i in "pthread" "c_r" "thread" "pthreads" "gthreads" ""
		do
			if ( test -n "$i" )
			then
				AC_MSG_CHECKING(for lib$i)
			else
				AC_MSG_CHECKING(for no library)
			fi

			INCLUDEDIR="pthread"
			if ( test "$i" = "gthreads" )
			then
				INCLUDEDIR="FSU"
			fi

			if ( test -n "$i" )
			then
				FW_CHECK_HEADERS_AND_LIBS([$PTHREADPATH],[$INCLUDEDIR],[pthread.h],[$i],[""],[""],[PTHREADINCLUDES],[PTHREADLIB],[PTHREADLIBPATH],[PTHREADSTATIC])
			fi

			if ( test -n "$PTHREADLIB" -o -z "$i" )
			then

				AC_MSG_RESULT(yes)

				if ( test -n "$i" )
				then
					AC_MSG_CHECKING(whether lib$i works)
				else
					AC_MSG_CHECKING(whether no library works)
				fi

				dnl  If we found a set of headers and libs, try
				dnl  linking with them.  We'll try six times,
				dnl  first with just the header and lib that we
				dnl  found, then with -pthread added to one,
				dnl  the other and both, and then finally
				dnl  without any libs, just -pthread
				for try in 1 2 3 4 5 6
				do

					if ( test "$try" = "1" )
					then
						TESTINCLUDES="$PTHREADINCLUDES"
						TESTLIB="$PTHREADLIB"
					elif ( test "$try" = "2" )
					then
						TESTINCLUDES="$PTHREADINCLUDES"
						TESTLIB="$PTHREADLIB -pthread"
					elif ( test "$try" = "3" )
					then
						TESTINCLUDES="$PTHREAD_COMPILE $PTHREADINCLUDES"
						TESTLIB="$PTHREADLIB"
					elif ( test "$try" = "4" )
					then
						TESTINCLUDES="$PTHREAD_COMPILE $PTHREADINCLUDESS"
						TESTLIB="$PTHREADLIB -pthread"
					elif ( test "$try" = "5" )
					then
						TESTINCLUDES="$PTHREADINCLUDESS"
						TESTLIB="-pthread"
					elif ( test "$try" = "6" )
					then
						TESTINCLUDES="$PTHREAD_COMPILE $PTHREADINCLUDESS"
						TESTLIB="-pthread"
					fi

					dnl try to link
					FW_TRY_LINK([#include <pthread.h>],[pthread_create(NULL,NULL,NULL,NULL);],[$CPPFLAGS $TESTINCLUDES],[$TESTLIB],[],[HAVE_PTHREAD="yes"],[])
					if ( test -z "$HAVE_PTHREAD" )
					then
						dnl try link again, some older
						dnl thread implementations have
						dnl non-pointer 2nd parameters
						FW_TRY_LINK([#include <pthread.h>],[pthread_create(NULL,pthread_attr_default,NULL,NULL);],[$CPPFLAGS $TESTINCLUDES],[$TESTLIB],[],[HAVE_PTHREAD="yes"],[])
					fi

					dnl  If the link succeeded then keep
					dnl  the flags.
					if ( test -n "$HAVE_PTHREAD" )
					then
						PTHREADINCLUDES="$TESTINCLUDES"
						PTHREADLIB="$TESTLIB"
						break
					fi

					dnl  If the link failed, reset the flags
					PTHREADINCLUDES=""
					PTHREADLIB=""
				done

				if ( test -n "$HAVE_PTHREAD" )
				then
					AC_MSG_RESULT(yes)
					break
				else
					AC_MSG_RESULT(no)
				fi

			else
				AC_MSG_RESULT(no)
			fi
		done
	fi

	FW_INCLUDES(pthreads,[$PTHREADINCLUDES])
	FW_LIBS(pthreads,[$PTHREADLIB])

	if ( test -z "$HAVE_PTHREAD" )
	then
		AC_MSG_ERROR(thread library not found.  Rudiments requires this package.)
		exit
	else
		AC_DEFINE(RUDIMENTS_HAS_THREADS,1,Rudiments supports threads)
	fi

	HAS_THREADS="yes"

else

	echo "disabled"

fi

AC_SUBST(PTHREADINCLUDES)
AC_SUBST(PTHREADLIB)
])


AC_DEFUN([FW_CHECK_MUTEX],
[
	if ( test $HAS_THREADS = "yes")
	then

		dnl check for pthread_mutex_t
		AC_MSG_CHECKING(for pthread_mutex_t)
		FW_TRY_LINK([#define _TIMESTRUC_T
#include <pthread.h>],[if (sizeof(pthread_mutex_t)) { return 0; } return 0;],[$CPPFLAGS $PTHREADINCLUDES],[$PTHREADLIB],[],[AC_DEFINE(RUDIMENTS_HAVE_PTHREAD_MUTEX_T,1,pthread_mutex_t type exists) AC_MSG_RESULT(yes)],[AC_MSG_RESULT(no)])

		dnl check for CreateMutex
		AC_MSG_CHECKING(for CreateMutex)
		FW_TRY_LINK([#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif],[HANDLE mut=CreateMutex(NULL,FALSE,NULL);],[$CPPFLAGS $PTHREADINCLUDES],[$PTHREADLIB],[],[AC_DEFINE(RUDIMENTS_HAVE_CREATE_MUTEX,1,CreateMutex function exists) AC_MSG_RESULT(yes)],[AC_MSG_RESULT(no)])
	fi
])


AC_DEFUN([FW_CHECK_PTHREAD_SIGMASK],
[
	dnl check for pthread_sigmask
	AC_MSG_CHECKING(for pthread_sigmask)
	FW_TRY_LINK([#include <signal.h>],[pthread_sigmask(1,0,0);],[$CPPFLAGS $PTHREADINCLUDES],[$PTHREADLIB],[],[AC_DEFINE(RUDIMENTS_HAVE_PTHREAD_SIGMASK,1,pthread_sigmask exists) AC_MSG_RESULT(yes)],[AC_MSG_RESULT(no)])
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

		AC_MSG_CHECKING(for openssl)
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
			FW_CHECK_HEADERS_AND_LIBS([/usr],[ssl],[openssl/ssl.h],[ssl],[""],[""],[OPENSSLINCLUDES],[OPENSSLLIBS],[SSLLIBPATH],[SSLSTATIC])
			FW_CHECK_HEADERS_AND_LIBS([/usr],[ssl],[openssl/crypto.h],[crypto],[""],[""],[CRYPTOINCLUDES],[CRYPTOLIBS],[CRYPTOLIBPATH],[CRYPTOSTATIC])
			if ( test -n "$OPENSSLINCLUDES" -a -n "$CRYPTOINCLUDES" )
			then
				SSLINCLUDES="$OPENSSLINCLUDES $CRYPTOINCLUDES"
			fi
			if ( test -n "$OPENSSLLIBS" -a -n "$CRYPTOLIBS" )
			then
				SSLLIBS="$OPENSSLLIBS $CRYPTOLIBS"
			fi
		fi

		if ( test -n "$SSLLIBS" )
		then
			AC_DEFINE(RUDIMENTS_HAS_SSL,1,Rudiments supports SSL)
			AC_MSG_RESULT(yes)

			AC_MSG_CHECKING(whether SSL_read/write can use a void * parameter)
			FW_TRY_LINK([#include <openssl/ssl.h>],[void *buf=0; SSL_read(NULL,buf,0);],[$CPPFLAGS $SSLINCLUDES],[$SSLLIBS],[],[SSL_VOID_PTR="yes"],[])
			if ( test -n "$SSL_VOID_PTR" )
			then
				AC_MSG_RESULT(yes)
				AC_DEFINE(RUDIMENTS_SSL_VOID_PTR,1,SSL_read/write can use a void * parameter instead of char *)
			else
				AC_MSG_RESULT(no)
			fi
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

		AC_MSG_CHECKING(for pcre)
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
			FW_TRY_LINK([#ifdef RUDIMENTS_HAVE_STDLIB_H
				#include <stdlib.h>
#endif
#include <pcre.h>],[pcre_extra *extra=pcre_study(NULL,0,NULL);],[$CPPFLAGS $PCREINCLUDES],[$PCRELIBS],[],[HAVE_PCRE="yes"; AC_DEFINE(RUDIMENTS_HAS_PCRE,1,Rudiments supports PCRE) AC_MSG_RESULT(yes)],[PCREINCLUDES=""; PCRELIBS=""; AC_MSG_RESULT(no)])
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

if ( test "$INCLUDE_RPCENTRY" = "1" )
then

HAVE_GETRPCBYNAME_R="no"
HAVE_GETRPCBYNAME="no"
HAVE_GETRPCBYNUMBER_R="no"
HAVE_GETRPCBYNUMBER="no"

AC_MSG_CHECKING(for getrpcbyname_r with 5 parameters in netdb.h)
AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getrpcbyname_r(NULL,NULL,NULL,0,NULL);,HAVE_GETRPCBYNAME_R="yes"; AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNAME_R_5,1,Some systems have getrpcbyname_r) AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNAME_R,1, Some systems have getrpcbyname_r) AC_DEFINE(RUDIMENTS_HAVE_NETDB_H,1, Some systems have netdb.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(for getrpcbyname_r with 5 parameters in rpc/rpcent.h)
AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#include <rpc/rpcent.h>],
getrpcbyname_r(NULL,NULL,NULL,0,NULL);,HAVE_GETRPCBYNAME_R="yes"; AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNAME_R_5,1,Some systems have getrpcbyname_r) AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNAME_R,1, Some systems have getrpcbyname_r) AC_DEFINE(RUDIMENTS_HAVE_RPCENT_H,1, Some systems have rpc/rpcent.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(for getrpcbyname_r with 5 parameters in rpc/rpc.h)
AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#include <rpc/rpc.h>],
getrpcbyname_r(NULL,NULL,NULL,0,NULL);,HAVE_GETRPCBYNAME_R="yes"; AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNAME_R_5,1,Some systems have getrpcbyname_r) AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNAME_R,1, Some systems have getrpcbyname_r) AC_DEFINE(RUDIMENTS_HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

if ( test -z "$HAVE_GETRPCBYNAME_R" )
then
	AC_MSG_CHECKING(for getrpcbyname_r with 4 parameters in netdb.h)
	AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getrpcbyname_r(NULL,NULL,NULL,0);,HAVE_GETRPCBYNAME_R="yes"; AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNAME_R_4,1,Some systems have getrpcbyname_r) AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNAME_R,1, Some systems have getrpcbyname_r) AC_DEFINE(RUDIMENTS_HAVE_NETDB_H,1, Some systems have netdb.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(for getrpcbyname_r with 4 parameters in rpc/rpcent.h)
	AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#include <rpc/rpcent.h>],
getrpcbyname_r(NULL,NULL,NULL,0);,HAVE_GETRPCBYNAME_R="yes"; AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNAME_R_4,1,Some systems have getrpcbyname_r) AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNAME_R,1, Some systems have getrpcbyname_r) AC_DEFINE(RUDIMENTS_HAVE_RPCENT_H,1, Some systems have rpc/rpcent.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(for getrpcbyname_r with 4 parameters in rpc/rpc.h)
	AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#include <rpc/rpc.h>],
getrpcbyname_r(NULL,NULL,NULL,0);,HAVE_GETRPCBYNAME_R="yes"; AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNAME_R_4,1,Some systems have getrpcbyname_r) AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNAME_R,1, Some systems have getrpcbyname_r) AC_DEFINE(RUDIMENTS_HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))
fi

if ( test -z "$HAVE_GETRPCBYNAME_R" )
then
	AC_MSG_CHECKING(for getrpcbyname in netdb)
	AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getrpcbyname(NULL);,HAVE_GETRPCBYNAME="yes"; AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNAME,1, Some systems have getrpcbyname) AC_DEFINE(RUDIMENTS_HAVE_NETDB_H,1, Some systems have netdb.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(for getrpcbyname in rpc/rpcent.h)
	AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#include <rpc/rpcent.h>],
getrpcbyname(NULL);,HAVE_GETRPCBYNAME="yes"; AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNAME,1, Some systems have getrpcbyname) AC_DEFINE(RUDIMENTS_HAVE_RPCENT_H,1, Some systems have rpc/rpcent.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(for getrpcbyname in rpc/rpc.h)
	AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#include <rpc/rpc.h>],
getrpcbyname(NULL);,HAVE_GETRPCBYNAME="yes"; AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNAME,1, Some systems have getrpcbyname) AC_DEFINE(RUDIMENTS_HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

fi

AC_MSG_CHECKING(for getrpcbynumber_r with 5 parameters in netdb.h)
AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getrpcbynumber_r(0,NULL,NULL,0,NULL);,HAVE_GETRPCBYNUMBER_R="yes"; AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNUMBER_R_5,1,Some systems have getrpcbynumber_r) AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNUMBER_R,1, Some systems have getrpcbynumber_r) AC_DEFINE(RUDIMENTS_HAVE_NETDB_H,1, Some systems have netdb.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(for getrpcbynumber_r with 5 parameters in rpc/rpcent.h)
AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#include <rpc/rpcent.h>],
getrpcbynumber_r(0,NULL,NULL,0,NULL);,HAVE_GETRPCBYNUMBER_R="yes"; AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNUMBER_R_5,1,Some systems have getrpcbynumber_r) AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNUMBER_R,1, Some systems have getrpcbynumber_r) AC_DEFINE(RUDIMENTS_HAVE_RPCENT_H,1, Some systems have rpc/rpcent.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(for getrpcbynumber_r with 5 parameters in rpc/rpc.h)
AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#include <rpc/rpc.h>],
getrpcbynumber_r(0,NULL,NULL,0,NULL);,HAVE_GETRPCBYNUMBER_R="yes"; AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNUMBER_R_5,1,Some systems have getrpcbynumber_r) AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNUMBER_R,1, Some systems have getrpcbynumber_r) AC_DEFINE(RUDIMENTS_HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

if ( test -z "$HAVE_GETRPCBYNUMBER_R" )
then

	AC_MSG_CHECKING(for getrpcbynumber_r with 4 parameters in netdb.h)
	AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getrpcbynumber_r(0,NULL,NULL,0);,HAVE_GETRPCBYNUMBER_R="yes"; AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNUMBER_R_4,1,Some systems have getrpcbynumber_r) AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNUMBER_R,1, Some systems have getrpcbynumber_r) AC_DEFINE(RUDIMENTS_HAVE_NETDB_H,1, Some systems have netdb.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(for getrpcbynumber_r with 4 parameters in rpc/rpcent.h)
	AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#include <rpc/rpcent.h>],
getrpcbynumber_r(0,NULL,NULL,0);,HAVE_GETRPCBYNUMBER_R="yes"; AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNUMBER_R_4,1,Some systems have getrpcbynumber_r) AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNUMBER_R,1, Some systems have getrpcbynumber_r) AC_DEFINE(RUDIMENTS_HAVE_RPCENT_H,1, Some systems have rpc/rpcent.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(for getrpcbynumber_r with 4 parameters in rpc/rpc.h)
	AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#include <rpc/rpc.h>],
getrpcbynumber_r(0,NULL,NULL,0);,HAVE_GETRPCBYNUMBER_R="yes"; AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNUMBER_R_4,1,Some systems have getrpcbynumber_r) AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNUMBER_R,1, Some systems have getrpcbynumber_r) AC_DEFINE(RUDIMENTS_HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))
fi

if ( test -z "$HAVE_GETRPCBYNUMBER_R" )
then
	AC_MSG_CHECKING(for getrpcbynumber in netdb)
	AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getrpcbynumber(0);,HAVE_GETRPCBYNUMBER="yes"; AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNUMBER,1, Some systems have getrpcbynumber) AC_DEFINE(RUDIMENTS_HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(for getrpcbynumber in rpc/rpcent.h)
	AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#include <rpc/rpcent.h>],
getrpcbynumber(0);,HAVE_GETRPCBYNUMBER="yes"; AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNUMBER,1, Some systems have getrpcbynumber) AC_DEFINE(RUDIMENTS_HAVE_RPCENT_H,1, Some systems have rpc/rpcent.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(for getrpcbynumber in rpc/rpc.h)
	AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#include <rpc/rpc.h>],
getrpcbynumber(0);,HAVE_GETRPCBYNUMBER="yes"; AC_DEFINE(RUDIMENTS_HAVE_GETRPCBYNUMBER,1, Some systems have getrpcbynumber) AC_DEFINE(RUDIMENTS_HAVE_RPC_H,1, Some systems have rpc/rpc.h) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))
fi

fi
])

dnl checks for shadow entry functions and header files
AC_DEFUN([FW_CHECK_SHADOW],
[

if ( test "$INCLUDE_SHADOWENTRY" = "1" )
then

HAVE_GETSPNAM_R=""

AC_MSG_CHECKING(for getspnam_r with 5 parameters)
AC_TRY_COMPILE([#include <shadow.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getspnam_r(NULL,NULL,NULL,0,NULL);,AC_DEFINE(RUDIMENTS_HAVE_GETSPNAM_R_5,1,Some systems have getspnam_r) AC_DEFINE(RUDIMENTS_HAVE_GETSPNAM_R,1, Some systems have getspnam_r) AC_MSG_RESULT(yes); HAVE_GETSPNAM_R="yes", AC_MSG_RESULT(no))

if ( test -z "$HAVE_GETSPNAM_R" )
then

	AC_MSG_CHECKING(for getspnam_r with 4 parameters)
	AC_TRY_COMPILE([#include <shadow.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getspnam_r(NULL,NULL,NULL,0);,AC_DEFINE(RUDIMENTS_HAVE_GETSPNAM_R_4,1,Some systems have getspnam_r) AC_DEFINE(RUDIMENTS_HAVE_GETSPNAM_R,1, Some systems have getspnam_r) AC_MSG_RESULT(yes); HAVE_GETSPNAM_R="yes", AC_MSG_RESULT(no))
fi

if ( test -z "$HAVE_GETSPNAM_R" )
then
	AC_MSG_CHECKING(for getspnam)
	AC_TRY_COMPILE([#include <shadow.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getspnam(NULL);,AC_DEFINE(RUDIMENTS_HAVE_GETSPNAM,1,Some systems have getspnam) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))
fi

AC_MSG_CHECKING(for spwd has sp_warn)
AC_TRY_COMPILE([#include <shadow.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
struct spwd sp; sp.sp_warn=0;,AC_DEFINE(RUDIMENTS_HAVE_SP_WARN,1,struct spwd has sp_warn) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(for spwd has sp_inact)
AC_TRY_COMPILE([#include <shadow.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
struct spwd sp; sp.sp_inact=0;,AC_DEFINE(RUDIMENTS_HAVE_SP_INACT,1,struct spwd has sp_inact)
AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(for spwd has sp_expire)
AC_TRY_COMPILE([#include <shadow.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
struct spwd sp; sp.sp_expire=0;,AC_DEFINE(RUDIMENTS_HAVE_SP_EXPIRE,1,struct spwd has sp_expire) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

AC_MSG_CHECKING(for spwd has sp_flag)
AC_TRY_COMPILE([#include <shadow.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
struct spwd sp; sp.sp_flag=0;,AC_DEFINE(RUDIMENTS_HAVE_SP_FLAG,1,struct spwd has sp_flag) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

fi
])

dnl checks for password entry functions and header files
AC_DEFUN([FW_CHECK_PWD],
[

HAVE_GETPWNAM_R=""

AC_MSG_CHECKING(for getpwnam_r with 5 parameters)
AC_TRY_COMPILE([#include <pwd.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getpwnam_r(NULL,NULL,NULL,0,NULL);,AC_DEFINE(RUDIMENTS_HAVE_GETPWNAM_R_5,1,Some systems have getpwnam_r) AC_DEFINE(RUDIMENTS_HAVE_GETPWNAM_R,1,Some systems have getpwnam_r) AC_MSG_RESULT(yes); HAVE_GETPWNAM_R="yes", AC_MSG_RESULT(no))

if ( test -z "$HAVE_GETPWNAM_R" )
then
	AC_MSG_CHECKING(for getpwnam_r with 4 parameters)
	AC_TRY_COMPILE([#include <pwd.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getpwnam_r(NULL,NULL,NULL,0);,AC_DEFINE(RUDIMENTS_HAVE_GETPWNAM_R_4,1,Some systems have getpwnam_r) AC_DEFINE(RUDIMENTS_HAVE_GETPWNAM_R,1,Some systems have getpwnam_r) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))
fi

HAVE_GETPWUID_R=""

AC_MSG_CHECKING(for getpwuid_r with 5 parameters)
AC_TRY_COMPILE([#include <pwd.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getpwuid_r(0,NULL,NULL,0,NULL);,AC_DEFINE(RUDIMENTS_HAVE_GETPWUID_R_5,1,Some systems have getpwuid_r) AC_DEFINE(RUDIMENTS_HAVE_GETPWUID_R,1,Some systems have getpwuid_r) AC_MSG_RESULT(yes); HAVE_GETPWUID_R="yes", AC_MSG_RESULT(no))

if ( test -z "$HAVE_GETPWUID_R" )
then
	AC_MSG_CHECKING(for getpwuid_r with 4 parameters)
	AC_TRY_COMPILE([#include <pwd.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getpwuid_r(0,NULL,NULL,0);,AC_DEFINE(RUDIMENTS_HAVE_GETPWUID_R_4,1,Some systems have getpwuid_r) AC_DEFINE(RUDIMENTS_HAVE_GETPWUID_R,1,Some systems have getpwuid_r) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))
fi

AC_MSG_CHECKING(for NetUserGetInfo)
AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif
#include <lm.h>],
NetUserGetInfo(NULL,NULL,0,NULL);,AC_DEFINE(RUDIMENTS_HAVE_NETUSERGETINFO,1,Some systems have NetUserGetInfo) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

])



dnl checks for group entry functions and header files
AC_DEFUN([FW_CHECK_GRP],
[

HAVE_GETGRNAM_R=""

AC_MSG_CHECKING(for getgrnam_r with 5 parameters)
AC_TRY_COMPILE([#include <grp.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getgrnam_r(NULL,NULL,NULL,0,NULL);,AC_DEFINE(RUDIMENTS_HAVE_GETGRNAM_R_5,1,Some systems have getgrnam_r) AC_DEFINE(RUDIMENTS_HAVE_GETGRNAM_R,1,Some systems have getgrnam_r) AC_MSG_RESULT(yes); HAVE_GETGRNAM_R="yes", AC_MSG_RESULT(no))

if ( test -z "$HAVE_GETGRNAM_R" )
then
	AC_MSG_CHECKING(for getgrnam_r with 4 parameters)
	AC_TRY_COMPILE([#include <grp.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getgrnam_r(NULL,NULL,NULL,0);,AC_DEFINE(RUDIMENTS_HAVE_GETGRNAM_R_4,1,Some systems have getgrnam_r) AC_DEFINE(RUDIMENTS_HAVE_GETGRNAM_R,1,Some systems have getgrnam_r) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))
fi

HAVE_GETGRGID_R=""

AC_MSG_CHECKING(for getgrgid_r with 5 parameters)
AC_TRY_COMPILE([#include <grp.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getgrgid_r(0,NULL,NULL,0,NULL);,AC_DEFINE(RUDIMENTS_HAVE_GETGRGID_R_5,1,Some systems have getgrgid_r) AC_DEFINE(RUDIMENTS_HAVE_GETGRGID_R,1,Some systems have getgrgid_r) AC_MSG_RESULT(yes); HAVE_GETGRGID_R="yes", AC_MSG_RESULT(no))

if ( test -z "$HAVE_GETGRGID_R" )
then
	AC_MSG_CHECKING(for getgrgid_r with 4 parameters)
	AC_TRY_COMPILE([#include <grp.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getgrgid_r(0,NULL,NULL,0);,AC_DEFINE(RUDIMENTS_HAVE_GETGRGID_R_4,1,Some systems have getgrgid_r) AC_DEFINE(RUDIMENTS_HAVE_GETGRGID_R,1,Some systems have getgrgid_r) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))
fi

AC_MSG_CHECKING(for NetGroupGetInfo)
NETAPI32LIB=""
AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif
#include <lm.h>],
NetGroupGetInfo(NULL,NULL,0,NULL);,AC_DEFINE(RUDIMENTS_HAVE_NETGROUPGETINFO,1,Some systems have NetGroupGetInfo) NETAPI32LIB="-lnetapi32"; AC_MSG_RESULT(yes), AC_MSG_RESULT(no))
AC_SUBST(NETAPI32LIB)

])


dnl checks for host entry functions and header files
AC_DEFUN([FW_CHECK_HOST],
[

AC_MSG_CHECKING(for getaddrinfo)
AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getaddrinfo(NULL,NULL,NULL,NULL);,AC_DEFINE(RUDIMENTS_HAVE_GETADDRINFO,1,Some systems have getaddrinfo) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

HAVE_GETHOSTBYNAME_R=""

AC_MSG_CHECKING(for gethostbyname_r with 6 parameters)
AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
gethostbyname_r(NULL,NULL,NULL,0,NULL,NULL);,AC_DEFINE(RUDIMENTS_HAVE_GETHOSTBYNAME_R,1,Some systems have gethostbyname_r) AC_DEFINE(RUDIMENTS_HAVE_GETHOSTBYNAME_R_6,1,Some systems have gethostbyname_r) AC_MSG_RESULT(yes); HAVE_GETHOSTBYNAME_R="yes", AC_MSG_RESULT(no))

if ( test -z "$HAVE_GETHOSTBYNAME_R" )
then
	AC_MSG_CHECKING(for gethostbyname_r with 5 parameters)
	AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
gethostbyname_r(NULL,NULL,NULL,0,NULL);,AC_DEFINE(RUDIMENTS_HAVE_GETHOSTBYNAME_R,1,Some systems have gethostbyname_r) AC_DEFINE(RUDIMENTS_HAVE_GETHOSTBYNAME_R_5,1,Some systems have gethostbyname_r) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))
fi

HAVE_GETHOSTBYADDR_R=""

AC_MSG_CHECKING(for gethostbyaddr_r with 8 parameters)
AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
gethostbyaddr_r(NULL,0,0,NULL,NULL,0,NULL,NULL);,AC_DEFINE(RUDIMENTS_HAVE_GETHOSTBYADDR_R,1,Some systems have gethostbyaddr_r) AC_DEFINE(RUDIMENTS_HAVE_GETHOSTBYADDR_R_8,1,Some systems have gethostbyaddr_r) AC_MSG_RESULT(yes); HAVE_GETHOSTBYADDR_R="yes", AC_MSG_RESULT(no))

if ( test -z "$HAVE_GETHOSTBYADDR_R" )
then
	AC_MSG_CHECKING(for gethostbyaddr_r with 7 parameters)
	AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
gethostbyaddr_r(NULL,0,0,NULL,NULL,0,NULL);,AC_DEFINE(RUDIMENTS_HAVE_GETHOSTBYADDR_R,1,Some systems have gethostbyaddr_r) AC_DEFINE(RUDIMENTS_HAVE_GETHOSTBYADDR_R_7,1,Some systems have gethostbyaddr_r) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))
fi

AC_MSG_CHECKING(for getnameinfo)
AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_SYS_SOCKET_H
	#include <sys/socket.h>
#endif
#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getnameinfo(NULL,0,NULL,0,NULL,0,0);,AC_DEFINE(RUDIMENTS_HAVE_GETNAMEINFO,1,Some systems have getnameinfo) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

])


dnl checks for proto entry functions and header files
AC_DEFUN([FW_CHECK_PROTO],
[

HAVE_GETPROTOBYNAME_R=""

AC_MSG_CHECKING(for getprotobyname_r with 5 parameters)
AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getprotobyname_r(NULL,NULL,NULL,0,NULL);,AC_DEFINE(RUDIMENTS_HAVE_GETPROTOBYNAME_R_5,1,Some systems have getprotobyname_r) AC_DEFINE(RUDIMENTS_HAVE_GETPROTOBYNAME_R,1,Some systems have getprotobyname_r) AC_MSG_RESULT(yes); HAVE_GETPROTOBYNAME_R="yes", AC_MSG_RESULT(no))

if ( test -z "$HAVE_GETPROTOBYNAME_R" )
then
	AC_MSG_CHECKING(for getprotobyname_r with 4 parameters)
	AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getprotobyname_r(NULL,NULL,NULL,0);,AC_DEFINE(RUDIMENTS_HAVE_GETPROTOBYNAME_R_4,1,Some systems have getprotobyname_r) AC_DEFINE(RUDIMENTS_HAVE_GETPROTOBYNAME_R,1,Some systems have getprotobyname_r) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))
fi

HAVE_GETPROTOBYNUMBER_R=""

AC_MSG_CHECKING(for getprotobynumber_r with 5 parameters)
AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getprotobynumber_r(0,NULL,NULL,0,NULL);,AC_DEFINE(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R_5,1,Some systems have getprotobynumber_r) AC_DEFINE(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R,1,Some systems have getprotobynumber_r) AC_MSG_RESULT(yes); HAVE_GETPROTOBYNUMBER_R="yes", AC_MSG_RESULT(no))

if ( test -z "$HAVE_GETPROTOBYNUMBER_R" )
then
	AC_MSG_CHECKING(for getprotobynumber_r with 4 parameters)
	AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getprotobynumber_r(0,NULL,NULL,0);,AC_DEFINE(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R_4,1,Some systems have getprotobynumber_r) AC_DEFINE(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R,1,Some systems have getprotobynumber_r) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))
fi

])


dnl checks for service entry functions and header files
AC_DEFUN([FW_CHECK_SVC],
[

HAVE_GETSERVBYNAME_R=""

AC_MSG_CHECKING(for getservbyname_r with 6 parameters)
AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getservbyname_r(NULL,NULL,NULL,NULL,0,NULL);,AC_DEFINE(RUDIMENTS_HAVE_GETSERVBYNAME_R_6,1,Some systems have getservbyname_r) AC_DEFINE(RUDIMENTS_HAVE_GETSERVBYNAME_R,1,Some systems have getservbyname_r) AC_MSG_RESULT(yes); HAVE_GETSERVBYNAME_R="yes", AC_MSG_RESULT(no))

if ( test -z "$HAVE_GETSERVBYNAME_R" )
then
	AC_MSG_CHECKING(for getservbyname_r with 5 parameters)
	AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getservbyname_r(NULL,NULL,NULL,NULL,0);,AC_DEFINE(RUDIMENTS_HAVE_GETSERVBYNAME_R_5,1,Some systems have getservbyname_r) AC_DEFINE(RUDIMENTS_HAVE_GETSERVBYNAME_R,1,Some systems have getservbyname_r) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))
fi

HAVE_GETSERVBYPORT_R=""

AC_MSG_CHECKING(for getservbyport_r with 6 parameters)
AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getservbyport_r(0,NULL,NULL,NULL,0,NULL);,AC_DEFINE(RUDIMENTS_HAVE_GETSERVBYPORT_R_6,1,Some systems have getservbyport_r) AC_DEFINE(RUDIMENTS_HAVE_GETSERVBYPORT_R,1,Some systems have getservbyport_r) AC_MSG_RESULT(yes); HAVE_GETSERVBYPORT_R="yes", AC_MSG_RESULT(no))

if ( test -z "$HAVE_GETSERVBYPORT_R" )
then
	AC_MSG_CHECKING(for getservbyport_r with 5 parameters)
	AC_TRY_COMPILE([#include <netdb.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif],
getservbyport_r(0,NULL,NULL,NULL,0);,AC_DEFINE(RUDIMENTS_HAVE_GETSERVBYPORT_R_5,1,Some systems have getservbyport_r) AC_DEFINE(RUDIMENTS_HAVE_GETSERVBYPORT_R,1,Some systems have getservbyport_r) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))
fi

])


dnl checks for mmap-related functions and header files
AC_DEFUN([FW_CHECK_MMAP],
[

HAS_MEMORYMAP="no"

AC_MSG_CHECKING(for mmap)
AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/mman.h>],
mmap(NULL,0,0,0,0,0);,AC_DEFINE(RUDIMENTS_HAVE_MMAP,1,Some systems have mmap) AC_MSG_RESULT(yes); HAS_MEMORYMAP="yes", AC_MSG_RESULT(no))

AC_MSG_CHECKING(for CreateFileMapping)
AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif],
HANDLE map=CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,1,NULL);,AC_DEFINE(RUDIMENTS_HAVE_CREATE_FILE_MAPPING,1,Some systems have CreateFileMapping) AC_MSG_RESULT(yes); HAS_MEMORYMAP="yes", AC_MSG_RESULT(no))

AC_SUBST(HAS_MEMORYMAP)

if ( test "$HAS_MEMORYMAP" = "yes" )
then

	AC_MSG_CHECKING(if munmap takes caddr_t argument)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/mman.h>],
void *ptr; munmap(ptr,0);,AC_MSG_RESULT(no), AC_DEFINE(RUDIMENTS_HAVE_MUNMAP_CADDR_T,1,Some systems use caddr_t argument for munmap) AC_MSG_RESULT(yes))

	AC_MSG_CHECKING(for mincore with char * argument)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/mman.h>],
char *tmp; mincore(NULL,0,tmp);,AC_DEFINE(RUDIMENTS_HAVE_MINCORE,1,Some systems have mincore) AC_DEFINE(RUDIMENTS_HAVE_MINCORE_CHAR,1,Some systems have mincore with char argument) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(for mincore with unsigned char * argument)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/mman.h>],
unsigned char *tmp; mincore(NULL,0,tmp);,AC_DEFINE(RUDIMENTS_HAVE_MINCORE,1,Some systems have mincore) AC_DEFINE(RUDIMENTS_HAVE_MINCORE_UCHAR,1,Some systems have mincore with unsigned char argument) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(if mincore takes caddr_t argument)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/mman.h>],
void *ptr; mincore(ptr,0,NULL);,AC_MSG_RESULT(no), AC_DEFINE(RUDIMENTS_HAVE_MINCORE_CADDR_T,1,Some systems use caddr_t argument for mincore) AC_MSG_RESULT(yes))

	AC_MSG_CHECKING(for mprotect)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/mman.h>],
mprotect(NULL,0,0);,AC_DEFINE(RUDIMENTS_HAVE_MPROTECT,1,Some systems have mprotect) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(if mprotect takes caddr_t argument)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/mman.h>],
void *ptr; mprotect(ptr,0,0);,AC_MSG_RESULT(no), AC_DEFINE(RUDIMENTS_HAVE_MPROTECT_CADDR_T,1,Some systems use caddr_t argument for mprotect) AC_MSG_RESULT(yes))

	AC_MSG_CHECKING(for msync)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/mman.h>],
msync(NULL,0,0);,AC_DEFINE(RUDIMENTS_HAVE_MSYNC,1,Some systems have msync) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(if msync takes caddr_t argument)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/mman.h>],
void *ptr; msync(ptr,0,0);,AC_MSG_RESULT(no), AC_DEFINE(RUDIMENTS_HAVE_MSYNC_CADDR_T,1,Some systems use caddr_t argument for msync) AC_MSG_RESULT(yes))

	AC_MSG_CHECKING(for mlock)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/mman.h>],
mlock(NULL,0);,AC_DEFINE(RUDIMENTS_HAVE_MLOCK,1,Some systems have mlock) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(if mlock takes caddr_t argument)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/mman.h>],
void *ptr; mlock(ptr,0);,AC_MSG_RESULT(no), AC_DEFINE(RUDIMENTS_HAVE_MLOCK_CADDR_T,1,Some systems use caddr_t argument for mlock) AC_MSG_RESULT(yes))

	AC_MSG_CHECKING(for mlockall)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/mman.h>],
mlockall(0);,AC_DEFINE(RUDIMENTS_HAVE_MLOCKALL,1,Some systems have mlockall) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(for munlock)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/mman.h>],
munlock(NULL,0);,AC_DEFINE(RUDIMENTS_HAVE_MUNLOCK,1,Some systems have munlock) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(if munlock takes caddr_t argument)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/mman.h>],
void *ptr; munlock(ptr,0);,AC_MSG_RESULT(no), AC_DEFINE(RUDIMENTS_HAVE_MUNLOCK_CADDR_T,1,Some systems use caddr_t argument for munlock) AC_MSG_RESULT(yes))

	AC_MSG_CHECKING(for munlockall)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/mman.h>],
munlockall();,AC_DEFINE(RUDIMENTS_HAVE_MUNLOCKALL,1,Some systems have munlockall) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(for madvise)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/mman.h>],
madvise(NULL,0,MADV_NORMAL);,AC_DEFINE(RUDIMENTS_HAVE_MADVISE,1,Some systems have madvise) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(if madvise takes caddr_t argument)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/mman.h>],
void *ptr; madvise(ptr,0,MADV_NORMAL);,AC_MSG_RESULT(no), AC_DEFINE(RUDIMENTS_HAVE_MADVISE_CADDR_T,1,Some systems use caddr_t argument for madvise) AC_MSG_RESULT(yes))

	AC_MSG_CHECKING(for memrchr)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#include <string.h>],
void *a=NULL; memrchr(a,0,0);,AC_DEFINE(RUDIMENTS_HAVE_MEMRCHR,1,Some systems have memrchr) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(for memmem)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#define _GNU_SOURCE
#include <string.h>],
memmem(NULL,0,NULL,0);,AC_DEFINE(RUDIMENTS_HAVE_MEMMEM,1,Some systems have memmem) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(for memccpy with char * argument)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#define _GNU_SOURCE
#include <string.h>],
char *tmp; memccpy(tmp,NULL,1,1);,AC_DEFINE(RUDIMENTS_HAVE_MEMCCPY_CHAR,1,Some systems have memccpy with char argument) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

	AC_MSG_CHECKING(for _memccpy)
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#define _GNU_SOURCE
#include <string.h>],
_memccpy(NULL,NULL,1,1);,AC_DEFINE(RUDIMENTS_HAVE__MEMCCPY,1,Some systems have _memccpy) AC_MSG_RESULT(yes), AC_MSG_RESULT(no))

fi

])


dnl checks for statfs/statvfs capibilities
AC_DEFUN([FW_STATFS],
[

STATFS_STYLE="unknown"

AC_MSG_CHECKING(for statfs/statvfs)

AC_TRY_COMPILE([#include <sys/vfs.h>],
[/* linux style */
struct statfs sfs;
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
,AC_DEFINE(RUDIMENTS_HAVE_LINUX_STATFS,1,Linux style statfs) AC_DEFINE(RUDIMENTS_HAVE_SOME_KIND_OF_STATFS,1,some type of statfs) STATFS_STYLE="linux style")

dnl cygwin is like linux but f_fsid is just a long
if ( test "$STATFS_STYLE" = "unknown" )
then
AC_TRY_COMPILE([#include <sys/vfs.h>],
[/* cygwin style */
struct statfs sfs;
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
,AC_DEFINE(RUDIMENTS_HAVE_CYGWIN_STATFS,1,Cygwin style statfs) AC_DEFINE(RUDIMENTS_HAVE_SOME_KIND_OF_STATFS,1,some type of statfs) STATFS_STYLE="cygwin style")
fi

dnl freebsd is very different from linux
if ( test "$STATFS_STYLE" = "unknown" )
then
AC_TRY_COMPILE([#include <sys/param.h>
#include <sys/mount.h>],
[/* freebsd style */
struct statfs sfs;
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
,AC_DEFINE(RUDIMENTS_HAVE_FREEBSD_STATFS,1,FreeBSD style statfs) AC_DEFINE(RUDIMENTS_HAVE_SOME_KIND_OF_STATFS,1,some type of statfs) STATFS_STYLE="freebsd style")
fi

dnl netbsd is like freebsd but lacks a few fields
if ( test "$STATFS_STYLE" = "unknown" )
then
AC_TRY_COMPILE([#include <sys/param.h>
#include <sys/mount.h>],
[/* netbsd style */
struct statfs sfs;
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
,AC_DEFINE(RUDIMENTS_HAVE_NETBSD_STATFS,1,NetBSD style statfs) AC_DEFINE(RUDIMENTS_HAVE_SOME_KIND_OF_STATFS,1,some type of statfs) STATFS_STYLE="old netbsd style")
fi

dnl netbsd-3.0 is called statvfs and is different from real statvfs
if ( test "$STATFS_STYLE" = "unknown" )
then
AC_TRY_COMPILE([#include <sys/statvfs.h>],
[/* netbsd-3.0 style */
struct statvfs sfs;
sfs.f_flag=0;
sfs.f_bsize=0;
sfs.f_frsize=0;
sfs.f_iosize=0;
sfs.f_blocks=0;
sfs.f_bfree=0;
sfs.f_bavail=0;
sfs.f_bresvd=0;
sfs.f_files=0;
sfs.f_ffree=0;
sfs.f_favail=0;
sfs.f_syncreads=0;
sfs.f_syncwrites=0;
sfs.f_asyncreads=0;
sfs.f_asyncwrites=0;
sfs.f_fsidx.__fsid_val[0]=0;
sfs.f_fsid=0;
sfs.f_namemax=0;
sfs.f_owner=0;
sfs.f_fstypename[0]=0;
sfs.f_mntonname[0]=0;
sfs.f_mntfromname[0]=0;
statvfs("/",&sfs);]
,AC_DEFINE(RUDIMENTS_HAVE_NETBSD_STATVFS,1,NetBSD-3.0 style statvfs) AC_DEFINE(RUDIMENTS_HAVE_SOME_KIND_OF_STATVFS,1,some type of statvfs) STATFS_STYLE="netbsd-3.0 style")
fi

dnl openbsd is like netbsd but with an additional mount_info union and without
dnl an f_type field
if ( test "$STATFS_STYLE" = "unknown" )
then
AC_TRY_COMPILE([#include <sys/param.h>
#include <sys/mount.h>],
[/* openbsd style */
struct statfs sfs;
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
,AC_DEFINE(RUDIMENTS_HAVE_OPENBSD_STATFS,1,OpenBSD style statfs) AC_DEFINE(RUDIMENTS_HAVE_SOME_KIND_OF_STATFS,1,some type of statfs) STATFS_STYLE="openbsd style")
fi

dnl darwin is similar to the other bsd's but with a few extra and a few missing
dnl fields
if ( test "$STATFS_STYLE" = "unknown" )
then
AC_TRY_COMPILE([#include <sys/param.h>
#include <sys/mount.h>],
[/* darwin style */
struct statfs sfs;
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
,AC_DEFINE(RUDIMENTS_HAVE_DARWIN_STATFS,1,Darwin style statfs) AC_DEFINE(RUDIMENTS_HAVE_SOME_KIND_OF_STATFS,1,some type of statfs) STATFS_STYLE="darwin style")
fi

dnl SCO and Solaris both have statvfs
if ( test "$STATFS_STYLE" = "unknown" )
then
AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/statvfs.h>],
[/* statvfs style */
struct statvfs sfs;
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
,AC_DEFINE(RUDIMENTS_HAVE_STATVFS,1,statvfs) AC_DEFINE(RUDIMENTS_HAVE_SOME_KIND_OF_STATVFS,1,some type of statvfs) STATFS_STYLE="statvfs")
fi

dnl Minix and Haiku have statvfs too, but with fewer members
if ( test "$STATFS_STYLE" = "unknown" )
then
AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/statvfs.h>],
[/* minix/haiku style */
struct statvfs sfs;
sfs.f_bsize=0;
sfs.f_frsize=0;
sfs.f_blocks=0;
sfs.f_bfree=0;
sfs.f_bavail=0;
sfs.f_files=0;
sfs.f_ffree=0;
sfs.f_favail=0;
sfs.f_fsid=0;
sfs.f_flag=0;
sfs.f_namemax=0;
statvfs("/",&sfs);]
,AC_DEFINE(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS,1,statvfs) AC_DEFINE(RUDIMENTS_HAVE_SOME_KIND_OF_STATVFS,1,some type of statvfs) STATFS_STYLE="minix/haiku style")
fi

dnl Ultrix has statfs too, but with an odd struct
if ( test "$STATFS_STYLE" = "unknown" )
then
AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/param.h>
#include <sys/mount.h>],
[/* ultrix style */
struct fs_data sfs;
sfs.fd_req.flags=0;
sfs.fd_req.mtsize=0;
sfs.fd_req.otsize=0;
sfs.fd_req.bsize=0;
sfs.fd_req.fstype=0;
sfs.fd_req.gtot=0;
sfs.fd_req.gfree=0;
sfs.fd_req.btot=0;
sfs.fd_req.bfree=0;
sfs.fd_req.bfreen=0;
sfs.fd_req.pgthresh=0;
sfs.fd_req.uid=0;
sfs.fd_req.dev=0;
sfs.fd_req.exroot=0;
sfs.fd_req.devname[0]=0;
sfs.fd_req.path[0]=0;
sfs.fd_req.nupdate=0;
statfs("/",&sfs);]
,AC_DEFINE(RUDIMENTS_HAVE_ULTRIX_STATFS,1,Ultrix style statfs) AC_DEFINE(RUDIMENTS_HAVE_SOME_KIND_OF_STATFS,1,some type of statfs) STATFS_STYLE="ultrix style")
fi


dnl Windows has it's own way of doing it
if ( test "$STATFS_STYLE" = "unknown" )
then
AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif],[GetDiskFreeSpace(NULL,NULL,NULL,NULL,NULL);],AC_DEFINE(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE,1,GetDiskFreeSpace) STATFS_STYLE="windows")
fi


AC_MSG_RESULT($STATFS_STYLE)
])

dnl checks for __xnet_*
AC_DEFUN([FW_CHECK_XNET_PROTOTYPES],
[
	if ( test -n "$XNETLIB" )
	then
		AC_MSG_CHECKING(for sendmsg prototype)
		AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_SOCKET_H
	#include <sys/socket.h>
#endif],
sendmsg(0,NULL,0);,AC_MSG_RESULT(yes), AC_DEFINE(RUDIMENTS_NEED_XNET_PROTOTYPES, 1, Solaris 2.6 has a few missing function prototypes) AC_MSG_RESULT(no))
	fi
])

dnl check to see if nanosleep is available
AC_DEFUN([FW_CHECK_NANOSLEEP],
[

	AC_MSG_CHECKING(for nanosleep)

	DONE=""
	for i in "" "-lrt"
	do
		FW_TRY_LINK([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#include <time.h>],[nanosleep(NULL,NULL);],[$CPPFLAGS],[$i],[],[NANOSLEEPLIB="$i"; DONE="yes"],[])
		if ( test -n "$DONE" )
		then
			break
		fi
	done

	if ( test -n "$DONE" )
	then
		AC_DEFINE(RUDIMENTS_HAVE_NANOSLEEP,1,Some systems have nanosleep)
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
	fi

	AC_SUBST(NANOSLEEPLIB)
])

dnl check to see if clock_nanosleep is available
AC_DEFUN([FW_CHECK_CLOCK_NANOSLEEP],
[

	AC_MSG_CHECKING(for clock_nanosleep)

	DONE=""
	for i in "" "-lrt"
	do
		FW_TRY_LINK([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#include <time.h>],[clock_nanosleep(0,0,NULL,NULL);],[$CPPFLAGS],[$i],[],[CLOCKNANOSLEEPLIB="$i"; DONE="yes"],[])
		if ( test -n "$DONE" )
		then
			break
		fi
	done

	if ( test -n "$DONE" )
	then
		AC_DEFINE(RUDIMENTS_HAVE_CLOCK_NANOSLEEP,1,Some systems have clock_nanosleep)
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
	fi

	AC_SUBST(CLOCKNANOSLEEPLIB)
])

dnl check to see if shm_open is available
AC_DEFUN([FW_CHECK_SHM_OPEN],
[

	HAS_SHM_OPEN="no"
	AC_MSG_CHECKING(for shm_open)

	DONE=""
	for i in "" "-lrt"
	do
		FW_TRY_LINK([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include <sys/mman.h>],[shm_open(NULL,0,0);],[$CPPFLAGS],[$i],[],[SHMOPENLIB="$i"; DONE="yes"; HAS_SHM_OPEN="yes"],[])
		if ( test -n "$DONE" )
		then
			break
		fi
	done

	if ( test -n "$DONE" )
	then
		AC_DEFINE(RUDIMENTS_HAVE_SHM_OPEN,1,Some systems have shm_open)
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
	fi

	AC_SUBST(SHMOPENLIB)
	AC_SUBST(HAS_SHM_OPEN)
])

dnl check to see which should be used of -lsocket, -lnsl and -lxnet
AC_DEFUN([FW_CHECK_SOCKET_LIBS],
[

	AC_MSG_CHECKING(for socket libraries)

	AC_LANG_SAVE
	AC_LANG(C)
	SOCKETLIBS=""
	DONE=""
	for i in "" "-lnsl" "-lsocket" "-lsocket -lnsl" "-lxnet" "-lwsock32 -lnetapi32" "-lnetwork"
	do
		FW_TRY_LINK([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_SOCKET_H
	#include <sys/socket.h>
#endif
#ifdef __MINGW32__
	#include <winsock2.h>
#endif],[connect(0,NULL,0);
listen(0,0);
bind(0,NULL,0);
accept(0,NULL,0);
send(0,NULL,0,0);
sendto(0,NULL,0,0,NULL,0);
#ifndef __MINGW32__
	sendmsg(0,NULL,0);
#endif
gethostbyname(NULL);],[$CPPFLAGS],[$i],[],[SOCKETLIBS="$i"; DONE="yes"],[])
		if ( test -n "$DONE" )
		then
			break
		fi
	done
	AC_LANG_RESTORE

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

AC_DEFUN([FW_CHECK_SCO_CFRONT_HACK],
[
	AC_MSG_CHECKING(whether SCO _CFRONT_3_0 hack is needed)
	LINKOK=""
	AC_TRY_COMPILE([#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif],exit(0);,LINKOK="yes")
	if ( test -z "$LINKOK" )
	then
		OLDCPPFLAGS="$CPPFLAGS"
		CPPFLAGS="-D_CFRONT_3_0 $CPPFLAGS"
		AC_TRY_LINK([#ifdef RUDIMENTS_HAVE_STDLIB_H
		#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif],exit(0);,LINKOK="yes")
		CPPFLAGS="$OLDCPPFLAGS"
		if ( test -z "$LINKOK" )
		then
			AC_MSG_RESULT(no)
		else
			AC_DEFINE(_CFRONT_3_0, 1, Hack for gcc on SCO OSR6)
			AC_MSG_RESULT(yes)
		fi
	else
		AC_MSG_RESULT(no)
	fi
])

AC_DEFUN([FW_CHECK_CRYPT_R],
[
if ( test "$INCLUDE_CRYPT" = 1 )
then

	AC_MSG_CHECKING(for crypt.h)
	FW_TRY_LINK([#include <crypt.h>],[int a=0;],[$CPPFLAGS],[],[],[CRYPT_H="#include <crypt.h>"],[CRYPT_H="#include <unistd.h>"])
	if ( test "$CRYPT_H" = "#include <crypt.h>" )
	then
		AC_MSG_RESULT(yes)
		AC_DEFINE(RUDIMENTS_HAVE_CRYPT_H,1,Some systems have crypt.h)
	else
		AC_MSG_RESULT(no)
	fi

	AC_MSG_CHECKING(for crypt)
	HAVE_CRYPT=""
	for i in "" "-lcrypt"
	do
		FW_TRY_LINK([#ifdef RUDIMENTS_HAVE_STDLIB_H
			#include <stdlib.h>
#endif
$CRYPT_H],[crypt(NULL,NULL);],[$CPPFLAGS],[$i],[],[HAVE_CRYPT="yes"; CRYPTLIB="$i"; AC_DEFINE(RUDIMENTS_HAVE_CRYPT,1,Some systems have crypt)],[])
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

	AC_MSG_CHECKING(for crypt_r)
	HAVE_CRYPT_R=""
	for i in "" "-lcrypt"
	do
		FW_TRY_LINK([#ifdef RUDIMENTS_HAVE_STDLIB_H
			#include <stdlib.h>
#endif
$CRYPT_H],[crypt_data cd; crypt_r(NULL,NULL,NULL);],[$CPPFLAGS],[$i],[],[RUDIMENTS_HAVE_CRYPT_R="yes"; CRYPTLIB="$i"; AC_DEFINE(RUDIMENTS_HAVE_CRYPT_R,1,Some systems have crypt_r)],[])
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
fi

AC_SUBST(CRYPTLIB)
])

AC_DEFUN([FW_CHECK_EXPLICIT_SPECIALIZATION],
[
AC_MSG_CHECKING(for whether explicit specialization is supported)
AC_TRY_COMPILE([
template <class t>
class a {
	public:
		int f();
};

template <> int a<int>::f() {
	return 1;
}
],[], AC_DEFINE(RUDIMENTS_EXPLICIT_SPECIALIZATION, template <>, Most modern systems support template <>) AC_MSG_RESULT(yes), AC_DEFINE(RUDIMENTS_EXPLICIT_SPECIALIZATION, , Most modern systems support template <>) AC_MSG_RESULT(no))
])
