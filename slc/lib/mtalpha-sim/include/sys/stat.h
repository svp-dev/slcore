#ifndef SLC_MTA_SYS_STAT_H
#define SLC_MTA_SYS_STAT_H

#include <sys/types.h>

struct stat {
    uint16_t  st_dev;
    ino_t     st_ino;
    mode_t    st_mode;
    nlink_t   st_nlink;
    time_t    st_atime;
    struct timespec st_atimespec;
    struct timespec st_mtimespec;
    struct timespec st_ctimespec;
    off_t     st_size;
    blkcnt_t  st_blocks;
    blksize_t st_blksize;
};

int fstat(int fd, struct stat* buf);

#endif
