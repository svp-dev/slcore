slc, a SVP language compiler.
Copyright (C) 2008, 2009, 2010, 2011, 2012   The SL project.

=====================
 Introduction to SLC
=====================

SLC, a generic SVP_ compiler.

.. _SVP: http://www.svp-home.org/

.. contents::

Installation
============

To install slc on your system, type in the classical sequence at the
command prompt::

        ./configure
        make
        make install (as root)

Between ``make`` and ``make install``, you may also want to run::

        make check

This will ensure that the demo programs can be compiled with various
slc targets.

Alternatively, the separate "SL installer" can be used to automate
installation. Check the separate CSA note [sl13] for details.

Requirements
============

The following tools are used by slc:

- GNU bash 3.x (3.2 or later)

- a standard ISO C99 compiler

- Python >=2.6 and <3.x (2.6 or later is required, 3.x not supported yet)

The following tools are also required, but can be installed using the
automated installer in the separate ``deploy`` package:

- GNU m4 1.4.x (1.4.6 or later), with patches to allow resetting the
  line counter and file name for error messages;

- GNU binutils 2.18+ modified with Microthreading extensions, supporting
  ``mtalpha`` and ``mtsparc`` targets;

- GCC 4.5+ modified to target Microthreaded architectures.

For more details about prerequisites, check out the separate CSA note
[sl13] for details.

Using slc
=========

The SL toolchain comes with several demonstration programs. Looking at
them is a good way to see what slc can support. They can be found in
the ``demo`` subdirectory of the separate ``sl-programs`` package.

See also
========

There are other sources of interest in the distribution:

- Headlines about the project can be found in the file ``NEWS``.

- Documentation about the input language SL and the accompanying
  software library can be found in the CSA notes at
  http://notes.svp-home.org/ 

- Additional documentation can be found in the ``doc/`` subdirectory.

Licensing
=========

This software is composed of the following two separate components:

- the SL compiler, able to produce object code from C/SL source
  form. The compiler is entirely contained in the ``tools/``
  subdirectory.  This is licensed under the terms of the GNU Public
  License, version 3.

- the SL library, necessary to produce complete executable programs
  from objects produced by the SL compiler. The library is contained
  in the ``lib/`` subdirectory. It is a composite software composed of:

  - the SVP-specific library code in:

    - ``lib/slsys``
    - ``lib/mgos/gfx``
    - ``lib/mgos/sep``
    
    This is licensed under the terms of the GNU Public License, version 3.

  - the Microgrid-specific library code in:

    - ``lib/libc``
    - ``lib/mipsel-sim``
    - subdirectories of ``lib/mgos`` not listed above.

    This is a composite package containing:
 
    - ``lib/libc/math/mipsel/``, ``lib/libc/math/mtalpha/m/``: a
      precompiled subset of the uClibc math library, compiled with
      ``slc``. The binary form is provided for convenience; it can be
      regenerated using ``slc`` from the uCLibc sources. uClibc is
      itself licensed under the terms of the Lesser GNU Public License
      (LGPL), version 2.1. A copy of uClibc can be obtained at
      http://www.uclibc.org/

      .. note:: To re-generate the math objecs: configure uClibc for
        the appropriate target; then modify ``Rules.mak`` to use ``slc
        -b ...`` as code generator; then run ``make``.

    - ``lib/libc/bsd/libc``: a modified subset of the FreeBSD C
      library. This is licensed under the terms of the BSD license.
      See the headers of each source file for licensing terms.

    - ``lib/libc/bsd/contrib/gdtoa``: a modified subset of the
      ``gdtoa`` software distributed with FreeBSD. This is licensed
      under the terms of a custom copyleft license by Lucent. See the
      headers of each source file for licensing terms.

    - ``lib/mipsel-sim/libgcc.a``: a precompiled version of GCC 4.7's
      embedded ``libgcc.a``, compiled using ``slc`` itself instead of
      ``gcc``. The binary form is provided as convenience; it can be
      regenerated usng ``slc`` from any GCC version. GCC 4.7 is
      licensed under the terms of the GNU Public License, version 3.

      .. note:: To re-generate ``libgcc.a``: configure gcc for target
	``mipsel-linux-gnu``; then run ``make`` a first time; then
	manually edit ``mipsel-linux-gnu/libgcc/Makefile`` in the
	build tree to use ``slc -b mipsel_s`` as code generator; then
	run ``make clean; make`` in ``mipsel-linux-gnu/libgcc``.


    - custom code in the subdirectories not already listed, licensed
      under the terms of the GNU Public License, version 3.

  - The library build tools, composed of the ``configure.ac`` script
    and derived ``configure``, and makefiles (files ``Makefile.in``,
    ``Makefile.am`` and filenames ending with ``.mk``). These are
    licensed under the terms of the GNU Public License, version 3.

References:

- Lesser GNU Public License, version 2.1: http://www.gnu.org/licenses/lgpl-2.1.txt

- GNU Public License, version 3: http://www.gnu.org/licenses/gpl-3.0.txt

.. Local Variables:
.. mode: rst
.. End:
