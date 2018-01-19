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
    } = compile("0");

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
fn fibonacci_tail(b: &mut Bencher) {
    let Module {
        functions: f,
        constants: c,
        entry_point: e,
        code: i
    } = compile("0");

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