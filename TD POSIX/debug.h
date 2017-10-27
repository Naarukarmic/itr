#ifndef __DEBUG_H__
#define __DEBUG_H__

#include<stdio.h>

#define debug_printf(fmt, ...) fprintf(stderr, fmt, ## __VA_ARGS__)
/* Debug printf */

#define DO_PRAGMA(x) _Pragma (#x)
#define TODO(x) DO_PRAGMA(message ("TODO - " #x))

#endif /* __DEBUG_H__ */
