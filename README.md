# Cellular automaton

## Build

```bash
# make build
make build

# cmake build
cmake -S . -B build
cmake --build build -- -j
```

## Run

```bash
# with make
make run
make run-debug

# with binary
./build/automaton
./build/automaton --help-all

# examples
./build/automaton -l wolfram 1 --code 110 --cell-width 5
./build/automaton -l fall --splices 5
./build/automaton -l life
```

## Debug

GTK's `g_debug()` is used for debugging. To enable debug logs use env variable:

```bash
G_MESSAGES_DEBUG=all ./build/automaton
```
