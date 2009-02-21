/////////////////////////////////////////
// Alpha standard registers
// listed in GCC allocation order

// function temporaries
#define PHYS_T0 "$1"
#define PHYS_T1 "$2"
#define PHYS_T2 "$3"
#define PHYS_T3 "$4"
#define PHYS_T4 "$5"
#define PHYS_T5 "$6"
#define PHYS_T6 "$7"
#define PHYS_T7 "$8"
#define PHYS_T8 "$22"
#define PHYS_T9 "$23"
#define PHYS_T10 "$24"
#define PHYS_T11 "$25"

// Assembly scratch register
#define PHYS_AT "$28"

// Return value register
#define PHYS_V0 "$0"

// Incoming function parameters
#define PHYS_A5 "$21"
#define PHYS_A4 "$20"
#define PHYS_A3 "$19"
#define PHYS_A2 "$18"
#define PHYS_A1 "$17"
#define PHYS_A0 "$16"

// Pointer to current function
#define PHYS_PV "$27"
#define PHYS_T12 PHYS_PV

// Callee-save registers
#define PHYS_S0 "$9"
#define PHYS_S1 "$10"
#define PHYS_S2 "$11"
#define PHYS_S3 "$12"
#define PHYS_S4 "$13"
#define PHYS_S5 "$14"

// Return address
#define PHYS_RA "$26"

// Frame-pointer / callee-save
#define PHYS_FP "$15"
#define PHYS_S6 PHYS_FP

// Global pointer
#define PHYS_GP "$29"
// Stack pointer
#define PHYS_SP "$30"
// Zero
#define PHYS_ZERO "$31"


///////////////////////////////////
// Microgrids registers

// Formula
// N = shareds
// M = globals
// #L (locals) = (31 - 2 * N - M)
// #F (fixed locals during create) = 2 + N + M
// #R = #L - #F

// Example:
// N = 3, M = 8 -> #L = 17, #F = 13, #R = 4
// N = 3, M = 7 -> #L = 18, #F = 12, #R = 6
// N = 3, M = 6 -> #L = 19, #F = 11, #R = 8
// we choose the latter


#define RL0_N 0
#define RL1_N 1
#define RL2_N 2
#define RL3_N 3
#define RL4_N 4
#define RL5_N 5
#define RL6_N 6
#define RL7_N 7
#define RL8_N 8
#define RL9_N 9
#define RL10_N 10
#define RL11_N 11
#define RL12_N 12
#define RL13_N 13
#define RL14_N 14
#define RL15_N 15
#define RL16_N 16
#define RL17_N 17
#define RL18_N 18



#ifdef MT_SYMBOLIC
#define RG0 "$g0"
#define RG1 "$g1"
#define RG2 "$g2"
#define RG3 "$g3"
#define RG4 "$g4"
#define RG5 "$g5"

#define RS0 "$s0"
#define RS1 "$s1"
#define RS2 "$s2"

#define RD0 "$d0"
#define RD1 "$d1"
#define RD2 "$d2"

#define RL0 "$l0"
#define RL1 "$l1"
#define RL2 "$l2"
#define RL3 "$l3"
#define RL4 "$l4"
#define RL5 "$l5"
#define RL6 "$l6"
#define RL7 "$l7"
#define RL8 "$l8"
#define RL9 "$l9"
#define RL10 "$l10"
#define RL11 "$l11"
#define RL12 "$l12"
#define RL13 "$l13"
#define RL14 "$l14"
#define RL15 "$l15"
#define RL16 "$l16"
#define RL17 "$l17"
#define RL18 "$l18"

#define RZ "$zero"
#define RGP "$lgp"
#define RSP "$lsp"
#define RRA "$lra"
#define RPV "$lpv"
#define RRV "$lrv"
#define RFP "$lfp"

#define RSYNC "$sync"
#define RBRK "$brk"

#define RAS0 "$as0"
#define RAS1 "$as1"
#define RAS2 "$as2"

#define RAG0 "$ag0"
#define RAG1 "$ag1"
#define RAG2 "$ag2"
#define RAG3 "$ag3"
#define RAG4 "$ag4"
#define RAG5 "$ag5" 

#else // !MT_SYMBOLIC

// Use argument registers for fixeds
#define RG0 PHYS_S5
#define RG1 PHYS_S4
#define RG2 PHYS_S3
#define RG3 PHYS_S2
#define RG4 PHYS_S1
#define RG5 PHYS_S0
#define RS0 PHYS_A0
#define RD0 PHYS_A1
#define RS1 PHYS_A2
#define RD1 PHYS_A3
#define RS2 PHYS_A4
#define RD2 PHYS_A5
// special
#define RZ PHYS_ZERO


// Match special with special
// at the end of the locals
#define RL18 PHYS_SP
#define RL17 PHYS_GP
#define RL16 PHYS_PV
#define RL15 PHYS_RA
#define RL14 PHYS_FP
#define RL13 PHYS_V0
#define RSP RL18
#define RGP RL17
#define RPV RL16
#define RRA RL15
#define RFP RL14
#define RRV RL13

// Allocate remaining locals
#define RL12 PHYS_AT
#define RL11 PHYS_T11
#define RL10 PHYS_T10
#define RL9 PHYS_T9
#define RL8 PHYS_T8
#define RL7 PHYS_T7
#define RL6 PHYS_T6
#define RL5 PHYS_T5
#define RL4 PHYS_T4
#define RL3 PHYS_T3
#define RL2 PHYS_T2
#define RL1 PHYS_T1
#define RL0 PHYS_T0

// Use locals for specials
#define RSYNC RL10
#define RBRK RL11

#define RAS0 RL7
#define RAS1 RL8
#define RAS2 RL9
#define RAG0 RL1
#define RAG1 RL2
#define RAG2 RL3
#define RAG3 RL4
#define RAG4 RL5
#define RAG5 RL6

#endif

#define RAS0_N RL7_N
#define RAS1_N RL8_N
#define RAS2_N RL9_N
#define RAG0_N RL1_N
#define RAG1_N RL2_N
#define RAG2_N RL3_N
#define RAG3_N RL4_N
#define RAG4_N RL5_N
#define RAG5_N RL6_N

// Fix registers; this effectively prevents

