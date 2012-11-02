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

.PHONY: lib/.build_done
lib/.build_done: lib/.configure_done
	$(AM_V_GEN)cd lib && $(SLC_VARS) $(MAKE) all

noinst_DATA += lib/.build_done

install-data-local: lib/.build_done
	cd lib && $(SLC_VARS) $(MAKE) install prefix="$(prefix)" DESTDIR="$(DESTDIR)"

uninstall-local: lib/.build_done
	cd lib && $(SLC_VARS) $(MAKE) uninstall prefix="$(prefix)" DESTDIR="$(DESTDIR)"

clean-local:
	if test -r lib/Makefile; then cd lib && make clean; fi
	rm -f lib/.build_done

distclean-local:
	rm -f lib/.configure_done
