from ..regdefs import regmagic
from ...mtsparc.asmproc.decode import decode
from ...mtsparc.asmproc.common import *
from ...common.asmproc.regextract import *
from ...common.asmproc.renameregs import *
from ...common.asmproc.labels import *
from ...common.asmproc.markused import *
from ...common.asmproc.compress import *
from ...mtsparc.asmproc.delay import *
from ...mtsparc.asmproc.opt import makedataflow
from ...mtsparc.asmproc.base import detectregs, cdetectregs, renamelocals, replaceret, xsplit, \
    initspfp, xjoin1, \
    xjoin2, \
    grouper, \
    cmarkused, \
    protectallcallregs, \
    protectend, \
    fillannulnop, \
    markaligned, \
    crenameregs, \
    renameregs, \
    zerog0

# FIXME: re-implement xjoin2 here


_filter_begin = [reader, lexer, splitsemi, parser, grouper]
_cfilter_inner = [canonregs,
                  crenameregs,

                  cdetectregs,
                  cmarkused,
                  # creplsave,
                  # creplrestore,

                  # FIXME: is this useful?
                  #fillannulnop,

                  decode,
                  makedataflow(addswch = False, drainargs = True),

                  zerog0,
                  cphyregs]
_filter_inner = [canonregs,
                 regextract,
                 renameregs,

                 replaceret,
                 # killrestore,
                 detectregs,

                 #replsave,
                 
                 findlabels,
                 killunusedlabels,

                 xsplit,

                 protectallcallregs,
                 initspfp,


                 xjoin1,

                 decode,

                 markused,
                 markaligned,

                 makespecialtransform('tlsp', regmagic),
                 makespecialtransform('fp', regmagic),
                 makespecialtransform('ra', regmagic),
                 makespecialtransform('ra_leaf', regmagic),
                 renamelocals,

                 compress,

                 xjoin2,

                 #fillannulnop,

                 decode,
                 makedataflow(addswch = False, drainargs = True),

                 markdelay,
                 protectend,

                 zerog0,
                 phyregs]
_filter_end = [flattener, printer]


from ...common.asmproc import fixup

def filter(*args):
    return fixup.filter(_filter_begin, _filter_inner, _cfilter_inner, _filter_end, *args)
