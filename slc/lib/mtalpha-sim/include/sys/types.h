#ifndef SLC_MTA_SYS_TYPES_H
#define SLC_MTA_SYS_TYPES_H

// this header exists for compatibility,
// not intended to be accurate!

#include <stddef.h>
#include <stdint.h>

typedef unsigned char   u_char;
typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef unsigned long   u_long;

typedef uint8_t       u_int8_t;       /* unsigned integrals (deprecated) */
typedef uint16_t      u_int16_t;
typedef uint32_t      u_int32_t;
typedef uint64_t      u_int64_t;

typedef uint64_t      u_quad_t;       /* quads (deprecated) */
typedef int64_t       quad_t;
typedef quad_t *        qaddr_t;

typedef char *          caddr_t;        /* core address */
typedef const char *  c_caddr_t;      /* core address, pointer to const */
typedef volatile char *v_caddr_t;     /* core address, pointer to volatile */

typedef int64_t  off_t;

typedef long clock_t;

#if defined(__alpha__) || defined(__mtalpha__)
typedef long ssize_t;
#endif

#endif
