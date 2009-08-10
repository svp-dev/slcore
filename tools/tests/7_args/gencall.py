import sys

def gencall(out, nrshareds, nrglobals, ty):
    if ("float" in ty) or ("double" in ty):
        sc = "f"
    else:
        sc = ""
    print >>out, "#include <svp/testoutput.h>\n"
    print >>out, "sl_def(foo_%d_%d, void, sl_glparm(%s*, output)" % (nrshareds, nrglobals, ty),
    if nrshareds or nrglobals:
        print >>out, ","
    print >>out, ',\n'.join(["\tsl_sh%sparm(%s,sarg%d)" % (sc,ty,x) for x in xrange(nrshareds)] + 
                   ["\tsl_gl%sparm(%s,garg%d)" % (sc,ty,x) for x in xrange(nrglobals)]),
    print >>out, ")\n{\n  *sl_getp(output) = (",
    if nrshareds or nrglobals:
        print >>out, '\t+'.join(["sl_getp(sarg%d)\n" % x for x in xrange(nrshareds)] +
                       ["sl_getp(garg%d)\n" % x for x in xrange(nrglobals)]),
    else:
        print >>out, "0",
    print >>out, "  );"
    for i in xrange(nrshareds):
        print >>out, "  sl_setp(sarg%d, sl_getp(sarg%d));" % (i, i)
    print >>out, "}\nsl_enddef"
    print >>out
    print >>out, "sl_def(do_call, void) {"
    print >>out, "  %s result; " % ty
    print >>out, "  sl_proccall(foo_%d_%d, sl_glarg(%s*, output, &result)" % (nrshareds, nrglobals, ty),
    if nrshareds or nrglobals:
        print >>out, ","
    print >>out, ',\n'.join(["\tsl_sh%sarg(%s,sarg%d,%d)" % (sc,ty,x,x) for x in xrange(nrshareds)] + 
                   ["\tsl_gl%sarg(%s,garg%d,%d)" % (sc,ty,x,x) for x in xrange(nrglobals)]),
    print >>out, "  );"
    print >>out, "  long iresult = result;"
    print >>out, "  output_int(iresult, 1);"
    print >>out, "  output_char('\\n', 1);\n}\nsl_enddef"
    print >>out, "sl_def(t_main, void) { sl_proccall(do_call); } sl_enddef"


if __name__ == "__main__":
    nrshareds = int(sys.argv[1])
    nrglobals = int(sys.argv[2])
    ty = sys.argv[3]
    gencall(sys.stdout, nrshareds, nrglobals, ty)

