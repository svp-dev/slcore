import re
import os
import sys
import subprocess
import slc_config
from StringIO import StringIO

from ..msg import die, log

# regex for C comments: 
re_com = r'/[*].*?[*]/|//.*?(?=\n)'
# regex for C braces:
re_br = r'(?P<braces>[{}])'
# regex for C strings:
re_str = r'''(?P<str>"(?:[^\\"]|\\.)*"|['](?:[^\\']|\\.)*['])'''
# regex for whitespace
re_ws = r'(?P<ws>\s+)'
# regex for line directives
re_line = r'#(?:[ \t]*line|)[ \t]+(?P<line>[0-9]+)[ \t]+"(?P<file>.*?)".*?(?=\n)'
# regex for id
re_id = r'(?P<id>[a-zA-Z_][$a-zA-Z0-9_]*)' 
re_all = r'%s|%s|%s|%s|%s|(?P<rest>\(|[^{}\(\'\"\#a-zA-Z_]+|.)' % (re_br, re_str, re_ws, re_line, re_id)
re_allc = re.compile(re_all, re.S|re.M)

re_str_comments_nl = r'%s|(?P<com>%s)|(?P<nl>\n+)' % (re_str,re_com)
re_str_comments_nlc = re.compile(re_str_comments_nl, re.S|re.M)

def makeqesc(specialchars):
    specials = ''.join(specialchars)
    def escaper(str):
        if str[0] == '"':
            d = '""'
        else: 
            d = ''
        newstr = ''
        for c in str:
            if c in specials:
                newstr += r'\x%2x%s' % (ord(c),d)
            else:
                newstr += c;
        # print >>sys.stderr, "HELLO", newstr
        return newstr
    return escaper

class CM4(object):
    """
    Process the source.

    The following stages should be piped from each other:

    - combine()
    - remove_comments()
    - massage()
    - do_m4()
    """

    def __init__(self, m4_lquote = '`', m4_rquote = "'",
                 m4_file = "__file__", m4_line = "__line__",
                 sl_lbr = 'sl_lbr', sl_rbr = 'sl_rbr',
                 m4 = None,
                 verbose = False,
                 synclines = True):
        if m4 is None:
            m4 = os.getenv("M4", slc_config.m4)
        self.m4 = m4
        self.verbose = verbose
        self.synclines = synclines

        # SL braces
        self.sl_lbr = '%s%s%s%s%s' % (m4_lquote, m4_rquote,
                                      sl_lbr,
                                      m4_lquote, m4_rquote)

        self.sl_rbr = '%s%s%s%s%s' % (m4_lquote, m4_rquote,
                                      sl_rbr,
                                      m4_lquote, m4_rquote)

        # replacement for line directives
        self.repl_line = '%s(%s%%s%s)%s(%%d)' % (m4_file, m4_lquote, m4_rquote, m4_line)
        # replacement for quote chars in strings
        self.quotes_escaper = makeqesc(list(set(m4_lquote + m4_rquote)))
        # replacement for strings
        self.quoted_string_fmt = "%s%%s%s" % (m4_lquote, m4_rquote)

    def _combine_gen(self, input_files):
        for fname in input_files:
            if fname == '-':
                title = '<stdin>'
                f = sys.stdin
            else:
                title = fname
                f = file(fname, 'r')
            
            if self.synclines:
                yield '\n# 1 "%s"\n' % title
            yield f.read()
            if fname != '-': f.close()

    def combine(self, input_files):
        """
        collapses all input files into one string
        """
        return ''.join(self._combine_gen(input_files))

    def _com_repl(self, matchobj):
        # helper for remove_comments() below
        nl = matchobj.group('nl')
        if nl is not None:
            data = '\n' * (self.acc_nl + len(nl))
            self.acc_nl = 0
            return data
        com = matchobj.group('com')
        if com is not None:
            self.acc_nl += com.count('\n')
            return ' '
        return matchobj.group(0)

    def remove_comments(self, data):
        """
        replaces comments by whitespace, but keep
        the number of newline characters right (for error reporting)
        """
        self.acc_nl = 0
        data = re.sub(re_str_comments_nlc, self._com_repl, data)
        data += '\n' * self.acc_nl
        return data


    def _wrepl(self, matchobj):
        # helper for massage() below

        if matchobj.group('line') is not None:
            # log("LINE")
            lineno = matchobj.group('line')
            filename = matchobj.group('file')
            self.acc_ws.append('\n' * matchobj.group(0).count('\n'))
            if self.synclines: self.acc_ws.append(self.repl_line % (filename, int(lineno)-1))
            return ""

        ws = matchobj.group('ws')
        if ws is not None:
            # log("WS: %r" % ws)
            self.acc_ws.append(ws)
            return ""

        id = matchobj.group('id')
        if id is not None:
            # log("ID: %r" % id)
            self.last_seen = 'id'
            self.acc_ws.append(id)
            data = ''.join(self.acc_ws)
            self.acc_ws = []
            return data

        str = matchobj.group('str')
        if str is not None:
            # log("STR: %r" % str);
            self.last_seen = 'rest'
            self.acc_ws.append(self.quoted_string_fmt % self.quotes_escaper(str))
            data = ''.join(self.acc_ws)
            self.acc_ws = []
            return data

        br = matchobj.group('braces')
        if br is not None:
            self.last_seen = 'rest'
            if br == '{':
                p = self.sl_lbr
            else: 
                p = self.sl_rbr

            self.acc_ws.append(p)
            data = ''.join(self.acc_ws)
            self.acc_ws = []
            return data

        rest = matchobj.group(0)
        # log("REST: %r" % rest)
        if not (self.last_seen == 'id' and rest.startswith('(')):
            self.acc_ws.append(rest)
            data = ''.join(self.acc_ws)
            self.acc_ws = []
        else:
            data = rest
        self.last_seen = 'rest'
        return data


    def massage(self, data):
        """
        replace braces by keywords and remove
        whitespace between identifiers and opening parentheses.
        """
        self.acc_ws = []
        self.last_seen = None
        data = re.sub(re_allc, self._wrepl, data)
        self.acc_ws.insert(0, data)
        res = ''.join(self.acc_ws)
        self.acc_ws = None
        return res
    
    

    def do_m4(self, data, m4_args = [], tracefile = None):
        """
        runs M4 and returns the result
        """

        # prepare command line
        m4_cmdline = [self.m4, '-E'] # -E = fail on error
        if self.synclines: m4_cmdline.append('-s')
        if tracefile is not None: 
            m4_cmdline.append('-dV')
            m4_cmdline.append('--debugfile=%s' % tracefile)
        m4_cmdline += m4_args
        m4_cmdline.append('-')

        if self.verbose: log("exec m4: %s" % repr(m4_cmdline))

        # do the work
        m4p = subprocess.Popen(m4_cmdline, 
                               stdin=subprocess.PIPE, 
                               stdout=subprocess.PIPE)

        (o, _) = m4p.communicate(input = data)
        if m4p.returncode != 0:
            die("m4 returned exit status %d" % m4p.returncode)

        return o


if __name__ == "__main__":
    c = CM4()
    files = sys.argv[1:]
    print "### COMBINE ###"
    d = c.combine(files)
    print d
    print "### REMOVE COMMENTS ###"
    d = c.remove_comments(d)
    print d
    print "### MASSAGE ###"
    d = c.massage(d)
    print d
    print "### M4 ###"
    d = c.do_m4(d)
    print d
