<img align="left" width="64px" src="presentation/logo.png" />

# Lilium

[![Build Status](https://travis-ci.org/cluosh/lilium.svg?branch=master)](https://travis-ci.org/cluosh/lilium)

Lilium is a simple register machine, designed to execute a language resembling Common Lisp.

## Building

Install a Rust nightly toolchain via [rustup](https://rustup.rs/). It has been tested on the following rustc version:

```terminal
rustc 1.25.0-nightly (7d6e5b9da 2018-01-27)
```

Build the binaries with:

```terminal
cargo +nightly build --release
```

After building, the binaries can be found in the target/release folder.

## Usage

The Lilium environment provides 3 tools:

* `lcc` compiles a Lilium lisp file into bytecode
* `lasm` prints the disassembly of a bytecode file
* `lexec` run a bytecode file on the Lilium VM

Example usage, for compiling a fibonacci example `fibonacci.l`:

```
(def fib (a b c)
  (if
    (> c 1)
    ((fib b (+ a b) (- c 1)))
    (b)))
(write (fib 0 1 50))
```

Compile the file:

```terminal
./lcc fibonacci.l
```

Look at the produced code:

```terminal
./lasm fibonacci.l.bc
```

```
0x00000: mov 5 3
0x00001: ld 6 1
0x00002: gt 4 5 6
0x00003: jtf 4 0x3
0x00004: mov 4 2
0x00005: jmf 0xc
0x00006: mov 5 2
0x00007: mov 7 1
0x00008: mov 8 2
0x00009: add 6 7 8
0x0000a: mov 8 3
0x0000b: ld 9 1
0x0000c: sub 7 8 9
0x0000d: mov 1 5
0x0000e: mov 2 6
0x0000f: mov 3 7
0x00010: jmb 0x10
0x00011: mov 1 4
0x00012: ret
0x00013: ld 3 0
0x00014: ld 4 1
0x00015: ld 5 50
0x00016: mvo 2 3 255
0x00017: mvo 3 4 255
0x00018: mvo 4 5 255
0x00019: call 0x0
0x0001a: ldr 2
0x0001b: write 1 2
0x0001c: hlt
```

Run the bytecode:

```
./lexec fibonacci.l.bc
```

Printing the result `12586269025`.

## Code Structure

The actual VM dispatch code and the code for the operations can be found in [src/vm/dispatch.rs](src/vm/dispatch.rs). The src/compiler directory contains the parser and the code generation, the src/disassembler directory contains the disassembler. Definitions can be found in src/common.
