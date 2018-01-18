#[macro_use]
mod common;

extern crate lilium;
use lilium::*;

#[test]
#[should_panic(expected = "stackoverflow")]
fn call_notail() {
    run_program!(concat!(
        "(def sum (a)",
        "  (if",
        "    (> a 0)",
        "    ((+ 1 (sum (- a 1))))",
        "    ((+ 0 1))))",
        "(sum 10)"
    ), 1536);
}

#[test]
fn call_tail() {
    let result = run_program!(concat!(
        "(def sum (a b)",
        "  (if ",
        "     (> a 0)",
        "     ((sum (- a 1) (+ b 1)))",
        "     ((+ b 1))))",
        "(sum 200 0)"
    ), 1536);
    assert_eq!(result, 201);
}