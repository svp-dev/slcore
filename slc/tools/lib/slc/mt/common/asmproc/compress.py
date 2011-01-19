
def makespecialtransform(special, regmagic):
    """
    Create a filter that renames a special register
    to the first free lower position in the local window (if any).

    FIXME: revisit when function calls are performed within the same context
    (then renaming is not possible)
    """
    nr = regmagic.get_vreg(special)['nr']
    def dotransform(fundata, items):
        regname = '$l%d' % nr
        rmask = fundata['usedregs']
        dorename = False
        newnr = nr
        if rmask[nr] == True and False in rmask[:nr] and not fundata['hasjumps']:
            freenr = [i for (i,r) in enumerate(rmask) if r == False][0]
            newnr = freenr
            dorename = True
            newname = '$l%d' % freenr
            rmask[nr] = False
            rmask[freenr] = True
            yield ('empty','','MT: special "%s" renamed from %s to %s' % (special, regname, newname))
        fundata['specials'][special] = newnr
        for (type, content, comment) in items:
            if type == 'other' and dorename:
                content = content.replace(regname, newname)
            yield (type, content, comment)

    dotransform.__name__ = 'dotransform_%s' % special

    return dotransform

def compress(fundata, items):
    """
    Compress the local and global register
    windows based on which registers are actually used.

    FIXME: need to revisit when function calls are done locally.
    """

    regs = fundata['regs']
    rmask = fundata['usedregs']
    rfmask = fundata['usedfregs']
    rgmask = fundata['usedgl']
    rgfmask = fundata['usedglf']

    if not fundata['hasjumps']:
        # Compress integer locals
        lastfound = None
        for i in xrange(30, -1, -1):
            if rmask[i]:
                lastfound = i
                break
        if lastfound is None:
            newnr = 0
        else:
            newnr = lastfound + 1

        if newnr < regs[2]:
            yield ('empty','','MT: compressed integer locals from %d to %d' % (regs[2], newnr))
            regs[2] = newnr

    # Compress integer globals
    lastfound = None
    for i in xrange(30, -1, -1):
        if rgmask[i]:
            lastfound = i
            break
    if lastfound is None:
        newnr = 0
    else:
        newnr = lastfound + 1
    if newnr < regs[0]:
        yield ('empty','','MT: compressed integer globals from %d to %d' % (regs[0], newnr))
        regs[0] = newnr

    # Compress FP locals
    if not fundata['hasjumps']:
        lastfound = None
        for i in xrange(30, -1, -1):
            if rfmask[i]:
                lastfound = i
                break
        if lastfound is None:
            newnr = 0
        else:
            newnr = lastfound + 1
        if newnr < regs[5]:
            yield ('empty','','MT: compressed FP locals from %d to %d' % (regs[5], newnr))
            regs[5] = newnr

    # Compress FP globals
    lastfound = None
    for i in xrange(30, -1, -1):
        if rgfmask[i]:
            lastfound = i
            break
    if lastfound is None:
        newnr = 0
    else:
        newnr = lastfound + 1
    if newnr < regs[3]:
        yield ('empty','','MT: compressed FP globals from %d to %d' % (regs[3], newnr))
        regs[3] = newnr

    for (type, content, comment) in items:
        yield (type, content, comment)


