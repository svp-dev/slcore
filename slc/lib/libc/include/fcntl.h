#ifndef SLC_MTA_FCNTL_H
#define SLC_MTA_FCNTL_H

/* values suitable for use with the mgsim RPC interface. */
#define O_RDONLY   1
#define O_WRONLY   2
#define O_RDWR     3
#define O_ACCMODE  3
#define O_APPEND   8
#define O_NOFOLLOW 0x100
#define O_CREAT    0x200
#define O_TRUNC    0x400
#define O_EXCL     0x800

int     open(const char *, int, ...);


#endif
