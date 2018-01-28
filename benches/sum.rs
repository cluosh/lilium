#![feature(test)]
extern crate test;
extern crate lilium;

use test::Bencher;
use lilium::*;

#[bench]
fn sum(b: &mut Bencher) {
    let Module {
        functions: f,
        constants: c,
        entry_point: e,
        code: i
    } = compile(concat!(
        "(def sum (a b)",
        "  (if ",
        "     (> a 0)",
        "     ((+ (sum (- a 1) (+ b 1)) 0))",
        "     ((+ b 1))))",
        "(sum 900 0)"
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
fn sum_tail(b: &mut Bencher) {
    let Module {
        functions: f,
        constants: c,
        entry_point: e,
        code: i
    } = compile(concat!(
        "(def sum (a b)",
        "  (if ",
        "     (> a 0)",
        "     ((sum (- a 1) (+ b 1)))",
        "     ((+ b 1))))",
        "(sum 900 0)"
    ));

    let mut registers: [i64; 1536] = [0; 1536];
    let mut thread = Thread {
        functions: &f,
        constants: &c,
        code: &i,
        registers: &mut registers,
        base: 0
    };

    b.iter(|| { run(&mut thread, e as usize) });
}