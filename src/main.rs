#[macro_use]
extern crate serde_derive;

extern crate serde;
extern crate serde_json;
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

#[derive(Serialize,Deserialize)]
struct Instruction {
    opcode: u8,
    target: u8,
    src1: u8,
    src2: u8,
}

fn assemble(expression: &ast::Expression,
            base_register: u8,
            instructions: &mut Vec<Instruction>) {
    use ast::Expression::*;

    match *expression {
        Integer(_) => {
            instructions.push(Instruction {
                opcode: 42,
                target: base_register,
                src1: 0,
                src2: 0,
            });
        },
        BinaryOp(_, ref left, ref right) => {
            let reg_left = base_register + 1;
            assemble(&left, reg_left, instructions);
            let reg_right = base_register + 2;
            assemble(&right, reg_right, instructions);
            instructions.push(Instruction {
                opcode: 13,
                target: base_register,
                src1: 0,
                src2: 0
            });
        },
        Function(_, ref operands) => {
            let mut base = base_register;
            for operand in operands {
                base += 1;
                assemble(&operand, base, instructions);
            }
            instructions.push(Instruction {
                opcode: 10,
                target: base_register,
                src1: 0,
                src2: 0
            });
        }

    }
}

fn main() {
    let mut instructions: Vec<Instruction> = Vec::new();
    let expressions = parser::parse_expressions("(name (+ 1 2) (* 3 4))").unwrap();
    for expression in expressions {
        assemble(&expression, 0, &mut instructions);
    }

    println!("{}\n", serde_json::to_string_pretty(&instructions).unwrap());
}
