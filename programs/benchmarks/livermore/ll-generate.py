#! /usr/bin/env python

import csv
import sys

codepat = "bench%d.c"
sizes = [100,200,500,1000,2000,5000,10000,20000,50000,100000]
datapat = "data/data%d.d%d"
authors = "mhicks & kena"

def genstatestruct(f, k):
    print >>f, "struct bdata {\n   size_t n;"
    for (name, spec) in k['args'].items():
        if spec['type'] == 'scalar':
            if 'w' in spec['mode']:
                print >>f, "   double %s_orig;" % name;
            print >>f, "   double %s;" % name;
        else:
            if 'w' in spec['mode']:
                print >>f, "   const double * restrict %s_orig;" % name;
                print >>f, "   double * restrict %s;" % name
            else:
                print >>f, "   const double * restrict %s;" % name;
            for (i,_) in enumerate(spec['size']):
                print >>f, "   size_t %s_dim%d;" % (name, i)
            print >>f, "   size_t %s_size;" % name

    print >>f, "};"

def geninit(f, k):
    print >>f, """
sl_def(initialize, void,
    sl_glparm(struct benchmark_state*, st))
{
   size_t i, f = 0;
   struct bdata *bdata = (struct bdata*) malloc(sizeof (struct bdata));
   svp_assert(bdata != NULL);

   output_char('\\n', 1);
   svp_assert(fibre_tag(f) == 0);
   svp_assert(fibre_rank(f) == 0);
   bdata->n = *(unsigned long*)fibre_data(f); ++f;
"""

    l = k['args'].keys()
    l.sort()
    for (name, spec) in ((v, k['args'][v]) for v in l):
        print >>f, '   output_string("#  reading data for %s...\\n", 1);' % name
        print >>f, "   svp_assert(fibre_tag(f) == 2);"
        if spec['type'] == 'scalar':
            print >>f, "   svp_assert(fibre_rank(f) == 0);"
            if 'w' in spec['mode']:
                target = "bdata->%s_orig" % name
            else: 
                target = "bdata->%s" % name
            print >>f, "   %s = *(double*)fibre_data(f); ++f;" % target;
        else:
            print >>f, "   svp_assert(fibre_rank(f) == %d);" % len(spec['size'])
            for (i,_) in enumerate(spec['size']):
                print >>f, "   bdata->%s_dim%d = fibre_shape(f)[%d];" % (name, i, i)
            sspec = " * ".join(("bdata->%s_dim%d" % (name,i) for i in xrange(len(spec['size']))))
            if 'w' not in spec['mode']:
                print >>f, "   bdata->%s = (const double*)fibre_data(f);" % name
            else:
                print >>f, "   bdata->%s_orig = (const double*)fibre_data(f);" % name
                print >>f, "   bdata->%s = (double*)malloc(sizeof(double) * %s);" % (name, sspec)
            print >>f, "   bdata->%s_size = %s; ++f;\n" % (name, sspec)

    print >>f, """
   sl_getp(st)->data = (void*) bdata;
}
sl_enddef
"""

def genprepare(f, k):
    print >>f, """
sl_def(prepare, void,
    sl_glparm(struct benchmark_state*, st))
{
   size_t i;
   struct bdata *bdata = (struct bdata*)sl_getp(st)->data;
"""
   
    for (name, spec) in k['args'].items():
        if spec['type'] == 'scalar':
            if 'w' in spec['mode']:
                print >>f, "   bdata->%s = bdata->%s_orig;" % (name, name);
        else:
            if 'w' in spec['mode']:
                print >>f, "   for (i = 0; i < bdata->%s_size; ++i) bdata->%s[i] = bdata->%s_orig[i];" % (name, name, name)

    print >>f, """
}
sl_enddef
"""

