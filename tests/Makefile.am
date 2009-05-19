# 
# slc, a SL compiler.
# 
# Copyright (C) 2008,2009 The SL project..
# All rights reserved.
# 

include $(srcdir)/alltests.mk
EXTRA_DIST = $(TESTSUITE) $(XTESTSUITE)

if ENABLE_TESTSUITE
TESTS = $(TESTSUITE)
if ENABLE_XFAIL_TESTS
TESTS += $(XTESTSUITE)
endif
endif

include $(top_srcdir)/tests.mk

