#!/bin/bash

time cat ../data/original/Spelling.Test.dat | ./best_word_corrector ../data/dict/150k-clear.dat $@  > ../data/res.txt

