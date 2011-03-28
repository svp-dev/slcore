//
// mtplaces.c: this file is part of the SL toolchain.
//
// Copyright (C) 2011 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include <svp/delegate.h>

#undef get_current_place
sl_place_t get_current_place(void) {
    return __inline_get_current_place();
}

#undef get_core_id
unsigned long get_core_id(void) {
    return __inline_get_core_id();
}

#undef get_local_place
sl_place_t get_local_place(void) {
    return __inline_get_local_place();
}

