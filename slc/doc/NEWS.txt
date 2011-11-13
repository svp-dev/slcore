====================================
 Release notes for the SL toolchain
====================================

This document describes major updates to the SL toolchain.

Version 3.7b (Development)
==========================

Changes since 3.6
-----------------

- the utc-ptl library is not bundled any more.

- 4 new run-time environments are defined, based on the utc-ptl and
  hlsim libraries. 

Version 3.6
===========

Changes since 3.5b
------------------

Language and library
````````````````````

- Program command-line arguments passed through ``slr`` are now
  propagated to C's ``main`` 's ``argc`` / ``argv`` pair on all
  targets.

- The Fibre and "slr variables" data input mechanisms now align the
  data on addresses multiple of 64 bytes. This is an attempt at
  aligning the data on cache line boundaries (requested by Bert
  Gijsbers).

- Multiple versions of the TLS heap allocator are generated, with
  different levels of debugging instrumentation and with and without
  garbage collection of remotely deallocated chunks. The
  implementation can be chosen by linking one of the various
  ``mta_tlsmalloc`` objects generated.

- Preliminary support for POSIX file access is implemented on MT
  targets.

- Remove support for so-called "infinite" thread families. Programs
  that need many threads can simple use a large index range or loop on
  family creation.

Microgrid-specific changes
``````````````````````````

- When targeting the Microgrid simulator, the SL compiler expects
  version 3.1 or later of the simulator.

- The Microgrid supports I/O devices. For now only devices connected
  to the first core are recognized. 

- In particular, the Microgrid supports a real time clock, from which
  C's ``time()`` function is implemented. More support for time will be
  added as need arises

- Also as a consequence the graphical framebuffer is supported as an
  I/O device. The ``gfx_`` API from the SL library has been modified to
  use this device when run on the first core, but not from other
  cores. For the time being, accesses to the framebuffer must
  therefore be delegated to the first core.

- The Microgrid simulator version 3.1 uses a different configuration
  format than previous versions. The machine profiles bundled with the
  SL tool chain have been updated; however any custom profile need to
  be updated as well. More details in CSA note [mgsim17].

- The machine profiles for large grids but only one DDR channel have
  been removed because they do not reflect a useful configuration. The
  profiles for large grids and random banked memory have been updated
  to reflect they they are not realistic configurations.

- The non-thread-safe functions ``vfprintf``, ``gdtoa`` and ``dlmalloc``
  have been isolated to execute mutually exclusively on their own
  cores.

- Several code generation optimizations and fixes, on both MT-Alpha
  and MT-SPARC (see the ChangeLog for details).

Version 3.5b (Development)
==========================

Changes since 3.4
-----------------

Microgrid-specific changes
``````````````````````````

- Code generation for MT targets was updated as a result of a major
  redesign of the Microgrid delegation protocol (MGSim version 3.0). In
  the new design, places are configurable and there are no fixed size
  clusters any more. For more details see the CSA note [mgsim11].

- The root SEP code in the SL library was updated to handle the new
  handling of place layouts.

Language and library
````````````````````

- New create specifiers are introduced in the language:

  - ``sl__nowait``: creation fails if a context cannot be allocated
    right away. 

  - ``sl__forcewait``: family creation suspends until a context
    becomes available; no fall-back to run the work sequentially is
    generated.

  - ``sl__forceseq``: family execution is performed sequentially
    within the parent thread's context.

  - ``sl__strategy(NAME)``: family allocation occurs according to the
    specified strategy. The following symbolic names are possible:

    - ``fill``: try to use as many cores in the place as possible.

    - ``exact``: try to use all cores in the place.

    - ``balanced``: try to use the least loaded core.

    - ``single``: use only the first core.

    - ``default``: the default strategy for the target place
      (currently ``fill``).

  For now these are only supported for the Microgrid targets. They are
  further documented in note [mgsim12].

Compiler interface
``````````````````

The following command-line flags are now supported::

   -fno-spr-create-strategies
   -fno-spr-collapse-singletons
   -fasmfix-tls-via-gettid
   -fno-asmopt-swch-before-ll-use

Check the manual page or ``slc --help`` for details.

Version 3.4, March 2011
=======================

Changes since 3.3
-----------------

