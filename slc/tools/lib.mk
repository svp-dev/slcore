include $(top_srcdir)/build-aux/slcvars.mk

.SECONDARY: lib/.configure_done
lib/.configure_done: $(srcdir)/lib/configure
	$(AM_V_at)rm -f "$@"
	$(AM_V_at)$(MKDIR_P) lib
	$(AM_V_GEN)SRC=`test -f 'lib/configure' || echo '$(srcdir)/'`.; SRC=`(cd "$$SRC" && pwd)`; \
	    cd lib && \
	    $(SLC_VARS) "$$SRC"/lib/configure \
	         SLC="$(SLC_LOCAL)" \
	         CCASFLAGS='' CFLAGS='' LDFLAGS='' CPPFLAGS='' \
	         SLRANLIB="$(SLRANLIB)" \
	         SLAR="$(SLAR)" $(RECURSIVE_CONFIGURE_FLAGS)
	$(AM_V_at)touch "$@"

.SECONDARY: lib/.build_done
lib/.build_done: lib/.configure_done bin/cce bin/slc $(srcdir)/lib/configure.ac $(srcdir)/lib/Makefile.am
	$(AM_V_at)rm -f $@
	$(AM_V_GEN)cd lib && $(SLC_VARS) $(MAKE) all
	$(AM_V_at)touch $@

noinst_DATA += lib/.build_done

DIST_SUBDIRS = lib $(SUBDIRS)

install-data-local: lib/.build_done
	cd lib && $(SLC_VARS) $(MAKE) install prefix="$(prefix)" DESTDIR="$(DESTDIR)"

uninstall-local: lib/.build_done
	cd lib && $(SLC_VARS) $(MAKE) uninstall prefix="$(prefix)" DESTDIR="$(DESTDIR)"

clean-local:
	if test -r lib/Makefile; then cd lib && make clean; fi
	rm -f lib/.build_done

distclean-local:
	if test -r lib/Makefile; then cd lib && make distclean; fi
	rm -f lib/.configure_done

maintainerclean-local:
	if test -r lib/Makefile; then cd lib && make maintainerclean; fi

