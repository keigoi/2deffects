#ifndef __SYD__TYPES__H__
#define __SYD__TYPES__H__

#ifdef _MSC_VER

typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
typedef __int8  int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;

#else
#include <stdint.h>

#endif



#endif /* __SYD__TYPES__H__ */