- A new construct ``sl_spawn`` is introduced in the language, together
  with the accompanying ``sl_spawndecl`` and ``sl_spawnsync``. This
  experimental language feature allows to decouple creation from
  synchronization.

- The SEP interface has been re-designed and simplified. See the CSA 
  note [sl16] for more details.

Version 3.3, March 2011
=======================

Changes since 3.2
-----------------

Language and library
````````````````````

- Several primitives previously defined only as C macros are now also
  implemented as C functions as well (macros are used unless
  undefined). For example ``clock``, ``mtperf_sample``, etc.

- Added support for C's function ``time``.

Change related to the MT targets
````````````````````````````````

- Preliminary support for 32-bit SPARC ISAs has been added. The target
  prefixes are ``mts`` (for uT-LEON3) and ``mtsn`` (for MGSim).

  Known limitations: the standard C library is not supported, nor is
  floating-point nor 64-bit integer types. Some C string handling
  functions might work.

Known limitations
-----------------

- Support for the C99 standard library is incomplete.

- C's ``long double`` type is not supported.

- The C ``tgamma`` function is less accurate than required by the
  specification.

- SVP's *kill* operation is inconsistently supported across targets
  (or not at all). No known programs are affected.

- Due to both software and hardware limitations, Microgrid code is
  much less efficient in several key places than the architecture
  would allow it to be (poor register management due to conservative
  assumptions about unknown use in C function calls). This will be
  investigated and optimized in future versions of the tool chain.

- The SPARC support code is largely untested and *will* break valid
  programs.

Version 3.2, December 2010
==========================

Changes since 3.1
-----------------

Language and library
````````````````````

- The SL language was extended with specifiers for thread function
  definitions, declarations and creates. Notably the specifier
  ``sl__native`` for thread functions indicates that only the "pure"
  code must be generated (without alternatives, for targets that
  support alternative) and the specifier ``sl__exclusive`` for creates
  indicates exclusivity.

- The C keyword ``return`` is allowed to terminate (sequential)
  execution in a thread function body.

- The semantics of the SVP *break* operation have been updated to only
  stop execution of the current thread and let other threads in the
  family run to completion. This is supported for the sequential and
  Microthreading targets.

Changes related to the Microgrid targets
````````````````````````````````````````

- SVP's *break* operation is supported (see above).

- The SEP code is updated to allow sharing of exclusive places between
  requesters.

- The asynchronous monitoring facility from the Microgrid simulator is
  now supported in ``slr``.

- New performance counters were added.

- The Microgrid machine profiles were updated to reflect the recent
  availability of multiple DDR channels in the new COMA memory system.

- The back-end compiler was updated to GCC 4.5.

Bug fixes since 3.1
-------------------

- A bug with thread function pointer declarations was fixed. This
  would otherwise cause compilation issues.

- A large inefficiency would cause exponential space complexity during
  transformation in the depth of C scope nesting. This would cause
  exceedingly large compilation times and/or stack blowouts for large
  programs. This was optimized to a quadratic algorithm in the depth
  of create nesting (i.e. not blocks).

- A bug in the handling of floating-point shared thread parameters for
  MT-Alpha was fixed. This would otherwise cause invalid results or
  errors during execution.

- A bug with assembly fixups on MT-Alpha was fixed. This would
  otherwise cause memory access errors or invalid results during
  execution.

- A bug with thread index declarations on MT-Alpha was fixed. This
  would otherwise cause invalid index values to be used during
  execution.

Known limitations
-----------------

- Support for the C99 standard library is incomplete.

- C's ``long double`` type is not supported.

- The C ``tgamma`` function is less accurate than required by the
  specification.

- SVP's *kill* operation is inconsistently supported across targets
  (or not at all). No known programs are affected.

- Due to both software and hardware limitations, Microgrid code is
  much less efficient in several key places than the architecture
  would allow it to be (poor register management due to conservative
  assumptions about unknown use in C function calls). This will be
  investigated and optimized in future versions of the tool chain.

Version 3.1, June 2010
======================

Changes since 3.0
-----------------

Language and library
````````````````````

- The standard C header names are supported for all targets
  (``stdio.h`` instead of ``cstdio.h``, etc).

Changes related to the Microgrid targets
````````````````````````````````````````

