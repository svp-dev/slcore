#ifndef SLC_MTA_UNISTD_H
# define SLC_MTA_UNISTD_H

#include <sys/types.h>

int     close(int);
off_t   lseek(int, off_t, int);
ssize_t read(int, void*, size_t);
ssize_t write(int, void*, size_t);

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#endif
