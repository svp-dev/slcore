
def flattener(items):
    """
    Flatten function code back into single stream.
    Produce a generator of (type, code, comment)
    """
    for (type, content, comment) in items:
        if type in ["fun", "cfun"]:
            for i in content['body']:
                yield i
        else:
            yield (type, content, comment)


def printer(items, commentprefix):
    for (type, content, comment) in items:
        if comment:
            tail = "%s%s" % (commentprefix, comment)
            fmt1 = '%-40s\t'
            fmt2 = '\t%-32s\t'
        else:
            tail = ""
            fmt1 = '%s'
            fmt2 = '\t%s'
        if type == "label":
            head = fmt1 % content
        elif content:
            head = fmt2 % content
        else:
            head = ''
        yield '%s%s\n' % (head, tail)
