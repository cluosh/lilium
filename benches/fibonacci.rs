#![feature(test)]
extern crate test;
extern crate lilium;

use test::Bencher;
use lilium::*;

#[bench]
fn fibonacci(b: &mut Bencher) {
    let Module {
        functions: f,
        constants: c,
        entry_point: e,
        code: i
    } = compile(concat!(
    "(def fib (a b c)",
    "  (if",
    "  (> c 1)",
    "  ((+ (fib b (+ a b) (- c 1)) 0))",
    "  (b)))",
    "(fib 0 1 900)"
    ));

    let mut registers = vec![0; 256 * 902];
    let mut thread = Thread {
        functions: &f,
        constants: &c,
        code: &i,
        registers: &mut registers,
        base: 0
    };

    b.iter(|| { run(&mut thread, e as usize) });
}

#[bench]
fn fibonacci_tail(b: &mut Bencher) {
    let Module {
        functions: f,
        constants: c,
        entry_point: e,
        code: i
    } = compile(concat!(
    "(def fib (a b c)",
    "  (if",
    "  (> c 1)",
    "  ((fib b (+ a b) (- c 1)))",
    "  (b)))",
    "(fib 0 1 900)"
    ));

    let mut registers: [i64; 25600] = [0; 25600];
    let mut thread = Thread {
        functions: &f,
        constants: &c,
        code: &i,
        registers: &mut registers,
        base: 0
    };

    b.iter(|| { run(&mut thread, e as usize) });
}

#[bench]
fn fibonacci_big(b: &mut Bencher) {
    let Module {
        functions: f,
        constants: c,
        entry_point: e,
        code: i
    } = compile(concat!(
    "(def fib (a b c)",
    "  (if",
    "  (> c 1)",
    "  ((fib b (+ a b) (- c 1)))",
    "  (b)))",
    "(fib 0 1 1000000)"
    ));

    let mut registers: [i64; 1024] = [0; 1024];
    let mut thread = Thread {
        functions: &f,
        constants: &c,
        code: &i,
        registers: &mut registers,
        base: 0
    };

    b.iter(|| { run(&mut thread, e as usize) });
}