
def rmswchdir(fundata, items):
    """
    Remove .swch and .noswch.
    """
    for (type, content, comment) in items:
        if type == 'directive' and content in [".swch", ".noswch"]:
            continue
        yield (type, content, comment)

def rmdupswch(fundata, items):
    """
    Remove duplicate "swch" annotations.
    """
    seeswch = 0
    for (type, content, comment) in items:
        if type == 'other':
            if content.startswith('swch'):
                if seeswch == 0:
                    seeswch = 1
                    yield (type, content, comment)
            else:
                seeswch = 0
                yield (type, content, comment)
        else:
            yield (type, content, comment)

def rmswchbegin(fundata, items):
    """
    Remove "swch" annotations immediately following a label.
    (including at the very start of a function, where it is invalid!)
    """
    seeother = False
    for (type, content, comment) in items:
        if type == 'label':
            seeother = False
        elif type == 'other':
            if content.startswith('swch') and seeother == False:
                continue
            seeother = True
        yield (type, content, comment)

