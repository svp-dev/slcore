#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void *__sgr_base;

void fail(const char *progname, const char *where)
{
  fprintf(stderr, "%s: %s: %s\n", progname, where, strerror(errno));
  exit(1);
}

void load(const char *progname, const char* fname) 
{
  if (!fname) {
    fprintf(stderr, "%s: warning: no data file specified (did you use sgr?)\n", progname);
    __sgr_base = 0;
    return ;
  }
  FILE *f;
  if (!(f = fopen(fname, "rb"))) fail(progname, "fopen");
  fseek(f, 0L, SEEK_END);

  long sz = ftell(f);
  if (sz < 0) fail(progname, "ftell");
  /*  else if (!sz) {
    fprintf(stderr, "%s: %s: data file is empty\n", progname, fname);
    exit(1);
    } */

  if (sz > 0) {
    void *p;
    if (!(p = malloc(sz))) fail(progname, "malloc");
    
    fseek(f, 0L, SEEK_SET);
    if (0 == fread(p, sz, 1, f)) fail(progname, "fread");
    __sgr_base = p;
  }
  else
    __sgr_base = 0;
  fclose(f);

}

