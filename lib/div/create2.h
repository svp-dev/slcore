#define CREATEX(FidName, Start, Limit, Step, Block, Func,		\
		TS0, S0, TS1, S1, TS2, S2,				\
		TG0, G0, ge0, TG1, G1, ge1, TG2, G2, ge2,		\
		TG3, G3, ge3, TG4, G4, ge4)				\
  register long long FidName;						\
  __asm__ __volatile__ ("allocate %0, %1, %2, %3, %4\t# CREATE " #FidName : "=r"(FidName) \
			: "I"(RAG0_N), "I"(RAS0_N), "I"(0), "I"(0));	\
  __asm__ ("setstart %0, %2\t# CREATE " #FidName "\n\tswch": "=r"(FidName) : "0"(FidName), "rI"(Start)); \
  __asm__ ("setlimit %0, %2\t# CREATE " #FidName : "=r"(FidName) : "0"(FidName), "rI"(Limit)); \
  __asm__ ("setstep %0, %2\t# CREATE " #FidName : "=r"(FidName) : "0"(FidName), "rI"(Step)); \
  __asm__ ("setblock %0, %2\t# CREATE " #FidName : "=r"(FidName) : "0"(FidName), "rI"(Block)); \
  __sync_var = FidName;							\
  register TS0 S0 __asm__(RAS0);						\
  __asm__ ("setempty %0\t# EMPTY " #S0 "(must be =" RAS0 ")": "=r"(S0) : "0"(S0));		\
  register TS1 S1 __asm__(RAS1);						\
  __asm__ ("setempty %0\t# EMPTY " #S1 "(must be =" RAS1 ")": "=r"(S1) : "0"(S1));		\
  register TS2 S2 __asm__(RAS2);						\
  __asm__ ("setempty %0\t# EMPTY " #S2 "(must be =" RAS2 ")": "=r"(S2) : "0"(S2));		\
  register TG0 const G0 __asm__(RAG0) = (ge0);					\
  register TG1 const G1 __asm__(RAG1) = (ge1);					\
  register TG2 const G2 __asm__(RAG2) = (ge2);					\
  register TG3 const G3 __asm__(RAG3) = (ge3);					\
  register TG4 const G4 __asm__(RAG4) = (ge4);					\
  register void* FidName ## __func __asm__(RAG5) = (Func);		\
  __asm__ __volatile__("crei %0, 0(%6)\t# CREATE " #FidName ", " #Func "\n\tswch"	\
		       : "=r"(__sync_var)				\
		       : "r"(G0), "r"(G1), "r"(G2), "r"(G3), "r"(G4), "r"(FidName ## __func), \
			 "r"(S0), "r"(S1), "r"(S2), "0"(__sync_var)	\
		       );

#define SYNCX(FidName, Ret, S0, S1, S2, G0, G1, G2, G3, G4)		\
  __asm__ __volatile__("bis %4, " PHYS_ZERO ", %0 # SYNC " #FidName "\n\tswch"	\
		       : "=r"(Ret), "=r"(S0), "=r"(S1), "=r"(S2)	\
		       : "r"(__sync_var), "1"(S0), "2"(S1), "3"(S2),	\
			 "r"(G0), "r"(G1), "r"(G2), "r"(G3), "r"(G4), "r"(FidName ## __func)) 
 

