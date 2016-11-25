
import re

_re_regs = re.compile(r'.registers (\d+) (\d+) (\d+) (\d+) (\d+) (\d+)')
def regextract(fundata, items):
    """
    Find the '.registers' directive in the function body,
    remove it from the code and cache the values for future stages.

    (the '.registers' directive is re-generated later by xjoin2)
    """
    for (type, content, comment) in items:
        m = _re_regs.match(content)
        if m is not None:
            assert not fundata.has_key('regs')
            fundata['regs'] = [int(x) for x in m.groups()]
            yield ('empty', '', 'MT: END PROLOGUE')
            continue

        yield (type, content, comment)
