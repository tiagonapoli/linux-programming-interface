../../mktlpi ./vfork-fork.c

hyperfine --export-markdown bench.md \
    "./a.out fork 5000 1000000" \
    "./a.out fork 5000 2000000" \
    "./a.out fork 5000 4000000" \
    "./a.out vfork 5000 1000000" \
    "./a.out vfork 5000 2000000" \
    "./a.out vfork 5000 4000000"