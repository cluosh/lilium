#[macro_use]
mod common;

extern crate lilium;
use lilium::*;

#[test]
fn const_and() {
    let result = run_program!("(& 1 0)", 256);
    assert_eq!(result, 0);
}

#[test]
fn const_or() {
    let result = run_program!("(| 1 0)", 256);
    assert_eq!(result, 1);
}

#[test]
fn conditional() {
    let result = run_program!(concat!(
        "(def fun (a b)",
        "  (if ",
        "     (> a 0)",
        "     ((fun (- a 1) (+ b 1)))",
        "     ((+ b 1))))",
        "(fun 20 2)"
    ), 6144);
    assert_eq!(result, 23);
}
