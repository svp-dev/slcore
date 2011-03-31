import re

_re_sb = re.compile(r'setstart \$l?\d+,\s*0\s*$')
_re_sl = re.compile(r'setlimit \$l?\d+,\s*1\s*$')
_re_ss = re.compile(r'setstep \$l?\d+,\s*1\s*$')
_re_sbl = re.compile(r'setblock \$l?\d+,\s*0\s*$')
def remdefaults(fundata, items):
    """
    Remove MT control instructions that configure default
    values.
    """
    justignored = False
    for (type, content, comment) in items:
        if type == 'other' and \
                 (_re_sb.match(content) is not None \
                     or _re_sl.match(content) is not None \
                     or _re_ss.match(content) is not None \
                     or _re_sbl.match(content) is not None):
                # default setting, ignore
            yield ('empty','','MT: "%s" ignored (uses default) # %s' % (content,comment))
            justignored = True
            continue
        elif type == 'other' and justignored and content == 'swch':
            yield ('empty','','MT: swch ignored too # %s' % comment)
            continue
        justignored = False
        yield (type, content, comment)
