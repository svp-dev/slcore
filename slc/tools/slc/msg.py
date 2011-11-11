import sys
import loc

def log(msg, context = None, stream = sys.stderr):
    if isinstance(context, loc.Loc):
        l = context
    elif hasattr(context, 'loc'):
        l = context.loc
    elif isinstance(context, dict) and context.has_key('loc'):
        l = context['loc']
    elif isinstance(context, str) or hasattr(context, '__str__'):
        l = context

    if l is None:
        l = sys.argv[0]

    print >>stream, '%s:' % l, msg

def die(msg, context = None, code = 1):
    log(msg, context)
    sys.exit(code)

def warn(msg, context = None):
    log('warning: %s' % msg, context)

def info(msg, context = None):
    log(msg, context, stream = sys.stdout)


