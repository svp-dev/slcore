def markdelay(fundata, items):
    """
    Mark delayed instructions and delay slots.
    """
    indelayslot = 0
    for (type, content, comment) in items:
        if type == 'directive':
            indelayslot = 0
        elif type == 'other':
            comment = comment.replace('DELAYED','').replace('DELAYSLOT','')
            if getattr(content, 'metadata',None) and content.metadata.delayed:
                if indelayslot == 1: die("%s: delayed instruction in delay slot is not supported" % fundata['name'])
                indelayslot = 1
                if 'DELAYED' not in comment:
                    comment = comment + ' DELAYED'
            else:
                if indelayslot == 1:
                    # this is the delay slot itself
                    if 'DELAYSLOT' not in comment:
                        comment = comment + ' DELAYSLOT'
                    indelayslot = 2
                elif indelayslot == 2:
                    # this is the instruction just after the delay slot
                    indelayslot = 0
        yield (type, content, comment)
