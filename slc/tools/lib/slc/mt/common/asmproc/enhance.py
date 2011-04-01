from ....front import dump

from stageopts import makestageopts
from filter import funfilter
import sys

def filter(fbegin, fstages, fend, *args):
    """
    Enhance already valid assembly.
    """

    opts = makestageopts([fbegin, fstages, fend], args)
    filter_stagenames = [s.__name__ for s in fstages]

    # parse flags
    sel = dict(((s,True) for s in filter_stagenames))
    
    for f in opts.resolved.selection:
        if f == 'no-all' or f == 'none':
            for k in sel.keys(): sel[k] = False
        elif f == 'all':
            for k in sel.keys(): sel[k] = True
        else:
            if f.startswith('no-'):
                name = f[3:]
                value = False
            else:
                name = f
                value = True
            if name not in filter_stagenames:
                die('error: unrecognized command line option "-f%s-%s"' %
                     (value and "asmopt" or "no-asmopt", name))
            sel[name] = value

    verbose = opts.resolved.verbose

    if verbose: log('asmsel: %r' % sel)

    fname = opts.resolved.output
    if fname == "-":
        outf = sys.stdout
    else:
        try:
            outf = file(fname, "w")
        except Exception, e:
            die('%s: %r' % (fname, e))

    extras = opts.resolved.extras
    for inname in opts.inputs:
        items = inname
        for t in fbegin:
            t.extra_options = extras
            items = t(items)
            items = dump.dump_gen(t.__name__, items)
        for t in fstages:
            if sel[t.__name__]:
                #if verbose: log('asmopt: %s' % t.__name__)
                t.extra_options = extras
                items = funfilter(t, items)
                items = dump.dump_gen(t.__name__, items)
        for t in fend:
            t.extra_options = extras
            items = t(items)
            items = dump.dump_gen(t.__name__, items)
        lines = items
        for line in lines:
            # print "YOO:", line,
            outf.write(line)
    outf.flush()
    if fname != "-": outf.close()


    
