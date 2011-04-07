import optparse
import sys
from slc_config import package_version, package_bugreport
from ..msg import warn

_option_list = []

_dump_stages = {}

def register_arg(*args, **kwargs):
    _option_list.append(optparse.make_option(*args, **kwargs))

def register_dump_stage(name, description = None):
    #assert name not in _dump_stages
    _dump_stages[name] = description

def register_option(opt):
    _option_list.append(opt)


register_arg('-v', '--verbose', action = "store_true", dest="verbose", default = False,
             help="Run verbosely.")
register_arg('-o', action = "store", nargs = 1, dest = "output", default = "-",
             metavar = "FILE", help="Print translated code to FILE.")
register_arg('-x', action = "append", dest = "extraopts", metavar = "EXTRAOPT",
             nargs = 1, default = [],
             help="Set the specified extra option EXTRAOPT.")

class ExtraOpts(object):

    def __init__(self, resolved):
        opts = {}
        for o in resolved.extraopts:
            if o.startswith('no-'):
                opts[o[3:]] = False
            else:
                opts[o] = True
        self.opts = opts
        self.touched = set()

    def __getitem__(self, name):
        self.touched.add(name)
        return self.get(name)

    def get(self, name, default = None):
        self.touched.add(name)
        return self.opts.get(name, default)
        
resolved = None
inputs = None
extras = None

def check_unused():
    for k in extras.opts.keys():
        if k not in extras.touched:
            warn('unused option "%s"' % k)


def parse_args(args = None):
    if args is None:
        args = sys.argv[1:]
    stages = _dump_stages.keys() + ['all']
    register_arg('-d', action = "append", dest = "dump_stages", metavar = "STAGE",
                 nargs = 1,
                 choices = stages, default = [],
                 help="Make debugging dumps during compilation at the times specified (choice: %s)." 
                 % ', '.join(stages))

    parser = optparse.OptionParser(option_list = _option_list,
                                   version = """%%prog %s

Copyright (C) 2009,2010,2011 The SL project.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Written by Raphael 'kena' Poss.
""" % package_version, epilog = "Report bugs and suggestions to %s." % package_bugreport)

    global resolved, inputs, extras
    resolved, inputs = parser.parse_args(args)
    extras = ExtraOpts(resolved)
    return (extras, resolved, inputs)
    



