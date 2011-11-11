def prunenopend(fundata, items):
    """
    Remove "nop" and "fnop" instructions and "swch" annotations that
    immediately precede "end".
    """
    queue = []
    for (type, content, comment) in items:
        if type == 'empty':
            #pprint.pprint((type, content, comment, queue))
            yield (type, content, comment)
            continue
        elif type in ['other','directive']:
            #pprint.pprint((type, content, comment, queue))
            if content in ['nop', 'fnop', 'swch'] and 'DELAYSLOT' not in comment:
                queue.append((type, content, comment))
                continue
            elif content == 'end':
                if len(queue)>0:
                    yield ('empty','','MT: killed cleanup: %s' % ','.join((c[1] for c in queue)))
                queue = []
        for q in queue:
            yield q
        queue = []
        yield (type, content, comment)

    for q in queue:
        yield q
