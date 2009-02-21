#include "regs2.h"
#include "encapsulate2.h"
#include "create2.h"
#include "divmod.h"

#define KILL(Fid) \
  __asm__ __volatile__ ("kill %0" : : "rI"(Fid));

#define INDEX(Name) \
  register const long long Name = __index

#define BREAK(Val) \
  __asm__ __volatile__ ("break %0" : : "rI"(Val));


#define DECLSHARG(Type, Name, R0, R1) \
  register Type Name ## _in __asm__(R0); \
  register Type Name ## _out __asm__(R1)
#define DECLGLARG(Type, Name, R) \
  register Type const Name __asm__(R)

#define SHARG0(Type, Name) DECLSHARG(Type, Name, RD0, RS0)
#define SHARG1(Type, Name) DECLSHARG(Type, Name, RD1, RS1)
#define SHARG2(Type, Name) DECLSHARG(Type, Name, RD2, RS2)
#define GLARG0(Type, Name) DECLGLARG(Type, Name, RG0)
#define GLARG1(Type, Name) DECLGLARG(Type, Name, RG1)
#define GLARG2(Type, Name) DECLGLARG(Type, Name, RG2)
#define GLARG3(Type, Name) DECLGLARG(Type, Name, RG3)
#define GLARG4(Type, Name) DECLGLARG(Type, Name, RG4)

#define GETSH(Name) \
  ({									\
    __asm__ __volatile__("# MT: read shared " # Name " (%0)" : : "r"(Name ## _in)); \
    Name ## _in; \
  })
#define SETSH(Name, Value) \
  ({									\
    __asm__ __volatile__("# MT: start write shared " # Name " (%1) <- " # Value " (and clobber incoming %0)" \
			 : /* ensure incoming shared is marked clobbered */ \
			   "=r" (Name ## _in)				\
			 : "r"(Name ## _out));				\
    Name ## _out = (Value);						\
    __asm__ __volatile__("# MT: end write shared " # Name " (%0)" : "=r" (Name ## _out) : "0"(Name ## _out)); \
  })
