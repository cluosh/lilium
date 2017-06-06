extern crate serde;
extern crate serde_json;
extern crate bincode;
extern crate lalrpop_util;

pub mod parser;
pub mod ast;

use serde::ser::{Serialize, Serializer, SerializeStruct};

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

enum Instruction {
    Op1 { opcode: u8, target: u8, operand: u16 },
    Op2 { opcode: u8, target: u8, left: u8, right: u8 },
}

impl Serialize for Instruction {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
        where S: Serializer
    {
        match *self {
            Instruction::Op1 { ref opcode, ref target, ref operand } => {
                let mut state = serializer.serialize_struct("Instruction", 3)?;
                state.serialize_field("opcode", opcode)?;
                state.serialize_field("target", target)?;
                state.serialize_field("operand", operand)?;
                state.end()
            },
            Instruction::Op2 { ref opcode, ref target, ref left, ref right } => {
                let mut state = serializer.serialize_struct("Instruction", 4)?;
                state.serialize_field("opcode", opcode)?;
                state.serialize_field("target", target)?;
                state.serialize_field("left", left)?;
                state.serialize_field("right", right)?;
                state.end()
            },
        }
    }
}

fn assemble(expression: &ast::Expression,
            base_register: u8,
            instructions: &mut Vec<Instruction>) {
    use ast::Expression::*;
    use Instruction::*;

    match *expression {
        Integer(_) => {
            instructions.push(Op1 {
                opcode: 42,
                target: base_register,
                operand: 1,
            });
        },
        BinaryOp(_, ref left, ref right) => {
            let reg_left = base_register + 1;
            assemble(&left, reg_left, instructions);
            let reg_right = base_register + 2;
            assemble(&right, reg_right, instructions);
            instructions.push(Op2 {
                opcode: 13,
                target: base_register,
                left: 2,
                right: 3,
            });
        },
        Function(_, ref operands) => {
            let mut base = base_register;
            for operand in operands {
                base += 1;
                assemble(&operand, base, instructions);
            }
            instructions.push(Op1 {
                opcode: 10,
                target: base_register,
                operand: 4,
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

    println!("{}", serde_json::to_string_pretty(&instructions).unwrap());
    let encoded = bincode::serialize(&instructions, bincode::Infinite).unwrap();
    for byte in encoded {
        print!("{0:02x}", byte);
    }
    println!("");
}
