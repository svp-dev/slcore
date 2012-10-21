###
### Common definitions for base and optimizing
### asmproc stages
###

import re

from ...common.asmproc import input
from ...common.asmproc import output
from ...common.asmproc.filter import *

#_re_ws = re.compile(r'\s+')
_re_codecom = re.compile(r'''((?:"(?:[^\\"]|\\.)*"|'(?:\\.|.)|[^#])*)((#.*)?)''')
_re_directive = re.compile(r'\.[a-z]+')

def lexer(lines):
    return input.lexer(lines, _re_codecom)

def parser(tokens):
    return input.parser(tokens, _re_directive)

reader = input.reader
splitsemi = input.splitsemi

def forcezero(items):
    for (type, content, comment) in items:
        content = content.replace('$l31','$31').replace('$lf31','$f31')
        yield (type, content, comment)

def printer(items):
    return output.printer(items, '#')

flattener = output.flattener

from ..regdefs  import regmagic
from ...common.asmproc import canon

def canonregs(fundata, items):
    return canon.canonregs(items, regmagic)