- A new lock-free memory heap allocator using thread-local storage has
  been introduced, and is now the default for all Microgrid targets
  (requires simulator version 2.0.162-r3816 or later).

Known limitations
-----------------

- Support for the C99 standard library is incomplete.

- C's ``long double`` type is not supported.

- The C ``tgamma`` function is less accurate than required by the specification.

- SVP's *break* and *kill* operations are inconsistently supported
  across targets (or not at all), since their definition is in flux at
  this period (early 2010). No known programs are affected.

- Due to both software and hardware limitations, Microgrid code is
  much less efficient in several key places than the architecture
  would allow it to be (poor register management due to conservative
  assumptions about unknown use in C function calls). This will be
  investigated and optimized in future versions of the tool chain.

Version 3.0, May 2010
=====================

.. note:: The SL language has evolved. Check CSA note [sl1] for
   details.

.. note:: A new SVP ISA and ABI have been implemented both in hardware
   (MT-Alpha simulator) and in the compiler. All programs need to be
   re-compiled, re-assembled and re-linked.

Changes since 2.4
-----------------

Language and library
````````````````````

- Programs can define their own ``main`` function with the standard C
  semantics (in which case ``t_main`` loses its special meaning).

- The constructs ``sl_create`` / ``sl_sync`` can be nested.

- Restrictions on the code that can be expressed between ``sl_create``
  and ``sl_sync`` have been removed.

- Pointer variables and typedefs can be declared with ``sl_decl_fptr``
  and ``sl_typedef_fptr``.

- Thread functions can have "attributes" (that must be repeated in
  ``sl_create``). 

- Improved checking of invalid uses of language constructs (better
  diagnostics).

Changes related to the Microgrid target
```````````````````````````````````````

- C function calls are performed within the same thread context as the
  caller. This decreases resource usage.

- A scheme has been implemented to flatten concurrency within the
  current thread context when concurrent resources are exhausted and
  until they become available again.

- Multiple MT targets are now available:

  - ``mta`` (a.k.a ``mtalpha``): hybrid scheme with tolerance to
    resource exhaustion;
 
  - ``mta_n`` ("naked"): pure microthreaded code sensitive to resource
    exhaustion;

  - ``mta_s`` ("sequential"): all concurrency flattened inside a
    single thread (useful for comparisons)

  - ``mta_on`` ("old naked"): previous implementation of the compiler;
    no support for the latest language developments.

Bug fixes since 2.4
-------------------

- The ``tgamma`` was incorrectly implemented. This has been replaced
  by a more correct but numerically inaccurate implementation.

Known limitations
-----------------

- Support for the C99 standard library is incomplete.

- C's ``long double`` type is not supported.

- The C ``tgamma`` function is less accurate than required by the specification.

- SVP's *break* and *kill* operations are inconsistently supported
  across targets (or not at all), since their definition is in flux at
  this period (early 2010). No known programs are affected.

- Due to both software and hardware limitations, Microgrid code is
  much less efficient in several key places than the architecture
  would allow it to be (poor register management due to conservative
  assumptions about unknown use in C function calls). This will be
  investigated and optimized in future versions of the tool chain.

Version 2.4, April 2010
=======================

.. note:: A new SVP ISA and ABI have been implemented both in hardware
   (MT-Alpha simulator) and in the compiler. All programs need to be
   re-compiled, re-assembled and re-linked. The new MT-Alpha compiler
   target name is ``mtalpha`` ("``ppp``" becomes obsolete).

Changes since 2.3
-----------------

Language and library
````````````````````

- The constructs ``sl_create``, ``sl_sync`` and uses of thread
  arguments via ``sl_geta`` and ``sl_seta`` are now allowed in plain C
  functions, not only thread functions defined with ``sl_def``.

- Support for the construct ``sl_funcall`` and accompanying
  ``sl_farg`` has been removed, since plain C function calls are
  supported.

Changes related to the Microgrid target
```````````````````````````````````````

- The Microgrid "operating system" now prints startup messages and
  configuration information automatically on the standard error stream
  at boot time. This display can be disabled by setting the
  undocumented environment variable ``MGSYS_QUIET`` before startup.

- A new COMA memory system was implemented. The previous
  implementation stays available with profile names starting with
  "``zlcoma...``"; the new implementation has profiles names starting
  with "``mlcoma...``". Use ``slr -m list`` for details.

