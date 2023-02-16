/*******************************************************************************/
#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#ifndef UCHAR	// uc
typedef unsigned char   UCHAR;
#endif

#ifndef PUCHAR	// puc
typedef unsigned char  *PUCHAR;
#endif

#ifndef USHORT	// us
typedef unsigned short  USHORT;
#endif

#ifndef WORD	// w
typedef unsigned short  WORD;
#endif

#ifndef PWORD	// pw
typedef WORD*           PWORD;
#endif

#ifndef PUSHORT	// pus
typedef unsigned short *PUSHORT;
#endif

#ifndef INT	// ui
typedef signed int    INT;
#endif

#ifndef UINT	// ui
typedef unsigned int    UINT;
#endif

#ifndef ULONG	// ul
typedef unsigned long   ULONG;
#endif

//  1 byte
#ifndef char	// c
typedef char            CHAR;
#endif

#ifndef PCHAR	// pc
typedef char            *PCHAR;
#endif

#ifndef short	// s
typedef short           SHORT;
#endif

#ifndef PSHORT	// ps
typedef short           *PSHORT;
#endif

#ifndef BYTE	// by
typedef unsigned char   BYTE;
#endif

#ifndef PBYTE	// pby
typedef BYTE*           PBYTE;
#endif

//  4 bytes
#ifndef DWORD	// dw
typedef unsigned long   DWORD;
#endif

#ifndef PDWORD	// pdw
typedef DWORD*          PDWORD;
#endif

#ifndef PUINT	// pui
typedef UINT*           PUINT;
#endif

#ifndef long	// l
typedef long            LONG;
#endif

#ifndef PLONG	// pl
typedef long            *PLONG;
#endif

#ifndef PULONG	// plu
typedef unsigned long   *PULONG;
#endif

#ifndef BOOLEAN	// b
typedef unsigned char   BOOLEAN;
#endif

#ifndef PVOID	// pv
typedef void*           PVOID;
#endif

#ifndef HANDLE	// h
typedef void*           HANDLE;
#endif

//--------- for floating point -------------
#ifndef DOUBLE
typedef double            DOUBLE;
#endif

#ifndef FLOAT	// fl
typedef float  			   FLOAT;
#endif

// non-window conflict types
#ifndef SCHAR   //c
typedef signed char     SCHAR;
#endif

#ifndef SWORD	// sw
typedef signed short    SWORD;
#endif

#ifndef SDWORD	// sdw
typedef signed int		SDWORD;
#endif

#ifndef VOID
typedef void            VOID;
#endif

typedef unsigned long long int  QWORD;  //qw
typedef long long int           SQWORD;//sqw

typedef char                  S08; /* !<  8 bits signed integer */
typedef short                 S16;  /* !< 16 bits signed integer */
typedef int                   S32;  /* !< 32 bits signed integer */
typedef long long             S64;  /* !< 64 bits signed integer */

typedef unsigned char         U08;  /* !<  8 bits unsigned integer */
typedef unsigned short        U16;  /* !< 16 bits unsigned integer */
typedef unsigned int          U32;  /* !< 32 bits unsigned integer */
typedef unsigned long long    U64; /* !< 64 bits unsigned integer */

typedef float                 FLOAT;  /* !< floating point real number, simple precision */
typedef double                DOUBLE;/* !< floating point real number, double precision */
//typedef unsigned char         BOOL;  /* !< boolean */

#define       SBYTE           S08    /* !<  8 bits signed integer */
#define       SWORD           S16    /* !< 16 bits signed integer */
#define       SDWORD          S32    /* !< 32 bits signed integer */
#define       SLWORD          S64    /* !< 64 bits signed integer */

#define       BYTE            U08    /* !<  8 bits signed integer */
#define       WORD            U16    /* !< 16 bits signed integer */
#define       DWORD           U32    /* !< 32 bits signed integer */
#define       LWORD           U64    /* !< 64 bits unsigned integer */

#ifndef ON
#define ON  	1
#endif

#ifndef OFF
#define OFF 	0
#endif

#ifndef S_OK
#define S_OK 	0
#endif

#ifndef S_FAIL
#define S_FAIL (SCODE)(-1)
#endif

#ifndef S_INVALID_VERSION
#define S_INVALID_VERSION (SCODE)(-2)
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL  0
#endif

#endif // _TYPEDEF_H_
