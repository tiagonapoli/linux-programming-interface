if [ ! -z "$(ls -A ./tmp)" ]; then
    echo "NOT EMPTY"
    exit 1
fi

sync; 
echo 3 | sudo tee /proc/sys/vm/drop_caches;