- The option ``-C`` (force COMA) to ``slr`` is removed. Equivalent
  behavior can be obtained by defining custom profiles.

- The ``rmb*`` profiles have been renamed to ``rbm*`` (stands for
  "Random Banked Memory").

- A new ISA was implemented in the MT-Alpha platform, intended to
  simplify a number of hardware mechanisms and provide SVP delegation
  without resource costs in the requesting core. The ISA changes
  required the implementation of a new MT-Alpha code generation
  back-end, much simplified compared to the previous
  implementation. This in turn allowed usage of the ``create``
  construct from plain C function contexts, and enables further
  work on automatic resource deadlock avoidance.
 
- The complete ``cce`` string for the new back-end is
  ``ppp2-mtalpha-sim``. The short alias ``ppp`` (for ``slc -b``, etc)
  is now obsolete, use ``mtalpha`` instead.

Known limitations
-----------------

- Support for the C99 standard library is incomplete.

- C's ``long double`` type is not supported.

- The C ``tgamma`` function may be incorrectly implemented.

- SVP's *break* and *kill* operations are inconsistently supported
  across targets (or not at all), since their definition is in flux at
  this period (early 2010). No known programs are affected.

Version 2.3, January 2010
=========================

New features since 2.2
----------------------

- Additional support for the C99 library has been added (see CSA note
  [sl5]), notably string handling and standard output (incl. ``printf``).

- Using option ``slr -n`` now causes the program's top level thread
  (``t_main``) to be automatically delegated to a SVP place of the
  specified number of cores, instead of the previous behavior of
  ``-n`` which caused the hardware parameters to change.

- A symbol table is provided to the Microgrid simulator to ease
  debugging (requires simulator version 1.0.131-r3441 or later).

Bug fixes since 2.2
-------------------

- A long-standing bug with line numbering has been fixed. This allows
  proper error reporting and debugging information.

- Support GCC 4.4.3 in alternate compiler for MT-Alpha.

- Fix bug with the ``fast_`` allocation macros.

Known limitations
-----------------

- Support for the C99 standard library is incomplete.

- C's ``long double`` type is not supported.

- The C ``tgamma`` function may be incorrectly implemented.

- It is not (yet) possible to create a family of threads from a
  regular C function.

Version 2.2, December 2009
==========================

The SL unit test suite, demo and benchmark programs are now
distributed in separate packages ``sl-tests`` and ``sl-demos``.

New features since 2.1.0a
-------------------------

Language and library
`````````````````````

- The C divide operator is reinstated.

- The C math library is supported (include ``<cmath.h>``).

- New API for pixel-oriented graphics output, declared in
  ``<svp/gfx.h>``.

- New keyword ``sl_anonymous``, which expands to a different C
  identifier each time it appears in source.

- The second argument to ``sl_*arg`` and ``sl_*parm`` (the identifier)
  can be left empty to define an anonymous thread argument/parameter.

- New API for gathering performance metrics (``<svp/perf.h>``)

MT-specific compiler changes
````````````````````````````

- New optimization for MT-Alpha back-end, yielding more efficient core
  pipeline usage in several benchmarks.

- Support GCC 4.4.2 in alternate compiler for MT-Alpha.

Toolchain features
``````````````````

- Microgrid configurations can be specified as "profiles" which
  encapsulate the entire set of architectural settings. Profiles are
  specified with ``slr -m``.

- When running with ``slr -p``, a directory is generated containing
  the files and execution script that are sufficient to reproduce the
  run outside of the SL toolchain. This can serve to reproduce the
  execution or reporting issues.

- New support for actual visual output in MG simulator with ``slr
  -X``.

- The target name ``mtalpha`` is recognized by ``cce`` (and thus ``slc
  -b``) as an alias to ``ppp-mtalpha-sim``.

- From now on, scripts will report meaningful version numbers with
  ``--version``.

- The script version number and a timestamp are reported by ``slt -t``
  and in ``slr`` log files.

Known limitations
-----------------

- C's ``long double`` type is not supported.

- The C ``tgamma`` function may be incorrectly implemented.

- It is not (yet) possible to create a family of threads from a
  regular C function.

Version 2.1.0a, August 2009
===========================

New features since 2.0.3a
-------------------------

Language features
`````````````````

