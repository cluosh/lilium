#![feature(try_from)]
#![feature(asm)]
#[macro_use]
extern crate serde_derive;
extern crate serde;
extern crate serde_json;
extern crate bincode;
extern crate lalrpop_util;

pub mod parser;
pub mod ast;

use std::convert::TryFrom;

type Opcode = u8;
mod ops {
    use super::Opcode;

    pub const HLT: Opcode = 0;
    pub const LD:  Opcode = 1;
    pub const LDB: Opcode = 2;
    pub const LDR: Opcode = 3;
    pub const ADD: Opcode = 4;
    pub const SUB: Opcode = 5;
    pub const MUL: Opcode = 6;
    pub const DIV: Opcode = 7;
    pub const CAL: Opcode = 8;
    pub const RET: Opcode = 9;
}

type Register = u8;
mod reg {
    use super::Register;

    pub const RET: Register = 0;
    pub const VAL: Register = 1;
}

struct Thread<'a> {
    functions: &'a [u64],
    constants: &'a [i64],
    code: &'a [Instruction],
    registers: &'a mut [i64],
    base: usize
}

#[derive(Serialize)]
struct Instruction {
    opcode: Opcode,
    target: u8,
    left: u8,
    right: u8
}

fn assemble(expression: &ast::Expression,
            base_register: u8,
            constants: &mut Vec<i64>,
            instructions: &mut Vec<Instruction>) {
    use ast::Expression::*;

    match *expression {
        Integer(i) => {
            match i16::try_from(i) {
                Ok(value) => {
                    let left = value as u8;
                    let right = (value >> 8) as u8;
                    instructions.push(Instruction {
                        opcode: ops::LD,
                        target: base_register,
                        left: left,
                        right: right
                    });
                }
                Err(_) => {
                    let len = constants.len();
                    let len = u16::try_from(len)
                        .expect("Reached maximum number of constants.");
                    let left = len as u8;
                    let right = (len >> 8) as u8;
                    constants.push(i);

                    instructions.push(Instruction {
                        opcode: ops::LDB,
                        target: base_register,
                        left: left,
                        right: right
                    });
                }
            }
        },
        BinaryOp(ref op, ref left, ref right) => {
            let reg_left = base_register + 1;
            assemble(&left, reg_left, constants, instructions);
            let reg_right = base_register + 2;
            assemble(&right, reg_right, constants, instructions);

            let mut instruction = Instruction {
                opcode: ops::HLT,
                target: base_register,
                left: base_register + 1,
                right: base_register + 2
            };

            match op.as_ref() {
                "+" => instruction.opcode = ops::ADD,
                "-" => instruction.opcode = ops::SUB,
                "*" => instruction.opcode = ops::MUL,
                "/" => instruction.opcode = ops::DIV,
                _ => panic!("Invalid operation")
            }
            instructions.push(instruction);
        },
        Function(_, ref operands) => {
            let mut base = base_register;
            for operand in operands {
                base += 1;
                assemble(&operand, base, constants, instructions);
            }
        }
    }
}

macro_rules! label {
    ($name:expr) => {
        unsafe {
            asm!(concat!($name, ":")
                 :
                 :
                 :
                 : "volatile");
        }
    }
}

#[cfg(target_arch = "x86_64")]
macro_rules! label_addr {
    ($name:expr) => {
        {
            let addr: usize;
            unsafe {
                asm!(concat!("leaq ", $name, "(%rip), $0")
                     : "=&r"(addr)
                     :
                     :
                     : "volatile");
            }
            addr
        }
    }
}

#[cfg(target_arch = "x86_64")]
macro_rules! dispatch {
    ($vm:expr, $pc:expr, $jumptable:expr) => {
        unsafe {
            let opcode = $vm.code.get_unchecked($pc).opcode as usize;
            let addr = *$jumptable.get_unchecked(opcode);
            asm!("jmpq *$0"
                 :
                 : "r"(addr), "{rdx}"($pc)
                 :
                 : "volatile");
        }
    }
}

#[cfg(target_arch = "x86_64")]
macro_rules! do_and_dispatch {
    ($vm:expr, $jmptable:expr, $name:expr, $pc:expr, $action:expr) => {
        unsafe {
            asm!(concat!($name, ":")
                 : "={rdx}"($pc)
                 :
                 :
                 : "volatile");
        }

        {
            $action
        }

        dispatch!($vm, $pc, $jmptable);
    }
}

#[inline(always)]
fn op_ld(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let number = instruction.left as u16 | (instruction.right as u16) << 8;

        let r = instruction.target as usize + thread.base;
        *registers.get_unchecked_mut(r) = number as i64;
    }
    pc + 1
}

#[inline(always)]
fn op_ldb(thread: &mut Thread, pc: usize) -> usize {
    let constants = &thread.constants;
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let index = instruction.left as u16 | (instruction.right as u16) << 8;
        let c = *constants.get_unchecked(index as usize);

        let r = instruction.target as usize + thread.base;
        *registers.get_unchecked_mut(r) = c;
    }
    pc + 1
}

#[inline(always)]
fn op_ldr(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let r = code.get_unchecked(pc).target as usize + thread.base;
        let rval = *registers.get_unchecked(reg::VAL as usize + thread.base + 256);
        *registers.get_unchecked_mut(r) = rval;
    }
    pc + 1
}

#[inline(always)]
fn op_add(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = left + right;
    }
    pc + 1
}

#[inline(always)]
fn op_sub(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = left - right;
    }
    pc + 1
}

#[inline(always)]
fn op_mul(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = left * right;
    }
    pc + 1
}

#[inline(always)]
fn op_div(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = left / right;
    }
    pc + 1
}

