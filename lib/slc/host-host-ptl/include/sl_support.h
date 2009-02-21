#ifndef SL_SUPPORT_H
#define SL_SUPPORT_H

#include "ptl_svp.h"

typedef uTC::family sl_family_t;
typedef uTC::place sl_place_t;

#define SVP_LOCAL uTC::PLACE_LOCAL

#define SVP_ENOERR uTC::EXIT_NORMAL
#define SVP_EBROKEN uTC::EXIT_BREAK
#define SVP_EKILLED uTC::EXIT_KILL

#endif
