import re
import common

_default_presets = set(("$%d" % x for x in xrange(31)))
def grouper(items):
    """
    Group function code together.
    Produce a generator of (type, code, comment).
    """

    presets = _default_presets.copy()
    presets.add("$l0")

    state = 0
    queue = []
    for (type, content, comment) in items:
        if state == 0:
            if content.startswith('.type') and content.endswith("#function"):
                name = content.split(None)[1].split(',')[0]
                state = 1
                queue.append((type, content, comment))
                continue
        else:
            queue.append((type, content, comment))
            if (not content.startswith('.size')) or content.split(None)[1].split(',')[0] != name:
                continue
            yield ('fun',{'name':name,'body':queue,'presets':presets},'')
            queue = []
            state = 0
            continue
        for q in queue:
            yield q
        queue = []
        state = 0
        yield (type, content, comment)

_re_vname = re.compile(r'%(r|t[dsgl])(f?\d+)')
_re_canon = re.compile(r'\$([dsgl]?f?\d+)')
def vnametocanon(fundata, items):
        def repl(match):
                t = match.group(1)
                n = match.group(2)
                t = t[1:]
                return '$%s%s' % (t, n)
                
        for (type,content,comment) in items:
                content = _re_vname.sub(repl, content)
                comment = _re_vname.sub(repl, comment)
                yield (type, content, comment)
        

def canontovname(fundata, items):
        def repl(match):
                t = match.group(1)
                if t[0] in "dsgl":
                        p = 't'
                else:
                        p = 'r'
                return '%%%s%s' % (p, t)
        for (type,content,comment) in items:
                content = _re_canon.sub(repl, content)
                comment = _re_canon.sub(repl, comment)
                yield (type, content, comment)


from ...common.asmproc.remdefaults import *
from ...common.asmproc.labels import *
from ...common.asmproc.swch import *
from ...common.asmproc.end import *

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

        rmdupswch,
        rmswchbegin,

        prunenopend,

        # protectend,

        canontovname,
        ]
_filter_end = [common.flattener,
               common.printer]
                 


from ...common.asmproc import enhance

def filter(*args):
	return enhance.filter(_filter_begin, _filter_stages, _filter_end, *args)

