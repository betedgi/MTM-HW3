#!/bin/bash

dir_copy_bfr=`ls -la`

make > tempout1

./mtmflix > tempout

diff_str=`diff "tempout" "outFiles/expmake"`

if [[ "$diff_str" == "" ]]; then
	echo "OK"
else
	echo "NOT OK"
fi

make clean > tempout1

rm tempout1
rm tempout

dir_copy_afr=`ls -la`
dir_diff=`diff "$dir_copy_bfr" "$dir_copy_afr"`
if [[ "$dir_diff" == "" ]]; then
	echo "OK"
else
	echo "NOT OK"
fi