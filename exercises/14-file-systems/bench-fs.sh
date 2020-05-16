g++ file-io.cpp

mkdir tmp
rm ./tmp/*

deleteOrder='ordered'

hyperfine --prepare 'bash ./prepareTest.sh' --export-markdown bench.md \
    "./a.out -n 400 -c $deleteOrder" \
    "./a.out -n 800 -c $deleteOrder" \
    "./a.out -n 1600 -c $deleteOrder" \
    "./a.out -n 3200 -c $deleteOrder" \
    "./a.out -n 6400 -c $deleteOrder" \
    "./a.out -n 12800 -c $deleteOrder" \
    "./a.out -n 25600 -c $deleteOrder" \
    "./a.out -n 51200 -c $deleteOrder" 


bash ./prepareTest.sh