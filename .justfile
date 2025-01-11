
[private]
default:
        @just --list

[private]
list:
        @just default

build:
        make

run: build
        ./main

clean:
        make clean

fresh:
        make clean || true
        make
        ./main
