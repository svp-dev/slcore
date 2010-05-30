import re
from msg import die

_ws = r'(?:\s|#line\s[^\n]*\n)*'

_re_identifier = re.compile('^' + _ws + r'([a-zA-Z_][a-zA-Z0-9_]*)' + _ws + '$', re.S)

# _re_ws = re.compile('^' + _ws + '$', re.S)
# _re_endswithline = re.compile(r'.*#line\s+\d+(?:\s+"[^"]*")?\s+$', re.S)

def is_simple_identifier(text):
    m = _re_identifier.match(text)
    if m is None: return (False, None)
    else: return (True, m.group(1))

def extract_id(context, name):
    (t, i) = is_simple_identifier(name)
    if not t: die("invalid identifier: '%s'" % name, context)
    return i
