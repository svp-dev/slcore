#! /usr/bin/perl
# template.pl: this file is part of the SL toolchain.
#
# Copyright (C) 2008,2009,2010 The SL project.
# All rights reserved.

### [ Configurable settings ] ###

# Define the maximum number of arguments a named thread can have
my $maxargs = 31;

### [ End of Configurable settings ] ###

# Get script name
my $self = $0;

use strict;

my @tpl = <STDIN>;

# Add a comment and header at the beginning of the file
print "/* This file is generated by $self, please do not modify */\n\n";
print "/***************************************************************************** \n";
print " *                                                                           * \n";
print " * Name:        ptl_create.h                                                 * \n";
print " * Version:     utc-ptl Release 3                                            * \n";
print " * Description: Header file containing create definitions for 0 to $maxargs        * \n";
print " *              arguments.                                                   * \n";
print " *                                                                           * \n";
print " *****************************************************************************/\n";
print <<EOL;
template <typename T>
struct is_shared {
    static const bool value = false;
};

template <typename T>
struct is_shared< shared<T> > {
    static const bool value = true;
};
EOL

# Process the template file for each number of arguments
for (my $i = 0; $i <= $maxargs; $i++)
{
	my @template_def;
	my @template_args;
	my @template_typedef;
	my @template_ref_typedef;
	my @template_invoke;
	my @template_init;
	my @template_thread_init;
	my @template_ctor_ref;
	my @template_ctor;
	my @template_bare;
	my @template_passon;
	my @template_params;
	my @template_block;
	my $template_isshared = "false";
	for (my $j = 1; $j <= $i; $j++)
	{
		push(@template_def,     "typename TT$j");
		push(@template_def,     "typename T$j");
		push(@template_args,    "T$j m_arg$j");
		push(@template_params,  "TT$j m_arg$j");
		push(@template_typedef,     "TT$j arg$j");
		push(@template_ref_typedef, "TT$j& arg$j");
		push(@template_invoke,  "m_arg$j");
		push(@template_init,    "m_arg$j(arg$j)");
		push(@template_thread_init, "m_arg$j(this, arg$j, get_prev(tci, &Thread::m_arg$j))");
		push(@template_ctor_ref,"T$j& arg$j");
		push(@template_ctor,    "T$j arg$j");
		push(@template_bare,    "TT$j");
		push(@template_bare,    "T$j");
		push(@template_passon,  "arg$j");
		push(@template_block,   "m_arg$j.block()");
		$template_isshared = $template_isshared . " || is_shared<TT$j>::value";
	}
	my @template_bare_nobreak = @template_bare;
	my @template_def_break    = @template_def;
	
	push(@template_def_break,    "typename BreakT");
	push(@template_bare,         "BreakT");
	
	my $template_def          = join(",",@template_def);
	my $template_def_break    = join(",",@template_def_break);
	my $template_params       = join(";", @template_params);
	my $template_args         = join(";", @template_args);
	my $template_typedef      = join(",", @template_typedef);
	my $template_ref_typedef  = join(",", @template_ref_typedef);
 	my $template_invoke       = join(",", @template_invoke);
	my $template_init         = join(",", @template_init);
	my $template_thread_init  = join(",", @template_thread_init);
	my $template_ctor_ref     = join(",", @template_ctor_ref);
	my $template_ctor         = join(",", @template_ctor);
	my $template_bare         = join(",", @template_bare);
	my $template_bare_nobreak = join(",", @template_bare_nobreak);
	my $template_passon       = join(",", @template_passon);
	my $template_block        = join(";", @template_block);
	my $template_ref_invoke   = $template_passon;
	my $template_thread_invoke = $template_invoke;

	if ($template_def)          { $template_def          = "template <$template_def>"; }
	if ($template_def_break)    { $template_def_break    = "template <$template_def_break>"; }
	if ($template_params)       { $template_params       = "$template_params;"; }
	if ($template_args)         { $template_args         = "$template_args;"; }
	if ($template_init)         { $template_init         = ", $template_init"; }
	if ($template_thread_init)  { $template_thread_init  = ", $template_thread_init"; }
	if ($template_ctor_ref)     { $template_ctor_ref     = ", $template_ctor_ref"; }
	if ($template_ctor)         { $template_ctor         = ", $template_ctor"; }
	if ($template_bare)         { $template_bare         = "<$template_bare>"; }
	if ($template_bare_nobreak) { $template_bare_nobreak = "<$template_bare_nobreak>"; }
	if ($template_passon)       { $template_passon       = ", $template_passon"; }
	if ($template_thread_invoke) { $template_thread_invoke = ", $template_thread_invoke"; }
	
	foreach my $line (@tpl)
	{
		my $l = $line;
		$l =~ s/\$template_def_break/$template_def_break/g;
		$l =~ s/\$template_def/$template_def/g;
		$l =~ s/\$template_args/$template_args/g;
		$l =~ s/\$template_params/$template_params/g;
		$l =~ s/\$template_typedef/$template_typedef/g;
		$l =~ s/\$template_ref_typedef/$template_ref_typedef/g;
		$l =~ s/\$template_invoke/$template_invoke/g;
		$l =~ s/\$template_thread_invoke/$template_thread_invoke/g;
		$l =~ s/\$template_ref_invoke/$template_ref_invoke/g;
		$l =~ s/\$template_init/$template_init/g;
		$l =~ s/\$template_thread_init/$template_thread_init/g;
		$l =~ s/\$template_ctor_ref/$template_ctor_ref/g;
		$l =~ s/\$template_ctor/$template_ctor/g;
		$l =~ s/\$template_bare_nobreak/$template_bare_nobreak/g;
		$l =~ s/\$template_bare/$template_bare/g;
		$l =~ s/\$template_passon/$template_passon/g;
		$l =~ s/\$template_block/$template_block/g;
		$l =~ s/\$template_isshared/$template_isshared/g;
		$l =~ s/\$n/$i/g;
		print $l;
	}
}
