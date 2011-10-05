#!/usr/bin/perl

use strict;
use warnings;

sub glue {
  my $t  =shift;
  $t =~ s/ //g;
  return $t;
}

while(<>) {
  chomp;
  my @f = split /\t/, $_;
  my $q = shift @f;

  
  for (@f) {
    s/( |:|^)((?:[a-z]\ ){3;20}[a-z])( |\t|$)/$1.glue($2).$3/ge;
  }

  print join("\t", $q, @f)."\n";
}
