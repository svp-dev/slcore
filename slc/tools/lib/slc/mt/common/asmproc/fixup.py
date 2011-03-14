from ....front import dump

from stageopts import makestageopts
from filter import funfilter, cfunfilter

import sys

def filter(fbegin, finner, fcinner, fend, *args):
    """ 
    Transform compiler output (invalid, raw asm) to good-looking
    .s files that can be assembled by an external assembler.
    """

    opts = makestageopts([fbegin, finner, fcinner, fend], args)
    fname = opts.resolved.output
    if fname == "-":
        outf = sys.stdout
    else:
        try:
            outf = file(fname, "w")
        except Exception, e:
            die('%s: %r' % (fname, e))

    for inname in opts.inputs:
        items = inname
        for t in fbegin:
            items = t(items)
            items = dump.dump_gen(t.__name__, items)
        for t in finner:
            items = funfilter(t, items)
            items = dump.dump_gen(t.__name__, items)
        for t in fcinner:
            items = cfunfilter(t, items)
            items = dump.dump_gen(t.__name__, items)
        for t in fend:
            items = t(items)
            items = dump.dump_gen(t.__name__, items)
        lines = items
        for line in lines:
            # print "YOO:", line,
            outf.write(line)
    outf.flush()
    if fname != "-": outf.close()