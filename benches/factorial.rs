#![feature(test)]
extern crate test;
extern crate lilium;

use test::Bencher;
use lilium::*;

#[bench]
fn factorial(b: &mut Bencher) {
    let Module {
        functions: f,
        constants: c,
        entry_point: e,
        code: i
    } = compile(concat!(
        "(def fac (a b)",
        "  (if ",
        "     (> a 0)",
        "     ((+ (fac (- a 1) (* b a)) 0))",
        "     (b)))",
        "(fac 900 1)"
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
fn factorial_tail(b: &mut Bencher) {
    let Module {
        functions: f,
        constants: c,
        entry_point: e,
        code: i
    } = compile(concat!(
        "(def fac (a b)",
        "  (if ",
        "     (> a 0)",
        "     ((fac (- a 1) (* b a)))",
        "     (b)))",
        "(fac 900 1)"
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