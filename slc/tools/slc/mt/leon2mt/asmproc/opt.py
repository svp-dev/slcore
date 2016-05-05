from ...mtsparc.asmproc import common
from ..regdefs import regmagic
from ...common.asmproc.remdefaults import *
from ...common.asmproc.labels import *
from ...common.asmproc.swch import *
from ...common.asmproc.end import *
from ...mtsparc.asmproc.decode import decode
from ...mtsparc.asmproc.delay import *
from ...mtsparc.asmproc.opt import grouper, vnametocanon, \
    canontovname, makedataflow, \
    protectend


_filter_begin = [common.reader, 
                 common.lexer, 
                 common.splitsemi, 
                 common.parser,
                 grouper]
_filter_stages = [
        vnametocanon,
        remdefaults,

        findlabels,
        killunusedlabels,

        decode,
        makedataflow(addswch = True, drainargs = False),

        rmdupswch,
        rmswchbegin,

        prunenopend,

        markdelay,
        protectend,

        canontovname,

        rmswchdir,
        ]
_filter_end = [common.flattener,
               common.printer]
                 

from ...common.asmproc import enhance

def filter(*args):
	return enhance.filter(_filter_begin, _filter_stages, _filter_end, *args)
