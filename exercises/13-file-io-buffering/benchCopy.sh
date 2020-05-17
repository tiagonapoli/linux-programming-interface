#!/bin/bash

rm -rf tmp

PREFIX=/mnt/mytmpfs

mkdir -p $PREFIX/tmp/tmp1
mkdir -p $PREFIX/tmp/tmp2

file="$PREFIX/tmp/tmp1/f"
dd if=/dev/urandom of=$file bs=1024 count=4096
buf="1 8 64 512 1024 2048 4096 8192 16384 32768"
tests=""
for i in $buf; do
    ../../mktlpi -DBUF_SIZE=$i copy.c -o $PREFIX/tmp/a$i.out 
    tests="$tests '$PREFIX/tmp/a$i.out $file $PREFIX/tmp/tmp2/x'"
done

echo $tests
bash -c "hyperfine --prepare 'rm -rf $PREFIX/tmp/tmp2; mkdir $PREFIX/tmp/tmp2' --export-markdown test.md $tests"