import os
import subprocess
import sys
from ...msg import die, log

class Compile:

    def __init__(self, regmagic):
        self.regmagic = regmagic

    def showregs(self, x):
        rm = self.regmagic.rd.reg_mapping
        if x != 'f':
            print ','.join((rm.get('l%d'%x,'.') for x in xrange(32)))
        else:
            print ','.join((rm.get('lf%d'%x,'.') for x in xrange(32)))

    def __call__(self, compiler, *args):
        """ Run the compiler, adding special flags to fix registers """

        # Obey suggestions from the register magic module
        fargs = ['-ffixed-%s' % x for x in self.regmagic.fixed_registers]
        #fargs += ['-fcall-used-%s' % x for x in regmagic.call_used_registers]

        cmd = [compiler] + fargs + list(args) # + ['-fverbose-asm']
        if os.getenv('SLC_VERBOSE', 'no') == 'yes':
            log('running: %s' % ' '.join(cmd))
        try:
            retcode = subprocess.call(cmd)
            sys.exit(retcode)
        except Exception, e:
            #log(' '.join(cmd))
            die('%s: %r' % (compiler, e), code = 126)
