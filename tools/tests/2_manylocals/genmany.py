#! /usr/bin/env python

from cStringIO import StringIO as sio
import random
import sys

body = sio()

N = int(sys.argv[1])
dofuncs = int((sys.argv + [0])[2])

numbers = ','.join([str(int(random.uniform(0, 1000))) for x in xrange(N)])

for i in xrange(0, N):
    print >>body,"reg l%d = a[%d];" % (i, i)

beg = 0
for i in xrange(0, N):
    ops = []
    off = 0
    for op in xrange(0, N/2):
        ops.append("OP%d(l%s,l%s)" % (op % 16, (beg + off)%N, (beg+off+1)%N))
        off = off + 2
    str = ' R '.join(ops)
    if not str:
        str = '0'
    print >>body, "a[%d] = %s;" % (i, str)
    print >>body, "docall(a, %s);" % ','.join("l%d" % x for x in xrange(0, N))
    beg = beg + 1

print """
/* Automatically generated. Do not edit! */
#include <svp/testoutput.h>
#define reg long
#define OP0(A,B) (A>>(B&1))
#define OP1(A,B) (A<<(B&1))
#define OP2(A,B) (A-B)
#define OP3(A,B) (A&~B)
#define OP4(A,B) (A^~B)
#define OP5(A,B) (A*-B)
#define OP6(A,B) (A+3*B)
#define OP7(A,B) (A-5*B)
#define OP8(A,B) (A+11*B)
#define OP9(A,B) (A-13*B)
#define OP10(A,B) (A+17*B)
#define OP11(A,B) (A-19*B)
#define OP12(A,B) (A+23*B)
#define OP13(A,B) (A-27*B)
#define OP14(A,B) (A+37*B)
#define OP15(A,B) (A-41*B)
#define R ^

static reg arr[%d] = {%s};
reg *a = arr;

#if %d
#include <svp/compiler.h>
noinline void foo(reg* hello, ...) { asm(""); };
#define docall(X, ...) foo(X, __VA_ARGS__)
#else
#define docall(X, ...) /* nothing */
#endif

sl_def(t_main, void) {
%s
int i;
for (i = 0; i < %d; ++i)
  { output_int(a[i], 1); output_char('\\n', 1); }
} 
sl_enddef
""" % (N, numbers, dofuncs, body.getvalue(), N)

