nobase_dist_pkgdata_DATA += \
	include/slc.m4 \
	include/slc_m4sugar.m4 \
	include/slc_m4sversion.m4 \
	include/slc_m4sforeach.m4 \
	include/svp/slr.h \
	include/svp/delegate.h \
	include/svp/fibre.h \
	include/svp/compiler.h \
	include/svp/gomt.h \
	include/svp/abort.h \
	include/svp/testoutput.h \
	include/svp/mgsim.h

EXTRA_DIST += \
	include/m4sugar.m4 \
	include/m4sugar_version.m4 \
	include/m4sugar_foreach.m4 \
	include/m4sugar_regexp.patch \
	include/m4sugar_transform.sed

MAINTAINERCLEANFILES += \
	include/slc_m4sugar.m4 \
	include/slc_m4sversion.m4 \
	include/slc_m4sforeach.m4

include/slc_m4sugar.m4: $(srcdir)/include/m4sugar.m4 $(srcdir)/include/m4sugar_transform.sed $(srcdir)/include/m4sugar_regexp.patch
	$(AM_V_at)$(MKDIR_P) include
	$(AM_V_GEN)$(SED) -f $(srcdir)/include/m4sugar_transform.sed $< >$@.tmp && patch $@.tmp $(srcdir)/include/m4sugar_regexp.patch
	$(AM_V_at)mv $@.tmp $@

include/slc_m4sversion.m4: $(srcdir)/include/m4sugar_version.m4 $(srcdir)/include/m4sugar_transform.sed
	$(AM_V_at)$(MKDIR_P) include
	$(AM_V_GEN)$(SED) -f $(srcdir)/include/m4sugar_transform.sed $< >$@.tmp
	$(AM_V_at)mv $@.tmp $@

include/slc_m4sforeach.m4: $(srcdir)/include/m4sugar_foreach.m4 $(srcdir)/include/m4sugar_transform.sed
	$(AM_V_at)$(MKDIR_P) include
	$(AM_V_gen)$(SED) -f $(srcdir)/include/m4sugar_transform.sed $< >$@.tmp
	$(AM_V_at)mv $@.tmp $@


install-data-hook:
	$(MKDIR_P) "$(DESTDIR)$(pkglibdir)/"
	rm -f "$(DESTDIR)$(pkglibdir)/"slc_config*
	$(LN_S) "$(DESTDIR)$(pythondir)/slc_config"* "$(DESTDIR)$(pkglibdir)/"
	rm -f "$(DESTDIR)$(pkglibdir)/slc"
	$(LN_S) "$(DESTDIR)$(pythondir)/slc" "$(DESTDIR)$(pkglibdir)/"

