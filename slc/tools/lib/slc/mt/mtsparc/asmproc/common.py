import re

from ...common.asmproc import input
from ...common.asmproc import output
from ...common.asmproc.filter import *

#_re_ws = re.compile(r'\s+')
_re_codecom = re.compile(r'''((?:"(?:[^\\"]|\\.)*"|'(?:\\.|.)'|[^!])*)((?:!.*)?)''')
_re_directive = re.compile(r'\.[a-z]+')

def lexer(lines):
    return input.lexer(lines, _re_codecom)

def parser(tokens):
    return input.parser(tokens, _re_directive)

reader = input.reader
splitsemi = input.splitsemi


def printer(items):
    return output.printer(items, '!')

flattener = output.flattener

from ..regdefs  import regmagic
from ...common.asmproc import canon

def canonregs(fundata, items):
    return canon.canonregs(items, regmagic)

_re_creg = re.compile('(\$f?\d+)')
def cphyregs(fundata, items):
    """
    Convert $N/$fN registers names
    to SPARC %rN/%fN names.
    """
    def repl(match):
        rname = match.group(1)
        if rname[1] == 'f':
            return '%%%s' % rname[1:]
        else:
            return '%%r%s' % rname[1:]
    for (type, content, comment) in items:
        content = _re_creg.sub(repl, content)
        comment = _re_creg.sub(repl, comment)
        yield (type, content, comment)

_re_reg = re.compile('(\$[glsd]f?\d+)')
def phyregs(fundata, items):
    """
    Convert $[glsd]N/$[glsd]fN registers names
    to SPARC %t[glsd]N/%t[glsd]fN names.
    """
    def repl(match):
        rname = match.group(1)
        return '%%t%s' % rname[1:]

    for (type, content, comment) in items:
        content = _re_reg.sub(repl, content)
        comment = _re_reg.sub(repl, comment)
        yield (type, content, comment)
