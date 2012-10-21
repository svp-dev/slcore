from ....msg import die, log
from ..regdefs import regmagic
from ....front import opts, dump
import  common

def grouper(items):
    """
    Group function code together.
    Produce a generator of (type, code, comment).
    """

    state = 0
    queue = []
    for (type, content, comment) in items:
        if state == 0:
            if content.startswith('.type') and content.endswith("function"):
                name = content.split(None)[1].split(',')[0]
                state = 1
                queue.append((type, content, comment))
                continue
        else:
            queue.append((type, content, comment))
            if (not content.startswith('.size')) or content.split(None)[1].split(',')[0] != name:
                continue
            yield ('fun',{'name':name,'body':queue},'')
            queue = []
            state = 0
            continue
        for q in queue:
            yield q
        queue = []
        state = 0
        yield (type, content, comment)

def noreorder(fundata,items):
    """
    .set reorder screws up delay branches. Remove it.
    """
    for (type, content, comment) in items:
        if type == 'directive':
            if "set" in content and "reorder" in content:
                yield(type, ".set noreorder", "MT: forced")
                continue
        yield (type, content, comment)

_filter_begin = [common.reader, 
                 common.lexer, 
                 common.splitsemi, 
                 common.parser, 
                 grouper]
_filter_stages = [noreorder]

_filter_end = [common.flattener,
               common.forcezero,
               common.printer]

from ...common.asmproc import enhance

def filter(*args):
    return enhance.filter(_filter_begin, _filter_stages, _filter_end, *args)

