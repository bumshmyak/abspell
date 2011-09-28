#!/usr/bin/perl

use warnings;
use strict;

use Text::Aspell;
my $speller = Text::Aspell->new;

die unless $speller;

# Set some options
$speller->set_option('lang','en_US');
$speller->set_option('sug-mode','fast');


sub correct_word {
  my $word = shift;

  if ($speller->check($word)) {
    return $word;
  }
  # lookup up words
  my @suggestions = grep {!m/[^a-z]/} $speller->suggest($word);
  push @suggestions, $word;

  return $suggestions[0];
}

sub correct_phrase {
  my $phrase = shift;

  my @result;
  my @words = split /\s+/, $phrase;

  for my $word (@words) {
    if (length($word) < 5 || $word !~ m/^[a-z]+$/) {
      push @result, $word;
    } else {
      push @result, correct_word($word);
    }
  }
  return join " ", @result;
}

while(<>) {
  chomp;
  print $_."\t1.0:".correct_phrase($_), "\n";
}
