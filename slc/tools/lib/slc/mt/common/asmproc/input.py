
from ....msg import die
import re

def reader(inname):
    """
    Read a file line by line, produce a generator of lines.
    """
    try:
        inf = file(inname)
    except Exception, e:
        die('%s: %r' % (inname, e))
    for line in (line.rstrip() for line in inf):
        yield line
    inf.close()

def lexer(lines, re_codecom):
    """
    Split code from comments, produce a generator of (code, comment).
    """
    for line in lines:
        m = re_codecom.match(line)
        asm = m.group(1)
        comment = m.group(2)
        if comment is None:
            comment = ''
        else:
            comment = comment[1:]
        yield (asm, comment)

_re_splitsemi = re.compile(r''';|(?P<rest>(?:"(?:[^\\"]|\\.)*"|'(?:\\.|.)'|[^;])+)''')
_re_label = re.compile(r'\S+?:')
def splitsemi(tokens):
    """
    Split lines at semicolons and labels
    """
    for (asm, comment) in tokens:
        done = False
        for m in _re_splitsemi.finditer(asm):
            rest = m.group('rest')
            if rest is not None:
                rest = rest.strip()
                if rest:
                    m = _re_label.match(rest)
                    if m is not None:
                        lbl, more = rest.split(':',1)
                        yield (lbl + ':', comment)
                        if more:
                            yield (more.strip(), comment)
                    else:
                        yield (rest, comment)
                    done = True
        if not done and len(comment) > 0:
            yield ('', comment)

def parser(tokens, re_directive):
    """
    Recognize assembly lines, produce a generator of (type, code, comment).
    Type can be "directive", "empty", "label", "other".
    """
    for (asm, comment) in tokens:
       if not asm:
           yield ('empty', asm, comment)
       elif _re_label.match(asm):
           yield ('label', asm, comment)
       elif re_directive.match(asm):
           yield ('directive', asm, comment)
       else:
           yield ('other', asm, comment)