def genoutput(f, k):
    print >>f, """
sl_def(output, void,
    sl_glparm(struct benchmark_state*, st))
{
   size_t i;
   struct bdata *bdata = (struct bdata*)sl_getp(st)->data;
"""
   
    l = k['args'].keys()
    l.sort()
    for (name, spec) in ((v, k['args'][v]) for v in l):
        if spec['type'] == 'scalar':
            if 'w' in spec['mode']:
                print >>f, '   output_string("# end value for %s\\n", 1);' % name
                print >>f, "   output_float(bdata->%s, 1, 4); output_char('\\n', 1); " % name
        else:
            if 'w' in spec['mode']:
                print >>f, '   output_string("# end values for %s[]\\n", 1);' % name
                print >>f, "   for (i = 0; i < bdata->%s_size; ++i) { output_float(bdata->%s[i], 1, 4); output_char('\\n', 1); }" % (name, name)

    print >>f, """
}
sl_enddef
"""

def genteardown(f, k):
    print >>f, """
sl_def(teardown, void,
    sl_glparm(struct benchmark_state*, st))
{
   struct bdata *bdata = (struct bdata*)sl_getp(st)->data;
"""
   
    for (name, spec) in k['args'].items():
        if spec['type'] == 'array' and 'w' in spec['mode']:
            print >>f, "   free(bdata->%s);" % name
    print >>f, """
   free(bdata);
}
sl_enddef
"""

def genwork(f, k):

    print >>f, """sl_decl(kernel%d, void,
   sl_glparm(size_t, ncores),
   sl_glparm(size_t, n)""" % k['idx']

    l = k['args'].keys()
    l.sort()
    for (name, spec) in ((v, k['args'][v]) for v in l):
        if spec['type'] == 'scalar':
            if 'w' in spec['mode']:
                print >>f, "     , sl_shfparm(double, %s)" % name
            else:
                print >>f, "     , sl_glfparm(double, %s)" % name
        else:
            if 'w' in spec['mode']:
                print >>f, "     , sl_glparm(double*restrict, %s)" % name
            else:
                print >>f, "     , sl_glparm(const double*restrict, %s)" % name
            for (i,_) in enumerate(spec['size']):
                print >>f, "     , sl_glparm(size_t, %s_dim%d)" % (name, i)
   
    print >>f, """);

#include "kernel%d.c"

sl_def(work, void,
    sl_glparm(struct benchmark_state*, st))
{
   struct bdata *bdata = (struct bdata*)sl_getp(st)->data;
   sl_create(,,,,,,, kernel%d, 
#if SVP_HAS_SEP
             sl_glarg(size_t, , sl_getp(st)->place->ncores),
#else
             sl_glarg(size_t, , 0),
#endif
             sl_glarg(size_t, , bdata->n)
""" % (k['idx'], k['idx'])
    
    l = k['args'].keys()
    l.sort()
    for (name, spec) in ((v, k['args'][v]) for v in l):
        if spec['type'] == 'scalar':
            if 'w' in spec['mode']:
                print >>f, "     , sl_shfarg(double, %s, bdata->%s)" % (name, name)
            else:
                print >>f, "     , sl_glfarg(double, , bdata->%s)" % name
        else:
            if 'w' in spec['mode']:
                print >>f, "     , sl_glarg(double*restrict, , bdata->%s)" % name
            else:
                print >>f, "     , sl_glarg(const double*restrict, , bdata->%s)" % name
            for (i,_) in enumerate(spec['size']):
                print >>f, "     , sl_glarg(size_t, , bdata->%s_dim%d)" % (name, i)
   
    print >>f, "   );\n   sl_sync();\n"
    for (name, spec) in k['args'].items():
        if spec['type'] == 'scalar' and 'w' in spec['mode']:
            print >>f, "   bdata->%s = sl_geta(%s);" % (name, name)
    print >>f, """
}
sl_enddef
"""


