NPROC=$(shell nproc)

# CC=/usr/bin/gcc-9
# CXX=/usr/bin/g++-9

.PHONY: build run clean

default: build run

build:
	cmake -S . -B build
	cmake --build build -- -j$(NPROC)

run: build/automaton
	@build/automaton

clean:
	rm -rf build
