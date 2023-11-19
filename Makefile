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

run-w: build/automaton
	@build/automaton -l wolfram --code 110 --cell-width 5

run-f: build/automaton
	@build/automaton -l fall --splices 0

run-l: build/automaton
	@build/automaton -l life

run-debug: build/automaton
	@G_MESSAGES_DEBUG=all ./build/automaton

run-help:
	@build/automaton --help-all

clean:
	rm -rf build
