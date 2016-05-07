
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
from ..mt.mtalpha.gentfun import *
from ..mt.mtalpha.gencreate import *
from ..mt.mtalpha.genssync import *
from ..mt.mtsparc.gentfun import *
from ..mt.mtsparc.gencreate import *
from ..mt.leon2mt.gentfun import *
from ..mt.leon2mt.gencreate import *
from ..mt.leon2mt.seq import *
from ..mt.common.strategy import *
from ..lower.spawndecl import *
from ..lower.lowerspawnsync import *
from ..am.visitors import *
from ..ptl.gentfun import *
from ..ptl.genptlcreate import *

def _common_prefix(signed_ix=True):
    return [
    ('walk', DefaultVisitor()),
    ('check', CheckVisitor()),
    ('cr2lc', Create_2_LowCreate(signed_ix)),
    ('lss', LowerSpawnSync()),
    ('lsd', LowerSpawnDecl()),
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
    'seq' : _common_prefix() + [
        ('lseta', LinkSetA()),
        ('autores', AutoResolve()),
        ('autoresss', AutoResolveSpawnSync()),
        ('flattencr',Create_2_Loop()),
        ('flattenfun',TFun_2_CFun()),
        ('flattenss',SSync_2_CSSync()),
        ] + _common_suffix,
    'hrt' : _common_prefix() + [
        ('lseta', LinkSetA()),
        ('autores', AutoResolve()),
        ('autoresss', AutoResolveSpawnSync()),
        ('flattencr',Create_2_HydraCall()),
        ('flattenfun',TFun_2_HydraCFunctions()),
        # ('flattenss',SSync_2_HydraSSync()),
        ] + _common_suffix,
    # FIXME: support hrt+seq here
    'mta' : _common_prefix() + [
        ('lseta', LinkSetA()),
        ('autores', AutoResolve()),
        ('autoresss', AutoResolveSpawnSync()),
        ('makestrategy', CreateMTStrategy(newisa=True)),
        ('flattencr', Create_2_MTACreate()), 
        ('flattenfun',TFun_2_MTATFun()),
        ('flattenss',SSync_2_MTASSync()),
        ] + _common_suffix,
    'mta+seq' : _common_prefix() + [
        ('makestrategy', CreateMTStrategy(newisa=True)),
        ('splitcr', SplitCreates(oracle=make_mt_oracle('mta'))),
        ('splitss', SplitSpawnSync(oracle=make_mt_oracle('mta'))),
        ('lseta', LinkSetA()),
        ('flseta', FlavorSetA()),
        ('splitfun', SplitFuns(oracle=make_mt_oracle('mta'))),
        ('flattencr', ScopedVisitor(Dispatcher({'cmta':Create_2_MTACreate(),
                                                'cseq':Create_2_Loop()}))),
        ('flattenfun', DefaultVisitor(Dispatcher({'fmta':TFun_2_MTATFun(),
                                                  'fseq':TFun_2_CFun()}))),
        ('flattenss', DefaultVisitor(Dispatcher({'smta':SSync_2_MTASSync(),
                                                 'sseq':SSync_2_CSSync()}))),
        ] + _common_suffix,
    'l2mt' : _common_prefix(False) + [
        ('lseta', LinkSetA()),
        ('autores', AutoResolve()),
#        ('autoresss', AutoResolveSpawnSync()),
        ('makestrategy', CreateMTStrategy()),
        ('flattencr', Create_2_L2MTCreate()), 
        ('flattenfun',TFun_2_L2MTTFun()),
#        ('flattenss',SSync_2_MTSSSync()),
        ] + _common_suffix,
    'l2mtseq' : _common_prefix(False) + [
        ('lseta', LinkSetA()),
        ('autores', AutoResolve()),
        ('autoresss', AutoResolveSpawnSync()),
        ('flattencr',Create_2_L2MTLoop()),
        ('flattenfun',TFun_2_L2MTCFun()),
        ('flattenss',SSync_2_CSSync()),
        ] + _common_suffix,
    'l2mt+seq' : _common_prefix(False) + [
        ('makestrategy', CreateMTStrategy()),
        ('splitcr', SplitCreates(oracle=make_mt_oracle('l2mt'))),
#        ('splitss', SplitSpawnSync(oracle=make_mt_oracle('mts'))),
        ('lseta', LinkSetA()),
        ('flseta', FlavorSetA()),
        ('splitfun', SplitFuns(oracle=make_mt_oracle('l2mt'))),
        ('flattencr', ScopedVisitor(Dispatcher({'cl2mt':Create_2_L2MTCreate(),
                                                'cseq':Create_2_L2MTLoop()}))),
        ('flattenfun', DefaultVisitor(Dispatcher({'fl2mt':TFun_2_L2MTTFun(),
                                                  'fseq':TFun_2_L2MTCFun()}))),
#        ('flattenss', DefaultVisitor(Dispatcher({'smta':SSync_2_MTSSSync(),
#                                                 'sseq':SSync_2_CSSync()}))),
        ] + _common_suffix,


    'mts' : _common_prefix() + [
        ('lseta', LinkSetA()),
        ('autores', AutoResolve()),
#        ('autoresss', AutoResolveSpawnSync()),
        ('makestrategy', CreateMTStrategy()),
        ('flattencr', Create_2_MTSCreate()), 
        ('flattenfun',TFun_2_MTSTFun()),
#        ('flattenss',SSync_2_MTSSSync()),
        ] + _common_suffix,
    'mts+seq' : _common_prefix() + [
        ('makestrategy', CreateMTStrategy()),
        ('splitcr', SplitCreates(oracle=make_mt_oracle('mts'))),
#        ('splitss', SplitSpawnSync(oracle=make_mt_oracle('mts'))),
        ('lseta', LinkSetA()),
        ('flseta', FlavorSetA()),
        ('splitfun', SplitFuns(oracle=make_mt_oracle('mts'))),
        ('flattencr', ScopedVisitor(Dispatcher({'cmts':Create_2_MTSCreate(),
                                                'cseq':Create_2_Loop()}))),
        ('flattenfun', DefaultVisitor(Dispatcher({'fmts':TFun_2_MTSTFun(),
                                                  'fseq':TFun_2_CFun()}))),
#        ('flattenss', DefaultVisitor(Dispatcher({'smta':SSync_2_MTSSSync(),
#                                                 'sseq':SSync_2_CSSync()}))),
        ] + _common_suffix,
    'mtsn' : _common_prefix() + [
        ('lseta', LinkSetA()),
        ('autores', AutoResolve()),
#        ('autoresss', AutoResolveSpawnSync()),
        ('makestrategy', CreateMTStrategy(newisa=True)),
        ('flattencr', Create_2_MTSCreate(newisa=True)), 
        ('flattenfun',TFun_2_MTSTFun()),
#        ('flattenss',SSync_2_MTSSSync()),
        ] + _common_suffix,
    'mtsn+seq' : _common_prefix() + [
        ('makestrategy', CreateMTStrategy(newisa=True)),
        ('splitcr', SplitCreates(oracle=make_mt_oracle('mts'))),
#        ('splitss', SplitSpawnSync(oracle=make_mt_oracle('mts'))),
        ('lseta', LinkSetA()),
        ('flseta', FlavorSetA()),
        ('splitfun', SplitFuns(oracle=make_mt_oracle('mts'))),
        ('flattencr', ScopedVisitor(Dispatcher({'cmts':Create_2_MTSCreate(newisa=True),
                                                'cseq':Create_2_Loop()}))),
        ('flattenfun', DefaultVisitor(Dispatcher({'fmts':TFun_2_MTSTFun(),
                                                  'fseq':TFun_2_CFun()}))),
#        ('flattenss', DefaultVisitor(Dispatcher({'smts':SSync_2_MTSSSync(),
#                                                 'sseq':SSync_2_CSSync()}))),
        ] + _common_suffix,
    'ptl' : _common_prefix() + [
        ('makestrategy', CreateMTStrategy(newisa=True)),
        ('lseta', LinkSetA()),
        ('autores', AutoResolve()),
        ('autoresss', AutoResolveSpawnSync()),
        ('flattencr',Create_2_ptlCreate()),
        ('flattenfun',TFun_2_ptlTFun()),
        #('flattenss',SSync_2_ptlSSync()), # not supported in ptl (yet)
        ] + _common_suffix,
    'ptl+seq' : _common_prefix() + [
        ('makestrategy', CreateMTStrategy(newisa=True)),
        ('splitcr', SplitCreates(oracle=make_mt_oracle('ptl'))), 
#        ('splitss', SplitSpawnSync(oracle=make_mt_oracle('ptl'))), # not supported in ptl (yet)
        ('lseta', LinkSetA()),
        ('flseta', FlavorSetA()),
        ('splitfun', SplitFuns(oracle=make_mt_oracle('ptl'))),
        ('flattencr', ScopedVisitor(Dispatcher({'cptl':Create_2_ptlCreate(),
                                                'cseq':Create_2_Loop()}))),
        ('flattenfun', DefaultVisitor(Dispatcher({'fptl':TFun_2_ptlTFun(),
                                                  'fseq':TFun_2_CFun()}))),
#        ('flattenss', DefaultVisitor(Dispatcher({'sptl':SSync_2_MTSSSync(),   # not supported in ptl (yet)
#                                                 'sseq':SSync_2_CSSync()}))),
        ] + _common_suffix,
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
        visitor.extra_options = opts.extras
        program.accept(visitor)
        
        dname = get_dump_fname(lbl)
        if dname is not None:
            f = file(dname, 'w')
            program.accept(PrintVisitor(stream = f))
            f.close()
    
__all__ = ['run_chain']
