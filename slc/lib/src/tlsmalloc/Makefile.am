
EXTRA_DIST = tls_arch_mtalpha.h

CC = $(PTHREAD_CC)

if CHECK_TLSMALLOC
check_PROGRAMS = tlsmcheck
noinst_PROGRAMS = $(check_PROGRAMS)
tlsmcheck_CPPFLAGS = -DARCH_TLS_SERVICES=\"tls_arch_pthreads.h\"
tlsmcheck_CFLAGS = $(PTHREAD_CFLAGS) $(AM_CFLAGS)
tlsmcheck_LDADD = $(PTHREAD_LIBS)
tlsmcheck_SOURCES = tlsmcheck.c tlsmalloc.c tlsmalloc.h tricks.h
endif
