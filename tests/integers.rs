#[macro_use]
mod common;

extern crate lilium;
use lilium::*;

#[test]
fn const_add() {
    let result = run_program!("(+ 1 (+ 2 123456))", 256);
    assert_eq!(result, 123459);
}

#[test]
fn const_sub() {
    let result = run_program!("(- 0 (- 123456 3))", 256);
    assert_eq!(result, -123453);
}

#[test]
fn const_mul() {
    let result = run_program!("(* 36 (* 36 36))", 256);
    assert_eq!(result, 46656);
}

#[test]
fn const_div() {
    let result = run_program!("(/ (/ 65536 2) 2)", 256);
    assert_eq!(result, 16384);
}

#[test]
fn combined() {
    let result = run_program!("(/ (+ (- 1000002 2) (- (* 500000 2) 0)) 2)", 256);
    assert_eq!(result, 1000000);
}
