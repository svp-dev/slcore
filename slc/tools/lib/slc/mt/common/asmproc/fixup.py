from ....front import opts, dump

opts.register_arg('-e', action = "append", dest = "selection",
                  metavar = "STAGE", nargs = 1,
                  default = [],
                  help = 'Select ("STAGE") or unselect ("no-STAGE") the specified pass.')

from filter import funfilter, cfunfilter

def filter(fbegin, finner, fcinner, fend, *args):
    """ 
    Transform compiler output (invalid, raw asm) to good-looking
    .s files that can be assembled by an external assembler.
    """

    for e in [fbegin, finner, fcinner, fend]:
        for s in e:
            opts.register_dump_stage(s.__name__)
    opts.parse_args(list(args))

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
