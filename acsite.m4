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

	for i in "$SEARCHPATH" "/usr/local/$NAME" "/opt/$NAME" "/usr/$NAME" "/usr/local" "/usr/pkg" "/opt/sfw" "/usr/local/firstworks"
	do
		if ( test -n "$i" )
		then
			FW_CHECK_HEADER_LIB([$i/include/$HEADER],[INCLUDESTRING=\"-I$i/include\"],[$i/lib/lib$LIBNAME.so],[LIBPATH=\"$i/lib\"; LIBSTRING=\"-L$i/lib -l$LIBNAME\"],[$i/lib/lib$LIBNAME.a],[LIBSTRING=\"-L$i/lib -l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
			if ( test -z "$LIBSTRING" )
			then
				FW_CHECK_HEADER_LIB([$i/include/$NAME/$HEADER],[INCLUDESTRING=\"-I$i/include/$NAME\"],[$i/lib/lib$LIBNAME.so],[LIBPATH=\"$i/lib\"; LIBSTRING=\"-L$i/lib -l$LIBNAME\"],[$i/lib/lib$LIBNAME.a],[LIBSTRING=\"-L$i/lib -l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
			fi
			if ( test -z "$LIBSTRING" )
			then
				FW_CHECK_HEADER_LIB([$i/include/$HEADER],[INCLUDESTRING=\"-I$i/include\"],[$i/lib/$NAME/lib$LIBNAME.so],[LIBPATH=\"$i/lib/$NAME\"; LIBSTRING=\"-L$i/lib/$NAME -l$LIBNAME\"],[$i/lib/$NAME/lib$LIBNAME.a],[LIBSTRING=\"-L$i/lib/$NAME -l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
			fi
			if ( test -z "$LIBSTRING" )
			then
				FW_CHECK_HEADER_LIB([$i/include/$NAME/$HEADER],[INCLUDESTRING=\"-I$i/include/$NAME\"],[$i/lib/$NAME/lib$LIBNAME.so],[LIBPATH=\"$i/lib/$NAME\"; LIBSTRING=\"-L$i/lib/$NAME -l$LIBNAME\"],[$i/lib/$NAME/lib$LIBNAME.a],[LIBSTRING=\"-L$i/lib/$NAME -l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
			fi
			if ( test -n "$LIBSTRING" )
			then
				HEADERSANDLIBSPATH="$i"
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

if ( test "$cross_compiling" = "yes" )
then
	
	dnl cross compiling
	echo "cross compiling"
	if ( test -n "$PTHREADSPATH" )
	then
		PTHREADSINCLUDES="-I$PTHREADSPATH/include"
		HAVE_PTHREADS="yes"
	fi

else
	AC_MSG_CHECKING(for /usr/include/$i.h)
	if ( test -r "/usr/include/pthread.h" )
	then
		AC_MSG_RESULT(yes)
		HAVE_PTHREADS="yes"
		break
	else
		AC_MSG_RESULT(no)
		for i in "pthread" "pthreads" "pth" "gthread" "gthreads"
		do
			for j in "$PTHREADSPATH/include" "$PTHREADSPATH/include/$i" "/usr/include/$i" "/usr/local/include" "/usr/local/include/$i" "/usr/pkg/include" "/usr/pkg/include/$i" "/usr/local/include" "/usr/local/include/$i" "/opt/sfw/include" "/opt/sfw/include/$i"
			do
				if ( test -n "$j" )
				then
					AC_MSG_CHECKING(for $j/pthread.h)
					if ( test -r "$j/pthread.h" )
					then
						PTHREADSINCLUDES="-I$j"
						AC_MSG_RESULT(yes)
						HAVE_PTHREADS="yes"
						break
					fi
					AC_MSG_RESULT(no)
				fi
			done
		done
	fi
fi
AC_SUBST(PTHREADSINCLUDES)
if ( test -z "$HAVE_PTHREADS" )
then
	AC_MSG_ERROR(pthreads.h not found.  Rudiments requires this file.)
	exit
fi
])