nobase_nodist_python_PYTHON += slc_config.py
DISTCLEANFILES += slc_config.pyc
nobase_python_PYTHON += \
	slc/__init__.py \
	slc/msg.py \
	slc/loc.py \
	slc/ast.py \
	slc/lex.py \
	slc/visitors.py \
	slc/mt/__init__.py \
	slc/mt/common/__init__.py \
	slc/mt/common/regmagic.py \
	slc/mt/common/compile.py \
	slc/mt/common/gentfun.py \
	slc/mt/common/strategy.py \
	slc/mt/common/asmproc/__init__.py \
	slc/mt/common/asmproc/input.py \
	slc/mt/common/asmproc/output.py \
	slc/mt/common/asmproc/enhance.py \
	slc/mt/common/asmproc/filter.py \
	slc/mt/common/asmproc/fixup.py \
	slc/mt/common/asmproc/canon.py \
	slc/mt/common/asmproc/renameregs.py \
	slc/mt/common/asmproc/regextract.py \
	slc/mt/common/asmproc/markused.py \
	slc/mt/common/asmproc/labels.py \
	slc/mt/common/asmproc/swch.py \
	slc/mt/common/asmproc/end.py \
	slc/mt/common/asmproc/remdefaults.py \
	slc/mt/common/asmproc/compress.py \
	slc/mt/common/asmproc/stageopts.py \
	slc/mt/mtalpha/__init__.py \
	slc/mt/mtalpha/regdefs.py \
	slc/mt/mtalpha/compile.py \
	slc/mt/mtalpha/gentfun.py \
	slc/mt/mtalpha/gencreate.py \
	slc/mt/mtalpha/genssync.py \
	slc/mt/mtalpha/asmproc/__init__.py \
	slc/mt/mtalpha/asmproc/base.py \
	slc/mt/mtalpha/asmproc/common.py \
	slc/mt/mtalpha/asmproc/opt.py \
	slc/mt/mtsparc/__init__.py \
	slc/mt/mtsparc/regdefs.py \
	slc/mt/mtsparc/compile.py \
	slc/mt/mtsparc/gentfun.py \
	slc/mt/mtsparc/gencreate.py \
	slc/mt/mtsparc/asmproc/__init__.py \
	slc/mt/mtsparc/asmproc/base.py \
	slc/mt/mtsparc/asmproc/common.py \
	slc/mt/mtsparc/asmproc/opt.py \
	slc/mt/mtsparc/asmproc/delay.py \
	slc/mt/mtsparc/asmproc/decode.py \
	slc/mt/mtsparc/asmproc/opc/mtsparc.py \
	slc/mt/mtsparc/asmproc/opc/__init__.py \
	slc/mt/mipsel/__init__.py \
	slc/mt/mipsel/regdefs.py \
	slc/mt/mipsel/asmproc/__init__.py \
	slc/mt/mipsel/asmproc/base.py \
	slc/mt/mipsel/asmproc/common.py \
	slc/mt/mipsel/asmproc/opt.py \
	slc/ptl/__init__.py \
	slc/ptl/genptlcreate.py \
	slc/ptl/gentfun.py \
	slc/am/__init__.py \
	slc/am/visitors.py \
	slc/lower/__init__.py \
	slc/lower/seq.py \
	slc/lower/split.py \
	slc/lower/lowercreate.py \
	slc/lower/lowgeta.py \
	slc/lower/flavorseta.py \
	slc/lower/lowccast.py \
	slc/lower/lowclabels.py \
	slc/lower/lowcvars.py \
	slc/lower/remflavors.py \
	slc/lower/rename.py \
	slc/lower/spawndecl.py \
	slc/lower/lowerspawnsync.py \
	slc/input/__init__.py \
	slc/input/read.py \
	slc/input/parse.py \
	slc/input/cm4.py \
	slc/input/check.py \
	slc/front/__init__.py \
	slc/front/main.py \
	slc/front/chains.py \
	slc/front/dump.py \
	slc/front/opts.py

nobase_dist_pkgdata_DATA += \
	t-spr/slimpl.m4 \
	t-spr/include/sl_support.h \
	t-sppalpha/slimpl.m4 \
	t-sppalpha/include/sl_support.h \
	t-utc/slimpl.m4 \
	t-utc0/slimpl.m4 \
	t-utc0/include/sl_support.h \
	t-seqc/slimpl.m4 \
	t-seqc/include/sl_support.h

nobase_dist_pkgdata_DATA += \
        mt-profiles/arggen \
        mt-profiles/coma128 \
        mt-profiles/default \
        mt-profiles/lzcoma128_2chan \
        mt-profiles/lzcoma256_4chan \
	mt-profiles/mlcoma128_2chan \
	mt-profiles/mlcoma256_4chan \
        mt-profiles/rbm1 \
        mt-profiles/rbm16 \
        mt-profiles/rbm128 \
        mt-profiles/rbm256 

nobase_dist_pkgdata_DATA += \
	arggen.c

EXTRA_DIST += \
	slc/mt/mtsparc/asmproc/opc/formats.db \
	slc/mt/mtsparc/asmproc/opc/decode.py

