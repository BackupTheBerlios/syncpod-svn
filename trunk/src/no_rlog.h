// Copyright (c)2005 Jean-Baptiste Lab (jean-baptiste dot lab at laposte dot net)
// See the file copying.txt for copying permission.

#if !defined NO_RLOG_H
#define NO_RLOG_H

// Define out the log macro used
/* #define rInfo( x... ) */
/* #define rWarning( x... ) */
/* #define rError( x... ) */

#include <stdio.h>

#define rInfo    printf( "\n" ); printf
#define rWarning printf( "\n" ); printf
#define rError   printf( "\n" ); printf

#endif
