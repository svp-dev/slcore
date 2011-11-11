import re

_re_lbl = re.compile(r'(.LL?\d+)')
def findlabels(fundata, items):
    """
    Detect which basic block labels (of the form [.$]LNNN) are
    being used, and which are being defined.
    """
    deflabels = set()
    uselabels = set()
    for (type, content, comment) in items:
        yield (type, content, comment)
        if type == 'label':
            m = _re_lbl.match(content)
            #print "BLABLA", content
            if m is not None:
                #print "BLIBLI", content
                deflabels.add(m.group(1))
        else:
            for m in _re_lbl.finditer(content):
                uselabels.add(m.group(1))
    fundata['deflabels'] = deflabels
    fundata['uselabels'] = uselabels


def killunusedlabels(fundata, items):
    """
    Remove basic block labels that are not used.

    This is needed to reduce the number of "nop" and "swch"
    annotations introduced by various passes.
    """
    #print 'def = ', fundata['deflabels']
    #print 'used = ', fundata['uselabels']
    unused = fundata['deflabels'] - fundata['uselabels']
    for (type, content, comment) in items:
        if type == 'label':
            lbl = content[:-1]
            if lbl in unused:
                yield ('empty','','MT: unused label: %s' % lbl)
                continue
        yield (type, content, comment)

