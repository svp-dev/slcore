class Loc:
    def __init__(self, filename, lineno = None, context = None):
        if filename is None:
            filename = '<unknown>'
        self.filename = filename

        if lineno is None or lineno < 1:
            lineno = -1
        self.lineno = int(lineno)

        self.context = context

    def __str__(self):
        if self.context is not None:
            ctx = ' (%s)' % self.context
        else: 
            ctx = ''
        if self.lineno >= 0:
            ll = ':%d' % self.lineno
        else:
            ll = ''
        return '%s%s%s' % (self.filename, ctx, ll)

    def __repr__(self):
        return 'Loc(%s)' % ', '.join(('%s = %r' % (k, getattr(self, k)) 
                                     for k in ['filename','lineno','context']
                                     if getattr(self, k) is not None))

    def ppline(self):
        if self.context is not None:
            ctx = ' (%s)' % self.context
        else:
            ctx = ''
        return '#line %d "%s%s"' % (self.lineno, self.filename, ctx)


def splitloc(locinfo):
    f, l = locinfo.rsplit(':', 1)
    return Loc(f, l)
