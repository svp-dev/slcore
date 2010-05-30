###
### Common definitions for base and optimizing
### asmproc stages
###

from ...msg import die
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

_re_ws = re.compile(r'\s+')
_re_codecom = re.compile(r'''((?:"(?:[^\\"]|\\.)*"|'(?:\\.|.)|[^#])*)((#.*)?)''')
def lexer(lines):
    """
    Split code from comments, produce a generator of (code, comment).
    """
    for line in lines:
        m = _re_codecom.match(line)
        asm = m.group(1)
        comment = m.group(2)
        if comment is None:
            comment = ''
        else:
            comment = comment[1:]
        yield (asm, comment)


_re_splitsemi = re.compile(r''';|(?P<rest>(?:"(?:[^\\"]|\\.)*"|'(?:\\.|.)|[^;])+)''')
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

_re_directive = re.compile(r'\.[a-z]+')

def parser(tokens):
    """
    Recognize assembly lines, produce a generator of (type, code, comment).
    Type can be "directive", "empty", "label", "other".
    """
    for (asm, comment) in tokens:
       if not asm:
           yield ('empty', asm, comment)
       elif _re_label.match(asm):
           yield ('label', asm, comment)
       elif _re_directive.match(asm):
           yield ('directive', asm, comment)
       else:
           yield ('other', asm, comment)
                    
def funfilter(filter, items):
    for (type, content, comment) in items:
        if type == 'fun':
            #print "XXXXXXXXXXXXXXXXXXXXXX"
            #print filter
            #pprint.pprint(content)
            content['body'] = [x for x in filter(content, content['body'])]
            #pprint.pprint(content)
        yield (type, content, comment)

def cfunfilter(filter, items):
    for (type, content, comment) in items:
        if type == 'cfun':
            #print "XXXXXXXXXXXXXXXXXXXXXX"
            #print filter
            #pprint.pprint(content)
            content['body'] = [x for x in filter(content, content['body'])]
            #pprint.pprint(content)
        yield (type, content, comment)

def flattener(items):
    """
    Flatten function code back into single stream.
    Produce a generator of (type, code, comment)
    """
    for (type, content, comment) in items:
        if type in ["fun", "cfun"]:
            for i in content['body']:
                yield i
        else:
            yield (type, content, comment)

def forcezero(items):
    for (type, content, comment) in items:
        content = content.replace('$l31','$31').replace('$lf31','$f31')
        yield (type, content, comment)

def printer(items):
    for (type, content, comment) in items:
        if comment:
            tail = "#%s" % comment
            fmt1 = '%-40s\t'
            fmt2 = '\t%-32s\t'
        else:
            tail = ""
            fmt1 = '%s'
            fmt2 = '\t%s'
        if type == "label":
            head = fmt1 % content
        elif content:
            head = fmt2 % content
        else:
            head = ''
        yield '%s%s\n' % (head, tail)
