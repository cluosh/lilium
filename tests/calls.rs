#[macro_use]
mod common;

extern crate lilium;
use lilium::*;

#[test]
fn calls_noargs() {
    let result = run_program!(concat!(
        "(def div () (/ 100 4))",
        "(def mul () (* 2 3))",
        "(def add () (+ 4 5))",
        "(def sub () (- 1000001 1000000))",
        "(+ (div) (+ (mul) (+ (add) (sub))))"
    ), 1536);
    assert_eq!(result, 41);
}

#[test]
fn calls_args() {
    let result = run_program!(concat!(
        "(def div (a b) (/ a b))",
        "(def mul (a b) (* a b))",
        "(def add (a b c) (+ a (+ b c)))",
        "(def neg (a) (- 0 a))",
        "(neg (add 10 20 (div 16 (mul 2 2))))"
    ), 1536);
    assert_eq!(result, -34);
}

#[test]
fn calls_vars() {
    let result = run_program!(concat!(
        "(def fun (a b) (let ((c (* a b)) (d (+ a b))) (+ (- c d) (* d c))))",
        "(def neg (a) (- 0 a))",
        "(neg (fun 10 20))"
    ), 1536);
    assert_eq!(result, -6170);
}