#! /bin/sh

# Build the code into the tmp directory
gcc main.c ./file_reader/file_reader.c ./hash_map/hash_map.c -o ./tmp/runnable

# Run the program
./tmp/runnable