- Now using the C preprocessor before M4: the ``.sl`` and ``.slh``
  extensions disappear in favor of ``.c`` and ``.h``; "``#include``"
  and "``#define``" can now be used freely and both included headers
  and CPP macro definitions can contain SL keywords.

- As a result of using the C preprocessor first, C comments and
  whitespace inside SL macros do not cause strange behavior any more.

- C function definitions and calls are supported by the MT-Alpha
  target. The macro ``sl_funcall`` now simply expands transparently to
  the corresponding C function call.

- New keyword ``sl_end_thread``, as a statement which causes the
  current thread to terminate when it is reached.

Execution and benchmarking
``````````````````````````

- When running with ``slr -t``, detailed information about how the
  program is run is printed on the standard error output. 

- New performance metrics framework in the SL library.

- New data input mechanism, to allow programs to read data formatted
  in Sisal's Fibre I/O format.

- New interface to access the thread local storage on the Microgrid,
  requires simulator revision r2462 or later.

- More benchmarks bundled with toolchain.

Scripts and testing environment
```````````````````````````````

- The ``slr``, ``slt`` and ``timeout`` scripts now react better to
  signals and terminal hangups.

- A new ``make`` target, ``make check-slt`` is provided to run the
  testsuite with ASCII-art output (eye-candy). ``make check`` is
  modified to use the new "parallel tests" feature from Automake 1.11
  which supports concurrency more efficiently than ``slt``.

- A new utility ``slt-many`` is provided to run ``slt`` on a custom
  list of test files / directories provided on the command line and
  provide a test results summary.

Known limitations
-----------------

- C's ``long double`` type is not supported.

- The C ``tgamma`` function may be incorrectly implemented.

- It is not (yet) possible to create a family of threads from a
  regular C function.

Version 2.0.3a, June 2009
=========================

- Development version only.

- Fix code generation bug for MT-Alpha happening when a thread
  function has many local variables.

Known limitations
-----------------

- Comments inside the parameters to ``sl_def`` and other SL macros
  cause undesired effects.

- Only "simple" function return types are supported in function calls
  from thread functions. Also only 6 function arguments are supported,
  of "simple" types as well. See CSA note [sl1] for details.

Version 2.0.2a, June 2009
=========================

New features since 2.0.1a
-------------------------

- New function call syntax from thread functions (``sl_funcall``)
  documented in CSA note [sl1].

- Add memory management services to the library (``malloc``), based on
  Doug Lea's implementation (dlmalloc).

Known issues
------------

- Comments inside the parameters to ``sl_def`` and other SL macros
  cause undesired effects.

- Only "simple" function return types are supported in function calls
  from thread functions. Also only 6 function arguments are supported,
  of "simple" types as well. See CSA note [sl1] for details.

Version 2.0.1a, June 2009
=========================

Changes since 2.0.0a
--------------------

- Change license to GPLv3 since we are reusing code from Autoconf's
  m4sugar.

- Improvements to benchmarking code.

Known issues
------------

- Comments inside the parameters to ``sl_def`` and other SL macros
  cause undesired effects.

Version 2.0.0a, May 2009
========================

Changes since 2.x
-----------------

- Lots of bug fixes; more unit tests. 

- Move language specification to external CSA notes [sl1] and [sl2].

- Support GCC 4.4 in alternate compiler for MT-Alpha.

- Upgrade the muTC-ptl back-end from upstream.

- Rename ``sgr`` to ``slr``.

- New FFT benchmark.

Known issues
------------

- Comments inside the parameters to ``sl_def`` and other SL macros
  cause undesired effects.


Version 2.x, February 2009
==========================

Changes since 1.x
-----------------

- Better documentation, including a formal SL language specification
  derived from C99.

- Alternate SL to MT-Alpha compiler.

- Initial "standard SL library".

- "``utcc``" renamed to "``slc``", to reflect the additional
  generality.

- New program input mechanism via "sgr" helper script.

Known issues
------------

- Comments inside the parameters to ``sl_def`` and other SL macros
  cause undesired effects.

Version 1.x (utcc), December 2008
=================================

- Include a unit test suite and demo applications together with the
  compiler.





.. Local Variables:
.. mode: rst
.. End:
