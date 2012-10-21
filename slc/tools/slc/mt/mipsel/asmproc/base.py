import re
from ..regdefs import regmagic

def grouper(items):
    """
    Group function code together (when applicable).
    Produce a generator of (type, code, comment).

    For functions, emit a single item with type == "fun". In this case
    code is a dictionary with keys:
    - "name" : name of the function
    - "body" : stream of instructions
    """
    
    state = 0
    queue = []
    name = None
    globl = False

    for (type, content, comment) in items:
        if state == 0 and content.startswith('.global'):
            name = content.split(' ')[1]
            queue.append((type, content, comment))
            state = 1
            globl = True
            continue
        if state in [0, 1] and content.startswith('.type') and content.endswith('function'):
            maybe_name = content.split(None)[1].split(',')[0]
            if state == 1 and maybe_name != name:
                # .global seen before, with different name
                # so reinit.
                globl = False
                for q in queue: yield q
                queue = []
            name = maybe_name
            queue.append((type, content, comment))
            state = 2
            continue
        if state == 2:
            # expect: stuff, then .size
            queue.append((type, content, comment))
            if (not content.startswith('.size')) or content.split(None)[1].split(',')[0] != name:
                continue
            state = 0
            if name.startswith('__slf_'):
                funtype = 'fun'
                name = name[6:]
            else:
                funtype = 'cfun'
            yield (funtype, {'name':name,
                             'body':queue,
                             'global':globl,
                             'specials':{}}, '')
            globl = False
            queue = []
            continue

        for q in queue:
            yield q
        queue = []
        state = 0
        yield (type, content, comment)
    
from common import *
from ...common.asmproc.regextract import *
from ...common.asmproc.renameregs import *

_filter_begin = [reader, lexer, splitsemi, parser, grouper]
_cfilter_inner = []
_filter_inner = []
_filter_end = [flattener, forcezero, printer]


from ...common.asmproc import fixup

def filter(*args):
    return fixup.filter(_filter_begin, _filter_inner, _cfilter_inner, _filter_end, *args)
