#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <svp/delegate.h>

#include "mgsim/mtconf.h"
#include "mgsim/RPCServiceDatabase.h"

/* device control words, see mgsim / RPC interface */
#define RC_CMD    0
#define RC_PROC   2
#define RC_COMPLL 4
#define RC_COMPLH 5
#define RC_ARG1SZ 6
#define RC_ARG2SZ 7
#define RC_ARG1AL 8
#define RC_ARG1AH 9
#define RC_ARG2AL 10
#define RC_ARG2AH 11
#define RC_ARG3   12
#define RC_ARG4   13
#define RC_RES1AL 14
#define RC_RES1AH 15
#define RC_RES2AL 16
#define RC_RES2AH 17

#define COMMON_BEGIN                                          \
    volatile uint32_t* ctl = sl_getp(ctl);                    \
    volatile long* notif = sl_getp(notif);                    \
    int err;                                                  \
    struct                                                    \
    {                                                         \
        uint32_t rval_low;                                    \
        uint32_t rval_high;                                   \
        uint32_t errno;                                       \
    } res;                                                    \
    /* drain notifications */                                 \
    *notif = 0;                                               \
    while (*notif != 0)                                       \
        ;                                                     \
    /* wait until the device is ready */                      \
    while (ctl[RC_CMD] != 0)                                  \
        ;                                                     \
    ctl[RC_COMPLL] = 0xcafe42; /* completion */               \
    ctl[RC_COMPLH] = 0; /* completion */                      \
    ctl[RC_ARG1SZ] = 0; /* arg size 1 */                      \
    ctl[RC_ARG2SZ] = 0; /* arg size 2 */                      \
    ctl[RC_RES1AL] = ((uintptr_t)&res & 0xffffffff);          \
    ctl[RC_RES1AH] = ((uintptr_t)&res >> 32);                 \
    ctl[RC_RES2AL] = 0; /* res base addr */                   \
    ctl[RC_RES2AH] = 0; /* res base addr */ 

#define COMMIT_CHECK                                            \
    /* commit */                                                \
    *notif = 1;                                                 \
    ctl[RC_CMD] = 1;                                            \
    /* wait until done */                                       \
    if (*notif != 0xcafe42)                                     \
        err = EIO;                                              \
    else                                                        \
    {                                                           \
        *notif = 0;                                             \
        err = res.errno;                                        \
    }                                                           \
    errno = err;                                                \
    long long rval = ((long long)res.rval_high << 32) | res.rval_low;

sl_def(do_open, sl__static, 
       sl_glparm(uint32_t*, ctl), 
       sl_glparm(long*, notif),
       sl_glparm(const char*, path), 
       sl_glparm(size_t, pathlen),
       sl_shparm(int, oflag_fd), 
       sl_glparm(int, mode))
{
    COMMON_BEGIN;

    ctl[RC_PROC] = RPC_open;
    ctl[RC_ARG1SZ] = sl_getp(pathlen);
    ctl[RC_ARG1AL] = ((uintptr_t)sl_getp(path) & 0xffffffff);
    ctl[RC_ARG1AH] = ((uintptr_t)sl_getp(path) >> 32);
    ctl[RC_ARG3] = sl_getp(oflag_fd);
    ctl[RC_ARG4] = sl_getp(mode);
    
    COMMIT_CHECK;
    
    sl_setp(oflag_fd, rval);
}
sl_enddef

int open(const char* path, int oflag, ...)
{
    if (mg_rpc_devid == (size_t)-1)
    {
        errno = ENOSYS;
        return -1;
    }
    uint32_t * ctl = mg_devinfo.base_addrs[mg_rpc_devid];
    long * notif = &mg_devinfo.channels[mg_rpc_chanid];

    int mode = 0;
    if (oflag & O_CREAT)
    {
        va_list ap;
        va_start(ap, oflag);
        mode = va_arg(ap, int);
        va_end(ap);
    }
    
    size_t pathlen = strlen(path);
    if (pathlen > 1023)
    {
        errno = ENAMETOOLONG;
        return -1;
    }
    char pathbuf[1024];
    memcpy(pathbuf, path, pathlen + 1);
    
    sl_create(, mg_io_place_id,,,,, sl__exclusive, do_open,
              sl_glarg(uint32_t*,, ctl),
              sl_glarg(long*,, notif),
              sl_glarg(const char*,, pathbuf),
              sl_glarg(size_t,, pathlen + 1),
              sl_sharg(int, fd, oflag),
              sl_glarg(int, , mode));
    sl_sync();
    return sl_geta(fd);
}

sl_def(do_close, sl__static, 
       sl_glparm(uint32_t*, ctl), 
       sl_glparm(long*, notif),
       sl_shparm(int, fd_ret))
{
    COMMON_BEGIN;

    ctl[RC_PROC] = RPC_close;
    ctl[RC_ARG3] = sl_getp(fd_ret);
    
    COMMIT_CHECK;
    
    sl_setp(fd_ret, rval);
}
sl_enddef

int close(int fd)
{
    if (mg_rpc_devid == (size_t)-1)
    {
        errno = ENOSYS;
        return -1;
    }
    uint32_t * ctl = mg_devinfo.base_addrs[mg_rpc_devid];
    long * notif = &mg_devinfo.channels[mg_rpc_chanid];

    sl_create(, mg_io_place_id,,,,, sl__exclusive, do_close,
              sl_glarg(uint32_t*,, ctl),
              sl_glarg(long*,, notif),
              sl_sharg(int, ret, fd));
    sl_sync();
    return sl_geta(ret);
}


