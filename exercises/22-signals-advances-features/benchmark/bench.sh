../../../mktlpi sigsuspend.c -o sigsuspend.out
../../../mktlpi sigwaitinfo.c -o sigwaitinfo.out

OPERATIONS=100000
hyperfine "./sigsuspend.out $OPERATIONS" "./sigwaitinfo.out $OPERATIONS" --export-markdown report.md.tmp
