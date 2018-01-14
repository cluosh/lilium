use common::*;
use vm::run;
use compiler::compile;

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

#[test]
fn conditional_const() {
    let result = run_program!(concat!(
        "(def asub (a b) (if (> a b) ((- a b)) ((- b a))))",
        "(asub 100 200)"
    ), 1536);
    assert_eq!(result, 100);

    let result = run_program!(concat!(
        "(def asub (a b) (if (> a b) ((- a b)) ((- b a))))",
        "(asub 200 100)"
    ), 1536);
    assert_eq!(result, 100);
}
