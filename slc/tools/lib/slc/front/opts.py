import optparse
from slc_config import package_version, package_bugreport
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
register_arg('-P', action = "store_false", dest = "synclines", default = True,
             help="Inhibit generation of linemarkers.")
register_arg('-I', action = "append", nargs = 1, dest = "includes", metavar = "DIR",
             help="Add DIR to the include search path.")

resolved = None
inputs = None

def parse_args():
    stages = _dump_stages.keys() + ['all']
    register_arg('-d', action = "append", dest = "dump_stages", metavar = "STAGES",
                 choices = stages, default = [],
                 help="Make debugging dumps during compilation at the times specified (choice: %s)." 
                 % ', '.join(stages))

    parser = optparse.OptionParser(option_list = _option_list,
                                   version = """%%prog %s

Copyright (C) 2009,2010 The SL project.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Written by Raphael 'kena' Poss.
""" % package_version, epilog = "Report bugs and suggestions to %s." % package_bugreport)

    global resolved, inputs
    resolved, inputs = parser.parse_args()
    return (resolved, inputs)
    



