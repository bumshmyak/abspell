#!/bin/bash

# You need aspell to run this

DATA_FILE="../data/preprocessed/simple.learn.oneword.300.dat"
NOANS_DATA_FILE="../data/preprocessed/simple.data.oneword.300.dat"
RESULT_FILE="result.dat"
TMP_FILE="tmp"

cat $NOANS_DATA_FILE | while read line ; do
    echo $line > $TMP_FILE
    echo 111 | aspell --ignore-case -l en -d en --sug-mode=bad-spellers check $TMP_FILE
    correction=`cat tmp | tr '[A-Z]' '[a-z]' | sed "s/'//"` 
    echo -e $line"\t1.0:"$correction>> $RESULT_FILE
done

./evaluate.py $DATA_FILE $RESULT_FILE

rm $TMP_FILE
rm $TMP_FILE.bak
rm $RESULT_FILE