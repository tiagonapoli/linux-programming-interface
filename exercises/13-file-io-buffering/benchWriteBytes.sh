#!/bin/bash

rm -rf tmp

PREFIX=/mnt/mytmpfs

mkdir -p $PREFIX/tmp/tmp1
mkdir -p $PREFIX/tmp/tmp2

file="$PREFIX/tmp/tmp1/f"
buf="1 8 64 512 1024 2048 4096 8192 16384 32768"
tests=""
for i in $buf; do
    ../../mktlpi write_bytes.c -o $PREFIX/tmp/a.out 
    sz=$(( 4096 * 1024 ))
    tests="$tests '$PREFIX/tmp/a.out $PREFIX/tmp/tmp2/x $sz $i'"
done

echo $tests
bash -c "hyperfine --prepare 'rm -rf $PREFIX/tmp/tmp2; mkdir $PREFIX/tmp/tmp2' --export-markdown test.md $tests"