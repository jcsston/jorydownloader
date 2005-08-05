#ifndef _CYGWIN_INTTYPES_H
#define _CYGWIN_INTTYPES_H
/* /usr/include/inttypes.h for CYGWIN
 * Copyleft 2001-2002 by Felix Buenemann
 * <atmosfear at users.sourceforge.net>
 */
typedef char *caddr_t;
typedef signed char int8_t;
typedef unsigned char u_int8_t;
typedef short int16_t;
typedef unsigned short u_int16_t;
typedef int int32_t;
typedef unsigned int u_int32_t;
#ifdef _MSC_VER
 typedef __int64 int64_t;
 typedef unsigned __int64 u_int64_t;
 #define i64(c) (c ## i64)
 #define u64(c) (c ## ui64)
#else
 #ifndef __int32
  typedef long int __int32; 
 #endif
 #ifndef __int64
  typedef long long __int64;
 #endif
 typedef long long int64_t;
 typedef unsigned long long u_int64_t;
 #define i64(c) (c ## LL)
 #define u64(c) (c ## ULL)
#endif
typedef int32_t register_t;
typedef u_int8_t uint8_t;
typedef u_int16_t uint16_t;
typedef u_int32_t uint32_t;
typedef u_int64_t uint64_t;
typedef uint32_t ptr_t;
#define __WORDSIZE 32

typedef unsigned short UINT16;
typedef signed short INT16;
typedef unsigned char UINT8;
typedef unsigned int UINT32;
typedef uint64_t UINT64;
typedef signed char INT8;
typedef signed int INT32;
typedef int64_t INT64;

typedef long _ssize_t;
typedef _ssize_t ssize_t;

#endif
