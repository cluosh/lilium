#[macro_use]
mod common;

extern crate lilium;
use lilium::*;

#[test]
fn conditional_small_const() {
    let result = run_program!(concat!(
        "(def asub (a b) (if (> a b) ((- a b)) ((- b a))))",
        "(asub 100 200)"
    ), 1536);
    assert_eq!(result, 100);
}

#[test]
fn conditional_big_const() {
    let result = run_program!(concat!(
        "(def asub (a b) (if (> a b) ((- a b)) ((- b a))))",
        "(asub 200 100)"
    ), 1536);
    assert_eq!(result, 100);
}