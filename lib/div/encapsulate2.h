#define BEGIN_FUNC__(Name, LN)						\
  void _x_ ## Name (void) {						\
    register long long __sync_var __asm__(RSYNC);			\
    register long long __index __asm__(RL0);				\
    __asm__ ("# MT: index starts in %0 (must be =" RL0 ")" : "=r"(__index)); \
    __asm__ __volatile__(".end _x_" #Name "\n"			\
			 "\n"						\
			 "\t.globl " #Name "\n"				\
			 "\t.ent " #Name "\n"				\
			 #Name ":\n"					\
			 "\t.registers 6 3 19 0 0 31\n"			\
			 "\tldah " RGP ",0(" RG5 ")\t!gpdisp!" # LN "\n" \
			 "\tlda " RGP ",0(" RGP ")\t!gpdisp!" # LN "\n" \
			 "$" # Name "..ng:\n"				\
			 "\tmov " RG5 "," RPV " # PV <- G5 (call convention)\n"	\
			 "\tmov " RL1 "," RSP " # SP <- L1 (thread init)\n" \
			 "\t.prologue 1\n");				\


#define END_FUNC(Name)							\
  __asm__ __volatile__("# MT: end of thread " #Name "()\n"		\
		       "\tnop\n"					\
		       "\tend\n"					\
		       "# RENAME_END " # Name );			\
  } \
  extern void Name(void);


#define BEGIN_FUNC_(Name, LN) BEGIN_FUNC__(Name, LN)
#define BEGIN_FUNC(Name) BEGIN_FUNC_(Name, __LINE__)



