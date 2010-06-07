
from dump import get_dump_fname
import opts
from ..visitors import *
from ..input.check import *
from ..lower.seq import *
from ..lower.lowercreate import *
from ..lower.split import *
from ..lower.lowgeta import *
from ..lower.lowcvars import *
from ..lower.lowccast import *
from ..lower.lowclabels import *
from ..lower.flavorseta import *
from ..lower.rename import *
from ..lower.remflavors import *
from ..mtalpha.visitors import *
from ..am.visitors import *

_common_prefix = [
    ('walk', DefaultVisitor()),
    ('check', CheckVisitor()),
    ('cr2lc', Create_2_LowCreate()),
    ]

_common_suffix = [
     ('geta', ReduceGetA()),
     ('rnlbls', RenameCLabels()),
     ('rnvars', RenameCVars()),
     ('cvars', ReduceCVars()),
     ('ccast', ReduceCCast()),
     ('clabels', ReduceCLabels()),
     ('remflavors', RemoveFlavors()),
]

_chains = {
    'seq' : _common_prefix + [
        ('lseta', LinkSetA()),
        ('flattencr',Create_2_Loop()),
        ('flattenfun',TFun_2_CFun())] + _common_suffix,
    'am' : _common_prefix + [
        ('lseta', LinkSetA()),
        ('flattencr',Create_2_HydraCall()),
        ('flattenfun',TFun_2_HydraCFunctions())], #  + _common_suffix,
    'mta' : _common_prefix + [
        ('lseta', LinkSetA()),
        ('flattencr', Create_2_MTACreate()), 
        ('flattenfun',TFun_2_MTATFun())] + _common_suffix,
    'mta+seq' : _common_prefix + [
        ('splitcr', SplitCreates()),
        ('lseta', LinkSetA()),
        ('flseta', FlavorSetA()),
        ('splitfun', SplitFuns()),
        ('flattencr', ScopedVisitor(Dispatcher({'cmta':Create_2_MTACreate(),
                                                'cseq':Create_2_Loop()}))),
        ('flattenfun', DefaultVisitor(Dispatcher({'fmta':TFun_2_MTATFun(),
                                                  'fseq':TFun_2_CFun()}))),
        ] + _common_suffix
}

_default_chain = 'seq'

for k,v in _chains.items():
    for (a, b) in v:
        opts.register_dump_stage(a)

opts.register_arg('-b', action = "store", nargs = 1, dest = "target",
                  choices = _chains.keys(), default = _default_chain,
                  help="Select translation target.")

def run_chain(program):
    t = _chains[opts.resolved.target]
    for (lbl, visitor) in t:
        #print "XX, ", lbl
        program.accept(visitor)
        
        dname = get_dump_fname(lbl)
        if dname is not None:
            f = file(dname, 'w')
            program.accept(PrintVisitor(stream = f))
            f.close()
    
__all__ = ['run_chain']