#[inline(always)]
fn op_cal(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let functions = &thread.functions;
    let registers = &mut thread.registers;
    thread.base += 256;
    unsafe {
        *registers.get_unchecked_mut(reg::RET as usize + thread.base) = pc as i64;

        let instruction = code.get_unchecked(pc);
        let b0 = *registers.get_unchecked(instruction.target as usize) as usize;
        let b1 = *registers.get_unchecked(instruction.left as usize) as usize;
        let b2 = *registers.get_unchecked(instruction.right as usize) as usize;
        let function_address = b0 | b1 << 8 | b2 << 16;
        *functions.get_unchecked(function_address) as usize
    }
}

#[inline(always)]
fn op_ret(thread: &mut Thread, pc: usize) -> usize {
    let registers = &mut thread.registers;
    let ret_pc = unsafe {
        *registers.get_unchecked(reg::RET as usize + thread.base)
    };
    thread.base -= 256;
    ret_pc as usize
}

#[inline(never)]
fn run(thread: &mut Thread) {
    let mut ops: [usize; 32] = [label_addr!("op_hlt"); 32];

    ops[ops::HLT as usize] = label_addr!("op_hlt");
    ops[ops::LD  as usize] = label_addr!("op_ld");
    ops[ops::LDB as usize] = label_addr!("op_ldb");
    ops[ops::LDR as usize] = label_addr!("op_ldr");
    ops[ops::ADD as usize] = label_addr!("op_add");
    ops[ops::SUB as usize] = label_addr!("op_sub");
    ops[ops::MUL as usize] = label_addr!("op_mul");
    ops[ops::DIV as usize] = label_addr!("op_div");
    ops[ops::CAL as usize] = label_addr!("op_cal");
    ops[ops::RET as usize] = label_addr!("op_ret");

    let mut pc: usize = 0;

    dispatch!(&thread, pc, ops);

    do_and_dispatch!(&thread, ops, "op_ld", pc, {
        pc = op_ld(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_ldb", pc, {
        pc = op_ldb(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_ldr", pc, {
        pc = op_ldr(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_add", pc, {
        pc = op_add(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_sub", pc, {
        pc = op_sub(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_mul", pc, {
        pc = op_mul(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_div", pc, {
        pc = op_div(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_cal", pc, {
        pc = op_cal(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_ret", pc, {
        pc = op_ret(thread, pc);
    });

    label!("op_hlt");
}

fn compile(program: &str) -> (Vec<u64>,Vec<i64>,Vec<Instruction>) {
    let expressions = parser::parse_expressions(program).unwrap();
    let mut functions: Vec<u64> = Vec::new();
    let mut constants: Vec<i64> = Vec::new();
    let mut instructions: Vec<Instruction> = Vec::new();

    for expression in expressions {
        assemble(&expression, 1, &mut constants, &mut instructions);
    }
    instructions.push(Instruction {
        opcode: ops::HLT,
        target: 0,
        left: 0,
        right: 0
    });

    (functions,constants,instructions)
}

fn main() {
    let (f, c, i) = compile("(/ 65536 2)");
    println!("Instructions:\n{}", serde_json::to_string_pretty(&i).unwrap());
    println!("Constants:\n{}", serde_json::to_string_pretty(&c).unwrap());

    let mut registers: [i64; 65536] = [0; 65536];
    let mut thread = Thread {
        functions: &f,
        constants: &c,
        code: &i,
        registers: &mut registers,
        base: 0
    };
    run(&mut thread);
}

#[cfg(test)]
mod integers {
    use super::*;

    #[test]
    fn const_add() {
        let (f, c, i) = compile("(+ 1 (+ 2 123456))");

        let mut registers: [i64; 256] = [0; 256];
        let mut thread = Thread {
            functions: &f,
            constants: &c,
            code: &i,
            registers: &mut registers,
            base: 0
        };
        run(&mut thread);

        assert!(thread.registers[reg::VAL as usize] == 123459);
    }

    #[test]
    fn const_sub() {
        let (f, c, i) = compile("(- 0 (- 123456 3))");

        let mut registers: [i64; 256] = [0; 256];
        let mut thread = Thread {
            functions: &f,
            constants: &c,
            code: &i,
            registers: &mut registers,
            base: 0
        };
        run(&mut thread);

        assert!(thread.registers[reg::VAL as usize] == -123453);
    }

    #[test]
    fn const_mul() {
        let (f, c, i) = compile("(* 36 (* 36 36))");

        let mut registers: [i64; 256] = [0; 256];
        let mut thread = Thread {
            functions: &f,
            constants: &c,
            code: &i,
            registers: &mut registers,
            base: 0
        };
        run(&mut thread);

        assert!(thread.registers[reg::VAL as usize] == 46656);
    }

    #[test]
    fn const_div() {
        let (f, c, i) = compile("(/ (/ 65536 2) 2)");

        let mut registers: [i64; 256] = [0; 256];
        let mut thread = Thread {
            functions: &f,
            constants: &c,
            code: &i,
            registers: &mut registers,
            base: 0
        };
        run(&mut thread);

        assert!(thread.registers[reg::VAL as usize] == 16384);
    }

    #[test]
    fn combined() {
        let (f, c, i) = compile("(/ (+ (- 1000002 2) (- (* 500000 2) 0)) 2)");

        let mut registers: [i64; 256] = [0; 256];
        let mut thread = Thread {
            functions: &f,
            constants: &c,
            code: &i,
            registers: &mut registers,
            base: 0
        };
        run(&mut thread);

        assert!(thread.registers[reg::VAL as usize] == 1000000);
    }
}
