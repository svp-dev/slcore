slc, a SVP language compiler.
Copyright (C) 2008, 2009 the CSA group, The SL project

=====================
 Introduction to SLC
=====================

:Date: 2009-03-01

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

Requirements
============

The following tools are used by slc:

- GNU bash 3.x (3.2 or later)

- GNU m4 1.4.x (1.4.6 or later)

- a standard ISO C99 compiler

- a standard ISO C++ compiler

- the POSIX thread library

- µTC MT-Alpha core compiler (v4.1.x) with mtalpha binutils

- GCC 4.3.x (or later) configured as cross-compiler for
  alpha-gnu-linux with alpha binutils

Using slc
=========

slc comes with several demo programs. Looking at them is a good way to
see what slc can support. They can be found in the ``demos``
subdirectory.


.. Local Variables:
.. mode: rst
.. End:
