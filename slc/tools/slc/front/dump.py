import pprint
from ..front import opts 

_counter = 1

def get_dump_fname(where):
    if where not in opts.resolved.dump_stages and \
            "all" not in opts.resolved.dump_stages:
        return None
    base = opts.resolved.output
    if base is None or base == '-':
        base = "tmp"

    global _counter
    fname = "%s.%03d.%s" % (base, _counter, where)
    _counter += 1
    return fname
    

def dump(where, data):
    fname = get_dump_fname(where)
    if fname is not None:
        f = file(fname, "w")
        f.write(data)
        f.close()

def dump_gen(where, data):
    fname = get_dump_fname(where)
    if fname is None:
        res = data
    else:
        res = []
        f = file(fname, "w")
        for item in data:
            pprint.pprint(item, stream = f)
            res.append(item)
        f.close()
    return res


