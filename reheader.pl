#! /usr/bin/env perl
# $Id$
#
## reheader.pl ensures that all C/C++ header files are properly
## "protected" with #ifndef..#endif, using a CPP variable that should
## be unique across the whole project. For this purpose it uses
## the filename uppercased and prefixes it with "UTC".
## Then the copyright and licence text is replaced with the
## official text.

use warnings;
use strict;

my $boundupper = "#ifndef %CPPNAME%
# define %CPPNAME%";
my $boundlower = "#endif // ! %CPPNAME%";

my $m4_boundupper = "sl_begin_header([[%CPPNAME%]])m4_dnl -*- m4 -*-";
my $m4_boundlower = "sl_end_header([[%CPPNAME%]])";

my $cxx_hint =
"//                                                             -*- C++ -*-
";



my $header =
"//
// %BASENAME%: this file is part of the SL toolchain.
//
// Copyright (C) %COPYRIGHT% The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//
// \$Id\$
//

";


sub rehead ($)
{
  my $fname = $_[0];

  # The short name, without the path from builddir to srcdir.
  $_ = $fname;
  s,^.*include/,,g; s,^.*impl/(.*),$1,g;
  my $shortname = $_;
  s,.*/,,g;
  my $basename = $_;

  $_ = $shortname;
  s,^[./]*,,g;
  s,^,SLC_,g;
  s,[^a-zA-Z],_,g;
  y/[a-z]./[A-Z]_/;
  my $cppname = $_;

  $_ = $boundupper;
  s,%BASENAME%,$basename,g;
  s,%CPPNAME%,$cppname,g;
  my $xboundupper = $_;

  $_ = $boundlower;
  s,%BASENAME%,$basename,g;
  s,%CPPNAME%,$cppname,g;
  my $xboundlower = $_;

  $_ = $m4_boundupper;
  s,%BASENAME%,$basename,g;
  s,%CPPNAME%,$cppname,g;
  my $m4_xboundupper = $_;

  $_ = $m4_boundlower;
  s,%BASENAME%,$basename,g;
  s,%CPPNAME%,$cppname,g;
  my $m4_xboundlower = $_;

  $_ = $header;
  s,%BASENAME%,$basename,g;
  s,%CPPNAME%,$cppname,g;
  my $xheader = $_;

  if ($fname =~ /\.(cc|hh|hxx|h)$/)
    { $xheader = $cxx_hint . $xheader; }

  # print STDERR "Processing $fname...\n";

  open(FILE, "<", $fname) or die "cannot open $fname: $!\n";

  my $year = `date +%Y`;
  $year =~ s,\n,,g;
  my $default_copyright = 1;
  my $copyright = $year;

  my $content;
  while (<FILE>)
    {
      $content .= $_;
      if (/Copyright \(C\) (.*) The SL project\./)
	{
	  $copyright = $1;
	  $copyright =~ s/,([^ ])/, $1/g;
	  if ($default_copyright)
	    {
	      $default_copyright = 0;
	    }
	  else
	    {
	      warn "$fname: Multiple copyright lines found\n";
	    }
	  # print STDERR "Copyright is $copyright\n";
	}
      elsif (/\@file\s+([\w.\/]+)/)
	{
	  warn "$fname: incorrect \@file\n"
	    unless $1 eq $basename or $1 eq $shortname;
	}
    }
  close FILE;

  $xheader =~ s,%COPYRIGHT%,$copyright,g;

  # Prepare the new contents.
  $_ = $content;

  # Kill trailing spaces.
  s/[ \t]+$//mg;

  s,^((/\*.*?\*/)|(//[^\n]*\n)|[ \t\n])*,,sg;
  s,([ \t\n])*$,\n,sg;

  # Adjust cpp guards.
  if ($fname =~ /\.(cc|hh|hxx|h|c|sl)$/) {
    if (/^\# *ifndef[^\n]*\n[\n\t ]*\# *define[^\n]*\n/s)
      {
	s,^\# *ifndef[^\n]*\n[\n\t ]*\# *define[^\n]*\n,$xboundupper\n,sg;
      }
    
    if (/\# *endif[^\n]*\n[\n\t ]*$/s)
      {
	s,\# *endif[^\n]*\n[\n\t ]*$,$xboundlower\n,sg;
      }
    
    s/^/$xheader/sg;
    }
  if ($fname =~ /\.slh$/) {
    if (/^ *sl_begin_header/s) {
      s,^ *sl_begin_header[^\n]*\n((/\*.*?\*/)|(//[^\n]*\n)|[ \t\n])*,$m4_xboundupper\n$xheader,sg;
      }
    if (/ *sl_end_header[^\n]*\n[\n\t ]*$/s) {
      s, *sl_end_header[^\n]*\n[\n\t ]*$,$m4_xboundlower\n,sg;
      }
    }

  # Make sure we have a unique ending eol.
  s/\n+\z/\n/;

  $content = $_;

  my @mv = ("mv", "-f", "$fname", "$fname.bak");
  system(@mv) == 0 or die "system @mv failed: $?\n";

  open(FILE, ">", $fname) or die "cannot open $fname for writing: $!\n";
  print FILE $content;
  my @svn = ("svn", "propset", "svn:keywords", "Id", "$fname");
  system(@svn);
}

foreach my $file (@ARGV)
{
  rehead ($file);
}

exit 0;

### Setup "GNU" style for perl-mode and cperl-mode.
## Local Variables:
## perl-indent-level: 2
## perl-continued-statement-offset: 2
## perl-continued-brace-offset: 0
## perl-brace-offset: 0
## perl-brace-imaginary-offset: 0
## perl-label-offset: -2
## cperl-indent-level: 2
## cperl-brace-offset: 0
## cperl-continued-brace-offset: 0
## cperl-label-offset: -2
## cperl-extra-newline-before-brace: t
## cperl-merge-trailing-else: nil
## cperl-continued-statement-offset: 2
## End:
