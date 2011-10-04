#!/bin/bash

time cat ../data/preprocessed/simple.data.dat | ./best_word_corrector ../data/dict/150k-clear.dat $@  > ../data/preprocessed/simple.ans_04.dat
./evaluate.py ../data/preprocessed/simple.learn.dat ../data/preprocessed/simple.ans_04.dat
#./showbad.py ../data/preprocessed/simple.learn.dat ../data/preprocessed/simple.ans_04.dat

