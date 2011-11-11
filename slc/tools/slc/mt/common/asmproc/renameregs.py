
import re
_re_rr1reg = re.compile('(\$f?\d+)')
def renameregs_gen(fundata, items, regmagic):
    """
    Convert all legacy register names to their virtual
    counterparts. 
    """
    rd = fundata['regs']
    repl = regmagic.makerepl(rd[0],rd[1],rd[3],rd[4])
    for (type, content, comment) in items:
        content = _re_rr1reg.sub(repl, content)
        comment = _re_rr1reg.sub(repl, comment)
        yield (type, content, comment)

def crenameregs_gen(fundata, items, regmagic):
    """
    Collate legacy register names in regular C function
    bodies to the beginning of the register window.
    """
    repl = regmagic.makecrepl(fundata['name'])
    for (type, content, comment) in items:
        content = _re_rr1reg.sub(repl, content)
        comment = _re_rr1reg.sub(repl, comment)
        yield (type, content, comment)
