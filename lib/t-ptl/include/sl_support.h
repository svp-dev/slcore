#ifndef SL_SUPPORT_H
#define SL_SUPPORT_H

#ifdef USE_EXTERNAL_PTL
#include <uTC.h>
#else
#include "ptl_svp.h"
#endif

typedef uTC::family sl_family_t;
typedef uTC::place sl_place_t;

#define PLACE_DEFAULT uTC::PLACE_GROUP
#define PLACE_LOCAL   uTC::PLACE_LOCAL

#define SVP_ENOERR uTC::EXIT_NORMAL
#define SVP_EBROKEN uTC::EXIT_BREAK
#define SVP_EKILLED uTC::EXIT_KILL

#endif
