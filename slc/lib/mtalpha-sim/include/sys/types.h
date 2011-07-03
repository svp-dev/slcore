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
typedef long time_t;

typedef long ssize_t;

typedef long suseconds_t;

typedef size_t blksize_t;
typedef uint64_t blkcnt_t;
typedef uint64_t ino_t;
typedef uint16_t mode_t;
typedef uint16_t nlink_t;

struct timespec {
    time_t  tv_sec;        /* seconds */
    long    tv_nsec;       /* and nanoseconds */
};


#endif
