import re

_re_li = re.compile(r'\$l(\d+)')
_re_lf = re.compile(r'\$lf(\d+)')
_re_gi = re.compile(r'\$g(\d+)')
_re_gf = re.compile(r'\$gf(\d+)')
def markused(fundata, items):
    """
    Collect which local and global registers are currently in use in
    the function body.
    """
    rmask = [False] * 32
    rfmask = [False] * 32
    rgmask = [False] * 32
    rgfmask = [False] * 32
    for (type, content, comment) in items:
        if type == 'other':
            for rm in _re_li.finditer(content):
                n = int(rm.group(1))
                rmask[n] = True
            for rm in _re_lf.finditer(content):
                n = int(rm.group(1))
                rfmask[n] = True
            for rm in _re_gi.finditer(content):
                n = int(rm.group(1))
                rgmask[n] = True
            for rm in _re_gf.finditer(content):
                n = int(rm.group(1))
                rgfmask[n] = True
        yield (type, content, comment)
    fundata['usedregs'] = rmask
    fundata['usedfregs'] = rfmask
    fundata['usedgl'] = rgmask
    fundata['usedglf'] = rgfmask
