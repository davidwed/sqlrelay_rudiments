// Copyright (c) 1999-2015  David Muse
// See the file COPYING for more information

#ifndef RUDIMENTS_DEBUGPRINT_H
#define RUDIMENTS_DEBUGPRINT_H

/** The debugprint header provides the following debug print functions:


 ** Print the name of the currently-executing function. *
 debugFunction();


 ** Print "formatstring", optionally followed by values which will be
 *  substitited into formatstring.  Similar to filedescriptor::printf(). *
 debugPrintf(formatstring, ...);


 ** Prints "length" bytes of "str", printing hex values for non-printable
 *  characters.  Similar to filedescriptor::safePrint(). *
 debugSafePrint(str,length);


 ** Prints a series of 1's and 0's representing the bits that compose
 *  "number". *
 debugPrintBits(number);



 ** To enable these functions, define DEBUG_MESSAGES prior to including this
 *  header.  For example:
 #define DEBUG_MESSAGES 1
 #include <rudiments/debugprint.h>


 ** To print to a file, rather than to standard output, define DEBUG_TO_FILE
 *  and provide a "debugfile".  For example:

 ** On Linux/Unix:
 #define DEBUG_MESSAGES 1
 #define DEBUG_TO_FILE 1
 static const char	debugfile[]="/tmp/debug.txt";
 #include <rudiments/debugprint.h>

 ** On Windows:
 #define DEBUG_MESSAGES 1
 #define DEBUG_TO_FILE 1
 static const char	debugfile[]="C:\\debug.txt";
 #include <rudiments/debugprint.h>

*/

#include <rudiments/private/debugprint.h>

#endif
