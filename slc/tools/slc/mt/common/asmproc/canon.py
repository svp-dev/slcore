
def canonregs(items, regmagic):
    """
    Rename all legacy register aliases (%rN, %oN, $rN, ...)  to a
    common numerical-only form $N/$fN.
    """
    
    repl = regmagic.makelegcanonrepl()
    re_reg = regmagic.makelegre()
    for (type, content, comment) in items:
        content = re_reg.sub(repl, content)
        comment = re_reg.sub(repl, comment)
        yield (type, content, comment)
