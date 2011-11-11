#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "tlsmalloc.h"
#include "tlsconfig.h"

#define TIMEOUT 20

#ifdef DEBUG_MALLOC_STORAGE
#define ARENA_SIZE (8*SLOT_SIZE)
#else
#define ARENA_SIZE (10*1024*1024) /* 10Mb */
#endif

THREADSPEC char* __th_bottom;
THREADSPEC char* __th_top;

static char arena[ARENA_SIZE] __attribute__((__aligned__(SLOT_SIZE)));

char *__alltls_base = &arena[0];
size_t __alltls_total_size = ARENA_SIZE;
size_t __tls_size;

volatile int finish_test;

void alarm_sig(int sig)
{
    finish_test = 1;
}

void die(const char *where)
{
    perror(where);
    _exit(1);
}

/* Code from DJGPP, http://www.delorie.com/djgpp/malloc/test.c */

#define MAXP 400
#define SUBP 20
#define NPASS 25
#define NLOOP 12

int lrand()
{
    static unsigned long long next = 0;
    next = next * 0x5deece66dLL + 11;
    return (int)((next >> 16) & 0x7fffffff);
}

int rsize()
{
    int rv = 8 << (lrand() % 15);
    rv = lrand() & (rv-1);
#ifdef DEBUG_MALLOC_STORAGE
    rv &= (FIRST_SIZE_CLASS << (NR_OF_BINS-1)) - 1;
#endif
    return rv;
}



void do_test(void)
{
    int i, r, loop, pass, subpass;
    int realloc_mask = 48;
    int absmax=0, curmax=0;

    char *pointers[MAXP];
    size_t size[MAXP];

    for (i = 0; i < MAXP; ++i)
        pointers[i] = tls_malloc(rsize());
#ifdef DO_MALLINFO
    fprintf(stderr, "---- stats ----\n");
    tls_malloc_stats();
    fflush(stdout);
    fprintf(stderr, "---- end stats ----\n");
#endif
    for (i = 0; i < MAXP; ++i)
        tls_free(pointers[i]);

    memset(pointers, 0, MAXP*sizeof(pointers[0]));

    for (loop=0; loop<NLOOP; loop++)
    {
        for (pass=0; pass<NPASS; pass++)
        {
            for (subpass=0; !finish_test && subpass<SUBP; subpass++)
            {
                for (i=0; i<MAXP; i++)
                {
#ifdef DO_MALLINFO
                    tls_malloc_stats();
#endif
                    int rno = rand();
                    if (rno & 8)
                    {
                        if (pointers[i])
                        {
                            if (!(rno & realloc_mask))
                            {
                                r = rsize();
                                curmax -= size[i];
                                curmax += r;
                                pointers[i] = tls_realloc(pointers[i], r);
                                size[i] = r;
                                if (absmax < curmax) absmax = curmax;
                            }
                            else
                            {
                                curmax -= size[i];
                                tls_free(pointers[i]);
                                pointers[i] = 0;
                            }
                        }
                        else
                        {
                            r = rsize();
                            curmax += r;
                            pointers[i] = tls_malloc(r);
                            size[i] = r;
                            if (absmax < curmax) absmax = curmax;
                        }
                    }
                }
            }
        }
    }
}

static pthread_barrier_t barrier;

void step(void)
{
    int c = pthread_barrier_wait(&barrier);
    if ((c != 0) && (c != PTHREAD_BARRIER_SERIAL_THREAD))
        die("pthread_barrier_wait");
}

void* start_thread(void *arg)
{
    size_t idx = (size_t)arg;

    if (__tls_size < SLOT_SIZE)
        return 0;

    __th_bottom = __alltls_base + idx * __tls_size;
    __th_top = __alltls_base + (idx+1) * __tls_size;

    fprintf(stderr, "Thread %zd starts (%p -> %p).\n", idx, __th_bottom, __th_top);
    if (mprotect(__th_bottom, SLOT_SIZE, PROT_READ|PROT_WRITE) == -1)
        die("mprotect");

    step();

    do_test();

    step();

    fprintf(stderr, "Thread %zd ends.\n", idx);
    return 0;
}

int main(int argc, char **argv)
{
    size_t P, l2P;
    size_t i;

    fprintf(stderr, "Arena: %p -> %p (%zd bytes)\n", 
            __alltls_base, __alltls_base + __alltls_total_size, __alltls_total_size);

    for (l2P = 0, P = 1; l2P <= 4; ++l2P, P *= 2)
    {
        fprintf(stderr, "Clearing arena...\n");
        if (mprotect(__alltls_base, __alltls_total_size, PROT_READ|PROT_WRITE) == -1)
            die("mprotect");
        memset(__alltls_base, 0, __alltls_total_size);
        if (mprotect(__alltls_base, __alltls_total_size, PROT_NONE) == -1)
            die("mprotect");

        fprintf(stderr, "Testing with %zd threads...\n", P);
        pthread_t ths[P];
        
        __tls_size = __alltls_total_size / P;

        if (pthread_barrier_init(&barrier, 0, P)) die("pthread_barrier_init");

        finish_test = 0;
        if (SIG_ERR == signal(SIGALRM, alarm_sig)) die("signal");
        alarm(TIMEOUT);

        srand(0);

        for (i = 0; i < P; ++i)
            if (pthread_create(&ths[i], 0, start_thread, (void*)i)) die("pthread_create");

        for (i = 0; i < P; ++i)
            if (pthread_join(ths[i], 0)) die("pthread_join");

        if (pthread_barrier_destroy(&barrier)) die("pthread_barrier_destroy");

    }
    return 0;
}
