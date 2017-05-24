extern crate lalrpop_util;

pub mod parser;
pub mod ast;

#[test]
fn syntax_tests() {
    assert!(parser::parse_expressions("(+ 1 2)").is_ok());
    assert!(parser::parse_expressions("(- 1 2)").is_ok());
    assert!(parser::parse_expressions("(* 1 2)").is_ok());
    assert!(parser::parse_expressions("(/ 1 2)").is_ok());
    assert!(parser::parse_expressions("(name 1 2)").is_ok());
    assert!(parser::parse_expressions("(name (+ 1 2) 2)").is_ok());
    assert!(parser::parse_expressions("(name (+ 1 2) (* 3 4))").is_ok());
}

fn print_operations(expression: ast::Expression, base_register: u8) {
    use ast::Expression::*;
    let mut base = base_register;

    match expression {
        Integer(i) => println!("load {} r{}", i, base_register),
        BinaryOp(name, left, right) => {
            let reg_left = base + 1;
            print_operations(*left, reg_left);
            let reg_right = base + 2;
            print_operations(*right, reg_right);
            println!("{} r{} r{}", name, reg_left, reg_right);
        },
        Function(name, operands) => {
            for operand in operands {
                base += 1;
                print_operations(operand, base);
            }
            println!("call {}", name);
        }
    }
}

fn main() {
    let expressions = parser::parse_expressions("(name (+ 1 2) (* 3 4))").unwrap();
    for expression in expressions {
        print_operations(expression, 0);
    }
}
