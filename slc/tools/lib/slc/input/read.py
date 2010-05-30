from ..msg import die

def cleanup(source):
    # remove extra whitespace

    if isinstance(source, list):
        return [x for x in (cleanup(y) for y in source) if len(x) != 0]
    elif isinstance(source, str):
        return source.strip(' \t')
    elif isinstance(source, dict):
        return dict( ( (k, cleanup(v)) for (k,v) in source.iteritems() ) )
    else: 
        die("unknown item type: %r" % source)

def read(file):
    
    # FIXME: insert invocation of cm4 here.

    source = eval('["""'+file.read()+'"""]')

    source = cleanup(source)

    return source