def gencode(k):
    idx = k['idx']
    df = codepat % idx
    print "Generating %s..." % df
    f = file(df,'w')
    print >>f, """
/* This file is automatically generated, do not edit! */
#include <svp/testoutput.h>
#include "benchmark.h"
#include <svp/fibre.h>
#include <svp/assert.h>
#include <cmalloc.h>

"""

    genstatestruct(f, k)

    geninit(f, k)

    genprepare(f, k)

    genwork(f, k)

    genoutput(f, k)

    genteardown(f, k)

    print >>f, """

sl_def(t_main, void)
{
  struct benchmark b = {
     "%s", "%s", "%s", 
     &initialize, &prepare, &work, &output, &teardown
  };
  sl_proccall(run_benchmark, sl_glarg(struct benchmark*, , &b));
}
sl_enddef
""" % ("LK%d (%s)" % (idx, k['key']), k['desc'], authors)
    f.close()


def geninputs(k):
    idx = k['idx']
    for sz in sizes:
        n = int(sz * k['ratio'])
        if n < 10:
            print "Size to small for kernel: %d" % n
            continue
        df = datapat % (idx, n)
        print "Generating %s..." % df
        f = file(df,'w')
        print >>f, "# n (problem size)"
        print >>f, n
        l = k['args'].keys()
        l.sort()
        for aname,aspec in ((v, k['args'][v]) for v in l):
            if 'r' not in aspec['mode']:
                continue
            print >>f, "# %s" % aname 
            if aspec['type'] == 'scalar':
                print >>f, "1.0"
            else:
                if len(aspec['size']) == 1:
                    # 1-D array
                    sz = aspec['size'][0]
                    if aspec['good']:
                        try:
                            sz = str(eval(sz))
                        except: pass
                    print >>f, "v(%s)" % sz
                elif len(aspec['size']) == 2:
                    # 2-D array
                    good = False
                    sz1 = aspec['size'][0]
                    sz2 = aspec['size'][1]
                    if aspec['good']:
                        try:
                            sz1 = eval(sz1)
                            sz2 = eval(sz2)
                            good = True
                        except: pass
                    if good:
                        print >>f, "[1,%d: %s]" % (sz2, ("v(%d) " % sz1) * sz2) 
                    else:
                        print >>f, "[1,1: [1,1: 1.0 ] ] # size unknown"
                elif len(aspec['size']) == 3:
                    # 2-D array
                    good = False
                    sz1 = aspec['size'][0]
                    sz2 = aspec['size'][1]
                    sz3 = aspec['size'][2]
                    if aspec['good']:
                        try:
                            sz1 = eval(sz1)
                            sz2 = eval(sz2)
                            sz3 = eval(sz3)
                            good = True
                        except: pass
                    if good:
                        print >>f, "[1,%d: %s]" % (sz3, ("[1,%d: %s] " % (sz2, ("v(%d) " % sz1) * sz2)) * sz3) 
                    else:
                        print >>f, "[1,1: [1,1: [1,1: 1.0 ] ] ] # size unknown"
                else:
                    print >>f, "[] # unknown rank"
        f.close()

def D(name, props):
    if props == 'o':
        return (name, {'type':'scalar', 'mode':'wo'})
    elif props == 'x':
        return (name, {'type':'scalar', 'mode':'ro'})
    elif props == 'xw':
        return (name, {'type':'scalar', 'mode':'rw'})
    else:
        mode = 'ro'
        if props[-2:] == ',W':
            mode = 'rw'
            props = props[:-2]
        dspec = []
        good = True
        for x in props.split(','):
            if ".." in x:
                dspec.append(x.split('..')[1])
            else:
                good = False
                dspec.append(x)
        return (name, {'type':'array', 'mode':mode, 'size':dspec, 'good':good})


def read(rows):
    rows.next() # skip first row (headers)
    for row in rows:
        yield { 'idx': int(row[0]),
                'key': row[1],
                'desc' : row[2],
                'indep' : row[3],
                'ratio': float(row[4].replace(',','.')),
                'args' : dict((eval(x) for x in row[5:] if x != ""))
                }

if __name__ == "__main__":
    csvread = csv.reader(sys.stdin, delimiter=',', quotechar='"')
    bread = read(csvread)        

    for kernel in bread:
        print kernel
        print "Generating code for kernel %d ('%s', %d args)..." % (kernel['idx'], kernel['key'], len(kernel['args']))
        gencode(kernel)
        geninputs(kernel)

