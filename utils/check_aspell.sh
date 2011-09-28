#!/bin/bash

# You need aspell and perl module Text::Aspell to run this
# apt-get install libtext-aspell-perl

PWD=$(pwd)
DATA_FILE="../data/preprocessed/simple.learn.oneword.dat"
NOANS_DATA_FILE="../data/preprocessed/simple.data.oneword.dat"
RESULT_FILE="result.dat"

${PWD}/aspell.pl <${NOANS_DATA_FILE} > ${RESULT_FILE}
${PWD}/evaluate.py ${DATA_FILE} ${RESULT_FILE}

rm ${RESULT_FILE}
