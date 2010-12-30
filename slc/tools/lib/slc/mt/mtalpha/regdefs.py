from ..common.regmagic import RegMagic

class RegDefs:

    # Magic numbers
    iargregs = 12
    fargregs = 12
    ilocalregs = 19
    flocalregs = 19

    regprefix = '$'
    regprefix_fmt = '$'

    canon_is_numeric = True
    comprefix = '#'

    legacy_fregs = {
       # Scratch registers
       'ft0' : 10,
       'ft1' : 11,
       'ft2' : 12,
       'ft3' : 13,
       'ft4' : 14,
       'ft5' : 15,
       'ft6' : 22,
       'ft7' : 23,
       'ft8' : 24,
       'ft9' : 25,
       'ft10' : 26,
       'ft11' : 27,
       'ft12' : 28,
       'ft13' : 29,
       'ft14' : 30,
       # Return value registers
       'frv' : 0,
       'frv2' : 1,
       # Incoming parameters
       'fa0' : 16,
       'fa1' : 17,
       'fa2' : 18,
       'fa3' : 19,
       'fa4' : 20,
       'fa5' : 21,
       # Call-save registers
       'fs0' : 2,
       'fs1' : 3,
       'fs2' : 4,
       'fs3' : 5,
       'fs4' : 6,
       'fs5' : 7,
       'fs6' : 8,
       'fs7' : 9,
       # Zero 
       'fzero' : 31,
       }

    legacy_regs = {
       # function temporaries
       't0' : 1,
       't1' : 2,
       't2' : 3,
       't3' : 4,
       't4' : 5,
       't5' : 6,
       't6' : 7,
       't7' : 8,
       't8' : 22,
       't9' : 23,
       't10' : 24,
       't11' : 25,
       # assembly scratch
       'at' : 28,
       # return value
       'rv' : 0,
       # incoming parameters
       'a5' : 21,
       'a4' : 20,
       'a3' : 19,
       'a2' : 18,
       'a1' : 17,
       'a0' : 16,
       # procedure function / temporary
       'pv' : 27,
       't12' : 27,
       # Callee-save registers
       's0' : 9,
       's1' : 10,
       's2' : 11,
       's3' : 12,
       's4' : 13,
       's5' : 14,
       # Return address
       'ra' : 26,
       # Frame pointer / callee save
       'fp' : 15,
       's6' : 15,
       # Global pointer
       'gp' : 29,
       # Stack pointer
       'sp' : 30,
       # Zero
       'zero' : 31
       }


    ######################
    # GCC Allocation order
    ######################
    # #define REG_ALLOC_ORDER {
    #    1, 2, 3, 4, 5, 6, 7, 8,      /* nonsaved integer registers */
    #    22, 23, 24, 25, 28,          /* likewise */
    #    0,                           /* likewise, but return value */
    #    21, 20, 19, 18, 17, 16,      /* likewise, but input args */
    #    27,                          /* likewise, but OSF procedure value */
    #    f10, f11, f12, f13, f14, f15,      /* nonsaved floating-point registers */
    #    f22, f23, f24, f25, f26, f27,      /* likewise */
    #    f28, f29, f30,                  /* likewise */
    #    f0, f1,                      /* likewise, but return values */
    #    f21, f20, f19, f18, f17, f16,      /* likewise, but input args */
    #    9, 10, 11, 12, 13, 14,       /* saved integer registers */
    #    26,                          /* return address */
    #    15,                          /* hard frame pointer */
    #    f2, f3, f4, f5, f6, f7,      /* saved floating-point registers */
    #    f8, f9,                      /* likewise */
    #    29, 30, 31, f31               /* gp, sp, ap, sfp */
    # }


    reg_mapping = {
       # Globals
       'g0' : 't11',
       'g1' : 't10',
       'g2' : 't9',
       'g3' : 't8',
       'g4' : 't7',
       'g5' : 't6',
       'g6' : 't5',
       'g7' : 't4',
       'g8' : 't3',
       'g9' : 't2',
       'g10' : 't1',
       'g11' : 't0',

       'gf0' : 'ft11',
       'gf1' : 'ft10',
       'gf2' : 'ft9',
       'gf3' : 'ft8',
       'gf4'  : 'ft7',
       'gf5' : 'ft6',
       'gf6' : 'ft5',
       'gf7' : 'ft4',
       'gf8' : 'ft3',
       'gf9' : 'ft2',
       'gf10' : 'ft1',
       'gf11' : 'ft0',

       # Shareds
       's0' : 't0',
       'd0' : 't1',
       's1' : 't2',
       'd1' : 't3',
       's2' : 't4',
       'd2' : 't5',
       's3' : 't6',
       'd3' : 't7',
       's4' : 't8',
       'd4' : 't9',
       's5' : 't10',
       'd5' : 't11',

       'sf0' : 'ft0',
       'df0' : 'ft1',
       'sf1' : 'ft2',
       'df1' : 'ft3',
       'sf2' : 'ft4',
       'df2' : 'ft5',
       'sf3' : 'ft6',
       'df3' : 'ft7',
       'sf4' : 'ft8',
       'df4' : 'ft9',
       'sf5' : 'ft10',
       'df5' : 'ft11',

       'lf31' : 'fzero',

       'lf0' : 'ft12',
       'lf1' : 'ft13',
       'lf2' : 'ft14',
       'lf3' : 'frv',
       'lf4' : 'frv2',
       'lf5' : 'fa5',
       'lf6' : 'fa4',
       'lf7' : 'fa3',
       'lf8' : 'fa2',
       'lf9' : 'fa1',
       'lf10' : 'fa0',
       'lf11' : 'fs0',
       'lf12' : 'fs1',
       'lf13' : 'fs2',
       'lf14' : 'fs3',
       'lf15' : 'fs4',
       'lf16' : 'fs5',
       'lf17' : 'fs6',
       'lf18' : 'fs7',

       # Special locals
       'l31' : 'zero',
       'l18' : 'sp',
       'l17' : 'gp',
       'l16' : 'fp',
       'l15' : 'ra',
       'l14' : 'pv',
       'l13' : 's5',

       'l12' : 's4',
       'l11' : 's3',
       'l10' : 's2',
       'l9' : 's1',
       'l8' : 's0',
       'l7' : 'a0',
       'l6' : 'a1',
       'l5' : 'a2',
       'l4' : 'a3',
       'l3' : 'a4',
       'l2' : 'a5',
       'l1' : 'rv',
       'l0' : 'at',
       }

    reg_aliases = {
       'tlsp' : 'l18',
       'gp' : 'l17',
       'pv' : 'l14',
       'ra' : 'l15',
       'fp' : 'l16',
       'rv' : 'l1',
       'idx_init' : 'l0',
       'basearg' : 'l1',
       'zero' : 'l31',
       'frv' : 'lf3',
       'fzero' : 'lf31',
       'fbasearg' : 'lf1'
       }

    def post_init_regmagic(self, rm):
        # all virtual register names, except SP and GP
        rm.allregs = set(['$l%d' % x for x in xrange(31 - self.iargregs) if x not in (17, 18)] + \
                  ['$lf%d' % x for x in xrange(31 - self.fargregs)] + \
                  ['$d%d' % x for x in xrange(self.iargregs / 2)] + \
                  ['$df%d' % x for x in xrange(self.fargregs / 2)] + \
                  ['$%d' % x for x in xrange(31)])

        ### Register lists for the compile commands ###
        rm.fixed_registers = []
        for (v, l) in [("f",rm._freg_inv), ("",rm._reg_inv)]:
            fixed = set()
            for rl in l:
                for r in rl:
                    if r['cat'] in 'sdg':
                        fixed.add(r['legnr'])
            for r in fixed:
                rm.fixed_registers.append('$%s%d' % (v, r))

        rm.call_saved_registers = []
        for i in xrange(6):
            rm.call_saved_registers.append('$%d' % self.legacy_regs['s%d' % i])
        for i in xrange(8):
            rm.call_saved_registers.append('$f%d' % self.legacy_fregs['fs%d' % i])

        rm.call_arg_registers = []
        for i in xrange(6):
            rm.call_arg_registers.append('$%d' % self.legacy_regs['a%d' % i])
        for i in xrange(6):
            rm.call_arg_registers.append('$f%d' % self.legacy_fregs['fa%d' % i])


regmagic = RegMagic(RegDefs())
__all__ = ['regmagic']

