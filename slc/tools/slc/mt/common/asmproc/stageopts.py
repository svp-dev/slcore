
from ....front import opts

def makestageopts(stagelists, args):
    opts.register_arg('-e', action = "append", dest = "selection",
                  metavar = "STAGE", nargs = 1,
                  default = [],
                  help = 'Select ("STAGE") or unselect ("no-STAGE") the specified pass.')

    for e in stagelists:
        for s in e:
            opts.register_dump_stage(s.__name__)
    opts.parse_args(list(args))

    return opts

