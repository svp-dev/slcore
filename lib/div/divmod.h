

extern void __divmodqu(void);
extern void __divmodlu(void);

#ifndef DIRECT_DIV_FUNCTIONS

#define DIVFUNC(Ptr, X, Y)						\
  do {									\
    register typeof(X) __arg0 __asm__(RL0) = (X);			\
    register typeof(Y) __arg1 __asm__(RL1) = (Y);			\
    __asm__ ("allocate " RL2 ", 0, 0, 0, 0\n"					\
	    "\t# SETREGS not necessary when (%0=" RL0") and (%1=" RL1 ")\n" \
	    "\tcrei " RL2 ",0(%4)\n\tswch\n"				\
	    "\tbis " PHYS_ZERO ", " RL2 ", " PHYS_ZERO "\n\tswch"	\
	    : "=r"(__arg0), "=r"(__arg1)				\
	    : "0"(__arg0), "1"(__arg1), "r"(Ptr) : RL2);		\
    (X) = __arg0; (Y) = __arg1;						\
  } while(0)

#else

#define DIVFUNC(Ptr, X, Y)						\
  do {									\
    register typeof(X) __arg0 __asm__(RL0) = (X);			\
    register typeof(Y) __arg1 __asm__(RL1) = (Y);			\
    __asm__ ("allocate " RL2 ", 0, 0, 0, 0\n"					\
	    "\t# SETREGS not necessary when (%0=" RL0") and (%1=" RL1 ")\n" \
	    "\tcred " RL2 "," # Ptr "\n\tswch\n"			\
	    "\tbis " PHYS_ZERO ", " RL2 ", " PHYS_ZERO "\n\tswch"	\
	    : "=r"(__arg0), "=r"(__arg1)				\
	    : "0"(__arg0), "1"(__arg1) : RL2);				\
    (X) = __arg0; (Y) = __arg1;						\
  } while(0)
#endif

#define divmodqu(X, Y) DIVFUNC(__divmodqu, X, Y)
#define divmodlu(X, Y) DIVFUNC(__divmodlu, X, Y)

