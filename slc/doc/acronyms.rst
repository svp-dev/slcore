====================================
  SLC-related commands and acronyms
====================================

cce
===

Acronym for: Configuration Chain Expander

Role: Manage toolchains for slc.

Rationale: the SLC toolchain is composed of many tools each with
specific options. Because specifying all tools and flags may be
error-prone the cce utility is provided and expands configuration
aliases to complete configuration information.

slc
===

Acronym for: SVP Language Compiler

Role: invoke the compiler toolchain.

scu
===

Acronym for: SLC Configuration Unknown

Role: serve as a placeholder for SVP languages that do not have a
compilation chain yet.

slr
===

Acronym: SL Runner

Role: execute a SVP binary in its intended run environment.

slt
===

Acronym for: SL Tester

Role: test that a SL program compiles and runs on multiple SVP
implementations.

sag
===

Acronym for: SLR Arguments Generator

Role: emit assembly source code suitable to build a data file during
program startup by ``slr``.

spp
===

Acronym for: SLC's Program Processor

Role: generate source code during expansion of SL macros and transform
the generated assembly.

cm4
===

Role: wrap around M4, protect C comments and string literals from M4
macro expansions, and trim whitespace between identifiers and opening
parentheses to make them visible as macro calls to M4.



