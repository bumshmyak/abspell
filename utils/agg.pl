#!/usr/bin/perl

use warnings;
use strict;

my @last = ('',2000,0,0,0);
while(<>) {
  chomp;
  my @fields = split /\t/;
  if ($last[0] eq $fields[0]) {
    for my $i (2..$#fields) {
      $last[$i] += $fields[$i];
    }
    $last[1] = $fields[1];
  } else {
    if ($last[0] ne '') {
      print join("\t", @last), "\n";
    }
    @last = @fields;
  }
}
if ($last[0] ne '') {
  print join("\t", @last);
}

