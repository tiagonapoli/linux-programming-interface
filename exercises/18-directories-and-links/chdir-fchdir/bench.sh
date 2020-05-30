#!/bin/bash 

../../../mktlpi chdir.c -o chdir.out
../../../mktlpi fchdir.c -o fchdir.out

hyperfine --export-markdown bench.md \
    "./chdir.out 1000000" \
    "./fchdir.out 1000000"