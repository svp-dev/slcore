import os
import subprocess
import sys
from ..msg import die, log
import regmagic

def compile(compiler, *args):
    """ Run the compiler, adding special flags to fix registers """

    # Obey suggestions from the register magic module
    fargs = ['-ffixed-%s' % x for x in regmagic.fixed_registers]
    fargs += ['-fcall-used-%s' % x for x in regmagic.call_used_registers]
    
    cmd = [compiler] + fargs + list(args) # + ['-fverbose-asm']
    if os.getenv('SLC_VERBOSE', 'no') == 'yes':
        log('running: %s' % ' '.join(cmd))
    try:
        retcode = subprocess.call(cmd)
        sys.exit(retcode)
    except Exception, e:
        #log(' '.join(cmd))
        die('%s: %r' % (compiler, e), code = 126)
