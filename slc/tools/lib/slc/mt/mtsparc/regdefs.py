from ..common.regmagic import RegMagic

class RegDefs:

    iargregs = 8
    ilocalregs = 24

    # There is not FP support for now, but we define this to keep the
    # common implementation happy.
    fargregs = 8
    flocalregs = 24

    regprefix = '%'
    regprefix_fmt = '%%'

    legacy_regs = {
        # globals
        'g1' : 1, # always tmp
        'g2' : 2, # app reg
        'g3' : 3, # app reg
        'g4' : 4, # app reg
        'g5' : 5, # OS reg
        'g6' : 6, # OS reg
        'g7' : 7, # OS reg
        # output regs
        'o5' : 13,
        'o4' : 12,
        'o3' : 11,
        'o2' : 10,
        'o1' : 9,
        'o0' : 8,
        # address of CALL (RA?)
        'o7' : 15,
        # locals
        'l0' : 16,
        'l1' : 17,
        'l2' : 18,
        'l3' : 19,
        'l4' : 20,
        'l5' : 21,
        'l6' : 22,
        'l7' : 23,
        # inputs
        'i5' : 29,
        'i4' : 28,
        'i3' : 27,
        'i2' : 26,
        'i1' : 25,
        'i0' : 24,
        # RA - 8
        'i7' : 31,
        # zero
        'g0' : 0,
        # stack pointer
        'sp' : 14,
        # frame pointer
        'fp' : 30
    }

    ######################
    # GCC Allocation order
    ######################
    # /* This is the order in which to allocate registers normally.

    #    We put %f0-%f7 last among the float registers, so as to make it more
    #    likely that a pseudo-register which dies in the float return register
    #    area will get allocated to the float return register, thus saving a move
    #    instruction at the end of the function.

    #    Similarly for integer return value registers.

    #    We know in this case that we will not end up with a leaf function.

    #    The register allocator is given the global and out registers first
    #    because these registers are call clobbered and thus less useful to
    #    global register allocation.

    #    Next we list the local and in registers.  They are not call clobbered
    #    and thus very useful for global register allocation.  We list the input
    #    registers before the locals so that it is more likely the incoming
    #    arguments received in those registers can just stay there and not be
    #    reloaded.  */

    # #define REG_ALLOC_ORDER \
    # { 1, 2, 3, 4, 5, 6, 7,                  /* %g1-%g7 */   \
    #   13, 12, 11, 10, 9, 8,                 /* %o5-%o0 */   \
    #   15,                                   /* %o7 */       \
    #   16, 17, 18, 19, 20, 21, 22, 23,       /* %l0-%l7 */   \
    #   29, 28, 27, 26, 25, 24, 31,           /* %i5-%i0,%i7 */\
    #   40, 41, 42, 43, 44, 45, 46, 47,       /* %f8-%f15 */  \
    #   48, 49, 50, 51, 52, 53, 54, 55,       /* %f16-%f23 */ \
    #   56, 57, 58, 59, 60, 61, 62, 63,       /* %f24-%f31 */ \
    #   64, 65, 66, 67, 68, 69, 70, 71,       /* %f32-%f39 */ \
    #   72, 73, 74, 75, 76, 77, 78, 79,       /* %f40-%f47 */ \
    #   80, 81, 82, 83, 84, 85, 86, 87,       /* %f48-%f55 */ \
    #   88, 89, 90, 91, 92, 93, 94, 95,       /* %f56-%f63 */ \
    #   39, 38, 37, 36, 35, 34, 33, 32,       /* %f7-%f0 */   \
    #   96, 97, 98, 99,                       /* %fcc0-3 */   \
    #   100, 0, 14, 30, 101}                  /* %icc, %g0, %o6, %i6, %sfp */
    #
    #
    # Default reg usage:
    # /* 1 for registers that have pervasive standard uses
    #    and are not available for the register allocator.

    #    On non-v9 systems:
    #    g1 is free to use as temporary.
    #    g2-g4 are reserved for applications.  Gcc normally uses them as
    #    temporaries, but this can be disabled via the -mno-app-regs option.
    #    g5 through g7 are reserved for the operating system.

    #    On v9 systems:
    #    g1,g5 are free to use as temporaries, and are free to use between calls
    #    if the call is to an external function via the PLT.
    #    g4 is free to use as a temporary in the non-embedded case.
    #    g4 is reserved in the embedded case.
    #    g2-g3 are reserved for applications.  Gcc normally uses them as
    #    temporaries, but this can be disabled via the -mno-app-regs option.
    #    g6-g7 are reserved for the operating system (or application in
    #    embedded case).
    #    ??? Register 1 is used as a temporary by the 64 bit sethi pattern, so must
    #    currently be a fixed register until this pattern is rewritten.
    #    Register 1 is also used when restoring call-preserved registers in large
    #    stack frames.
    # */

    # #define FIXED_REGISTERS  \
    #  {1, 0, 2, 2, 2, 2, 1, 1,       \
    #   0, 0, 0, 0, 0, 0, 1, 0,       \
    #   0, 0, 0, 0, 0, 0, 0, 0,       \
    #   0, 0, 0, 0, 0, 0, 1, 1,       \
    #                                 \
    #   0, 0, 0, 0, 0, 0, 0, 0,       \
    #   0, 0, 0, 0, 0, 0, 0, 0,       \
    #   0, 0, 0, 0, 0, 0, 0, 0,       \
    #   0, 0, 0, 0, 0, 0, 0, 0,       \
    #                                 \
    #   0, 0, 0, 0, 0, 0, 0, 0,       \
    #   0, 0, 0, 0, 0, 0, 0, 0,       \
    #   0, 0, 0, 0, 0, 0, 0, 0,       \
    #   0, 0, 0, 0, 0, 0, 0, 0,       \
    #                                 \
    #   0, 0, 0, 0, 0, 1}

    # /* 1 for registers not available across function calls.
    #    These must include the FIXED_REGISTERS and also any
    #    registers that can be used without being saved.
    #    The latter must include the registers where values are returned
    #    and the register where structure-value addresses are passed.
    #    Aside from that, you can include as many other registers as you like.  */

    # #define CALL_USED_REGISTERS  \
    #  {1, 1, 1, 1, 1, 1, 1, 1,       \
    #   1, 1, 1, 1, 1, 1, 1, 1,       \
    #   0, 0, 0, 0, 0, 0, 0, 0,       \
    #   0, 0, 0, 0, 0, 0, 1, 1,       \
    #                                 \
    #   1, 1, 1, 1, 1, 1, 1, 1,       \
    #   1, 1, 1, 1, 1, 1, 1, 1,       \
    #   1, 1, 1, 1, 1, 1, 1, 1,       \
    #   1, 1, 1, 1, 1, 1, 1, 1,       \
    #                                 \
    #   1, 1, 1, 1, 1, 1, 1, 1,       \
    #   1, 1, 1, 1, 1, 1, 1, 1,       \
    #   1, 1, 1, 1, 1, 1, 1, 1,       \
    #   1, 1, 1, 1, 1, 1, 1, 1,       \
    #                                 \
    #   1, 1, 1, 1, 1, 1}

    # REMINDER:

    # SPARC reg sems
    #                  %g0  (r00)       always zero
    #                  %g1  (r01)  [1]  temporary value
    #                  %g2  (r02)  [2]  global 2
    #      global      %g3  (r03)  [2]  global 3
    #                  %g4  (r04)  [2]  global 4
    #                  %g5  (r05)       reserved for SPARC ABI
    #                  %g6  (r06)       reserved for SPARC ABI
    #                  %g7  (r07)       reserved for SPARC ABI

    #                  %o0  (r08)  [3]  outgoing parameter 0 / return value from callee   
    #                  %o1  (r09)  [1]  outgoing parameter 1
    #                  %o2  (r10)  [1]  outgoing parameter 2
    #      out         %o3  (r11)  [1]  outgoing parameter 3
    #                  %o4  (r12)  [1]  outgoing parameter 4
    #                  %o5  (r13)  [1]  outgoing parameter 5
    #             %sp, %o6  (r14)  [1]  stack pointer
    #                  %o7  (r15)  [1]  temporary value / address of CALL instruction

    #                  %l0  (r16)  [3]  local 0
    #                  %l1  (r17)  [3]  local 1
    #                  %l2  (r18)  [3]  local 2
    #      local       %l3  (r19)  [3]  local 3
    #                  %l4  (r20)  [3]  local 4
    #                  %l5  (r21)  [3]  local 5
    #                  %l6  (r22)  [3]  local 6
    #                  %l7  (r23)  [3]  local 7

    #                  %i0  (r24)  [3]  incoming parameter 0 / return value to caller
    #                  %i1  (r25)  [3]  incoming parameter 1
    #                  %i2  (r26)  [3]  incoming parameter 2
    #      in          %i3  (r27)  [3]  incoming parameter 3
    #                  %i4  (r28)  [3]  incoming parameter 4
    #                  %i5  (r29)  [3]  incoming parameter 5
    #             %fp, %i6  (r30)  [3]  frame pointer
    #                  %i7  (r31)  [3]  return address - 8

    # Notes:

    # [1] assumed by caller to be destroyed (volatile) across a procedure call
    # [2] should not be used by SPARC ABI library code
    # [3] assumed by caller to be preserved across a procedure call

    # /* This is the order in which to allocate registers for
    #    leaf functions.  If all registers can fit in the global and
    #    output registers, then we have the possibility of having a leaf
    #    function.

    #    The macro actually mentioned the input registers first,
    #    because they get renumbered into the output registers once
    #    we know really do have a leaf function.

    #    To be more precise, this register allocation order is used
    #    when %o7 is found to not be clobbered right before register
    #    allocation.  Normally, the reason %o7 would be clobbered is
    #    due to a call which could not be transformed into a sibling
    #    call.

    #    As a consequence, it is possible to use the leaf register
    #    allocation order and not end up with a leaf function.  We will
    #    not get suboptimal register allocation in that case because by
    #    definition of being potentially leaf, there were no function
    #    calls.  Therefore, allocation order within the local register
    #    window is not critical like it is when we do have function calls.  */

    # #define REG_LEAF_ALLOC_ORDER \
    # { 1, 2, 3, 4, 5, 6, 7,                  /* %g1-%g7 */   \
    #   29, 28, 27, 26, 25, 24,               /* %i5-%i0 */   \
    #   15,                                   /* %o7 */       \
    #   13, 12, 11, 10, 9, 8,                 /* %o5-%o0 */   \
    #   16, 17, 18, 19, 20, 21, 22, 23,       /* %l0-%l7 */   \
    #   40, 41, 42, 43, 44, 45, 46, 47,       /* %f8-%f15 */  \
    #   48, 49, 50, 51, 52, 53, 54, 55,       /* %f16-%f23 */ \
    #   56, 57, 58, 59, 60, 61, 62, 63,       /* %f24-%f31 */ \
    #   64, 65, 66, 67, 68, 69, 70, 71,       /* %f32-%f39 */ \
    #   72, 73, 74, 75, 76, 77, 78, 79,       /* %f40-%f47 */ \
    #   80, 81, 82, 83, 84, 85, 86, 87,       /* %f48-%f55 */ \
    #   88, 89, 90, 91, 92, 93, 94, 95,       /* %f56-%f63 */ \
    #   39, 38, 37, 36, 35, 34, 33, 32,       /* %f7-%f0 */   \
    #   96, 97, 98, 99,                       /* %fcc0-3 */   \
    #   100, 0, 14, 30, 31, 101}              /* %icc, %g0, %o6, %i6, %i7, %sfp */

    reg_mapping = {
        # MT Globals
        'g0' : 'l7',
        'g1' : 'l6',
        'g2' : 'l5',
        'g3' : 'l4',
        'g4' : 'l3',
        'g5' : 'l2',
        'g6' : 'l1',
        'g7' : 'l0',
        # MT Shareds
        's0' : 'l0',
        'd0' : 'l1',
        's1' : 'l2',
        'd1' : 'l3',
        's2' : 'l4',
        'd2' : 'l5',
        's3' : 'l6',
        'd3' : 'l7',
        # Special locals
        'l0' : 'g0', # zero
        'l1' : 'g1', # temp
        'l2' : 'g2', # app reg
        'l3' : 'g3', # app reg
        'l4' : 'g4', # app reg
        'l5' : 'g5', # OS reg
        'l6' : 'g6', # OS reg
        'l7' : 'g7', # OS reg
        'l8' : 'o5',
        'l9' : 'o4',
        'l10' : 'o3',
        'l11' : 'o2',
        'l12' : 'o1',
        'l13' : 'o0',
        'l14' : 'sp',
        'l15' : 'o7', # HARD-WIRED INTO CALL: l15 must assemble to %r15 = %o7!
        'l16' : 'i5',
        'l17' : 'i4',
        'l18' : 'i3',
        'l19' : 'i2',
        'l20' : 'i1',
        'l21' : 'i0',
        'l22' : 'i7', # RA - 8
        'l23' : 'fp'
    }

    reg_aliases = {
        'tlsp' : 'l14',
        'fp' : 'l23',
        'idx_init' : 'l1',
        'zero' : 'l0',
    }

    def post_init_regmagic(self, rm):
        # all virtual register names
        rm.allregs = set(['$l%d' % x for x in xrange(31 - self.iargregs) if x not in (14,)] + \
                  ['$d%d' % x for x in xrange(self.iargregs / 2)] + \
                  ['$%d' % x for x in xrange(31)])

        ### Register lists for the compile commands ###
        rm.fixed_registers = []
        for (v, l) in [("",rm._reg_inv)]:
            fixed = set()
            for rl in l:
                for r in rl:
                    if r['cat'] in 'sdg':
                        fixed.add(r['legnr'])
            for r in fixed:
                rm.fixed_registers.append('%%%s%d' % (v, r))


regmagic = RegMagic(RegDefs())
__all__ = ['regmagic']