sl_def(do_read, sl__static, 
       sl_glparm(uint32_t*, ctl), 
       sl_glparm(long*, notif),
       sl_shparm(ssize_t, fd_ret),
       sl_glparm(void*, data),
       sl_glparm(size_t, sz))
{
    COMMON_BEGIN;

    ctl[RC_PROC] = RPC_read;
    ctl[RC_RES2AL] = ((uintptr_t)sl_getp(data) & 0xffffffff);
    ctl[RC_RES2AH] = ((uintptr_t)sl_getp(data) >> 32);
    ctl[RC_ARG3] = sl_getp(fd_ret);
    ctl[RC_ARG4] = sl_getp(sz);
    
    COMMIT_CHECK;
    
    sl_setp(fd_ret, rval);
}
sl_enddef

ssize_t read(int fd, void* data, size_t sz)
{
    if (mg_rpc_devid == (size_t)-1)
    {
        errno = ENOSYS;
        return -1;
    }
    uint32_t * ctl = mg_devinfo.base_addrs[mg_rpc_devid];
    long * notif = &mg_devinfo.channels[mg_rpc_chanid];

    sl_create(, mg_io_place_id,,,,, sl__exclusive, do_read,
              sl_glarg(uint32_t*,, ctl),
              sl_glarg(long*,, notif),
              sl_sharg(ssize_t, ret, fd),
              sl_glarg(void*,,data),
              sl_glarg(size_t,, sz));
    sl_sync();
    return sl_geta(ret);
}

sl_def(do_write, sl__static, 
       sl_glparm(uint32_t*, ctl), 
       sl_glparm(long*, notif),
       sl_shparm(ssize_t, fd_ret),
       sl_glparm(void*, data),
       sl_glparm(size_t, sz))
{
    COMMON_BEGIN;

    ctl[RC_PROC] = RPC_write;
    ctl[RC_ARG1SZ] = sl_getp(sz);
    ctl[RC_ARG1AL] = ((uintptr_t)sl_getp(data) & 0xffffffff);
    ctl[RC_ARG1AH] = ((uintptr_t)sl_getp(data) >> 32);
    ctl[RC_ARG3] = sl_getp(fd_ret);
    ctl[RC_ARG4] = sl_getp(sz);
    
    COMMIT_CHECK;
    
    sl_setp(fd_ret, rval);
}
sl_enddef

ssize_t write(int fd, void* data, size_t sz)
{
    if (mg_rpc_devid == (size_t)-1)
    {
        errno = ENOSYS;
        return -1;
    }
    uint32_t * ctl = mg_devinfo.base_addrs[mg_rpc_devid];
    long * notif = &mg_devinfo.channels[mg_rpc_chanid];

    sl_create(, mg_io_place_id,,,,, sl__exclusive, do_write,
              sl_glarg(uint32_t*,, ctl),
              sl_glarg(long*,, notif),
              sl_sharg(ssize_t, ret, fd),
              sl_glarg(void*,,data),
              sl_glarg(size_t,, sz));
    sl_sync();
    return sl_geta(ret);
}

sl_def(do_fstat, sl__static, 
       sl_glparm(uint32_t*, ctl), 
       sl_glparm(long*, notif),
       sl_shparm(int, fd_ret),
       sl_glparm(struct vstat*, st))
{
    COMMON_BEGIN;

    ctl[RC_PROC] = RPC_fstat;
    ctl[RC_RES2AL] = ((uintptr_t)sl_getp(st) & 0xffffffff);
    ctl[RC_RES2AH] = ((uintptr_t)sl_getp(st) >> 32);
    ctl[RC_ARG3] = sl_getp(fd_ret);
    
    COMMIT_CHECK;
    
    sl_setp(fd_ret, rval);
}
sl_enddef

int fstat(int fd, struct stat* buf)
{
    if (mg_rpc_devid == (size_t)-1)
    {
        errno = ENOSYS;
        return -1;
    }
    uint32_t * ctl = mg_devinfo.base_addrs[mg_rpc_devid];
    long * notif = &mg_devinfo.channels[mg_rpc_chanid];

    struct vstat vst;

    sl_create(, mg_io_place_id,,,,, sl__exclusive, do_fstat,
              sl_glarg(uint32_t*,, ctl),
              sl_glarg(long*,, notif),
              sl_sharg(int, ret, fd),
              sl_glarg(struct vstat*,,&vst));
    sl_sync();
    if (sl_geta(ret) == 0)
    {
        buf->st_dev = vst.vst_dev;
        buf->st_ino = ((uint64_t)vst.vst_ino_high << 32) | (uint64_t)vst.vst_ino_low;
        buf->st_mode = vst.vst_mode;
        buf->st_nlink = vst.vst_nlink;
        buf->st_atimespec.tv_sec = vst.vst_atime_secs;
        buf->st_atimespec.tv_nsec = vst.vst_atime_nsec;
        buf->st_mtimespec.tv_sec = vst.vst_mtime_secs;
        buf->st_mtimespec.tv_nsec = vst.vst_mtime_nsec;
        buf->st_ctimespec.tv_sec = vst.vst_ctime_secs;
        buf->st_ctimespec.tv_nsec = vst.vst_ctime_nsec;
        buf->st_size = ((uint64_t)vst.vst_size_high << 32) | (uint64_t)vst.vst_size_low;
        buf->st_blocks = ((uint64_t)vst.vst_blocks_high << 32) | (uint64_t)vst.vst_blocks_low;
        buf->st_blksize = vst.vst_blksize;
    }
    return sl_geta(ret);
}
