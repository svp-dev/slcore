slc, a SVP language compiler.
Copyright (C) 2008, 2009 the CSA group, The SL project.

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

See also
========

There are other sources of interest in the distribution:

- Headlines about the project can be found in the file ``NEWS`` at the
  root of the source tree.

- Documentation about the input language SL and the accompanying
  software library can be found in CSA notes. 

- Additional documentation can be found in the ``doc/`` subdirectory.



.. Local Variables:
.. mode: rst
.. End:
