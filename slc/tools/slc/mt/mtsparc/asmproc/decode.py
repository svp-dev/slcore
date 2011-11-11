import re
from opc.mtsparc import insninfo
from ....msg import warn

class insnstr(str, object):
    pass

_insnre = re.compile('\s*(\S+)(.*)$')
def decode(fundata, items):
    name = fundata['name']
    for (type, content, comment) in items:
        if type == 'other':
            content = insnstr(content)
            content.mnemonic = None
            content.metadata = None
            content.format_re = None
            content.operands = None

            m = _insnre.match(content)
            if m is None:
                warn('%s: cannot decode: %s' % (name, content))
            else:
                insn = m.group(1)
                rest = m.group(2)

                candidates = insninfo.get(insn, None)
                if candidates is None:
                    if insn not in ['end','swch']:
                        warn('%s: unrecognized mnemonic: %s' % (name, insn))
                else:
                    found = None
                    for (parser, info) in candidates:
                        found = parser.match(rest)
                        if found is not None:
                            break
                    if found is None:
                        warn('%s: unrecognized instruction format: %s' % (name,content))
                    else:
                        # instruction was recognized OK
                        content.mnemonic = insn
                        content.metadata = info
                        content.format_re = parser
                        content.operands = found.groups()
        yield (type, content, comment)
