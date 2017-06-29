#![feature(try_from)]
#![feature(asm)]
#[macro_use]
extern crate serde_derive;
extern crate serde;
extern crate bincode;
extern crate lalrpop_util;

pub mod parser;
pub mod ast;

use std::convert::TryFrom;
use std::collections::HashMap;

type Opcode = u8;
mod ops {
    use super::Opcode;

    pub const HLT: Opcode =  0;
    pub const LD:  Opcode =  1;
    pub const LDB: Opcode =  2;
    pub const LDR: Opcode =  3;
    pub const ADD: Opcode =  4;
    pub const SUB: Opcode =  5;
    pub const MUL: Opcode =  6;
    pub const DIV: Opcode =  7;
    pub const CAL: Opcode =  8;
    pub const RET: Opcode =  9;
    pub const MOV: Opcode = 10;
    pub const MVO: Opcode = 11;
}

type Register = u8;
mod reg {
    use super::Register;

    pub const RET: Register = 0;
    pub const VAL: Register = 1;
}

type Type = u8;
mod types {
    use super::Type;

    pub const INT: Type = 0;
    //pub const FLOAT: Type = 0;
    //pub const INTLIST: Type = 0;
    //pub const FLOATLIST: Type = 0;
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
            functions: &mut Vec<u64>,
            instructions: &mut Vec<Instruction>,
            function_mapping: &mut HashMap<String, u32>,
            variables: &HashMap<String, (Type, Register)>) {
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
            assemble(&left,
                     reg_left,
                     constants,
                     functions,
                     instructions,
                     function_mapping,
                     variables);
            let reg_right = base_register + 2;
            assemble(&right,
                     reg_right,
                     constants,
                     functions,
                     instructions,
                     function_mapping,
                     variables);

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
        Function(ref name, ref operands) => {
            let index = {
                match function_mapping.get(name) {
                    Some(index) => *index,
                    _ => panic!("Function {} is not defined", name)
                }
            };

            let base = base_register + 1;
            let mut param_base = reg::VAL;
            for operand in operands {
                assemble(&operand,
                         base,
                         constants,
                         functions,
                         instructions,
                         function_mapping,
                         variables);

                param_base += 1;
                instructions.push(Instruction {
                    opcode: ops::MVO,
                    target: param_base,
                    left: base,
                    right: 0xFF
                });
            }

            instructions.push(Instruction {
                opcode: ops::CAL,
                target: index as u8,
                left: (index >> 8) as u8,
                right: (index >> 16) as u8
            });
            instructions.push(Instruction {
                opcode: ops::LDR,
                target: base_register,
                left: 0,
                right: 0
            });
        },
        FunctionDefinition(ref name, ref param, ref body) => {
            let index = functions.len() as u32;
            let address = instructions.len() as u64;
            function_mapping.insert(name.to_string(), index);
            functions.push(address);

            let mut base = base_register;
            let mut variables = variables.clone();
            for p in param {
                variables.insert(p.to_string(), (types::INT, base));
                base += 1;
            }

            let base = base;
            let variables = &variables;
            for expression in body {
                assemble(&expression,
                         base,
                         constants,
                         functions,
                         instructions,
                         function_mapping,
                         variables);
            }

            instructions.push(Instruction {
                opcode: ops::MOV,
                target: reg::VAL,
                left: base,
                right: 0
            });
            instructions.push(Instruction {
                opcode: ops::RET,
                target: 0,
                left: 0,
                right: 0
            });
        },
        Variable(ref name) => {
            let (_, reg) = match variables.get(name) {
                Some(index) => *index,
                _ => panic!("Variable {} is not defined", name)
            };

            instructions.push(Instruction {
                opcode: ops::MOV,
                target: base_register,
                left: reg,
                right: 0
            });
        }
    }
}

fn disassemble(constants: &[i64],
               functions: &[u64],
               instructions: &[Instruction]) {
    let mut pc: usize = 0;
    for instruction in instructions {
        print!("0x{:05x}: ", pc);
        pc += 1;

        match instruction.opcode {
            ops::HLT => println!("hlt"),
            ops::LD => {
                let rl = instruction.left as u16;
                let rr = instruction.right as u16;
                let val = rl | rr << 8;
                let r = instruction.target;
                println!("ld {} {}", r, val as i16);
            }
            ops::LDB => {
                let rl = instruction.left as u16;
                let rr = instruction.right as u16;
                let val = rl | rr << 8;
                let r = instruction.target;
                println!("ld {} {}", r, constants[val as usize]);
            }
            ops::LDR => {
                let r = instruction.target;
                println!("ldr {}", r);
            }
            ops::ADD => {
                let rl = instruction.left;
                let rr = instruction.right;
                let r = instruction.target;
                println!("add {} {} {}", r, rl, rr);
            }
            ops::SUB => {
                let rl = instruction.left;
                let rr = instruction.right;
                let r = instruction.target;
                println!("sub {} {} {}", r, rl, rr);
            }
            ops::MUL => {
                let rl = instruction.left;
                let rr = instruction.right;
                let r = instruction.target;
                println!("mul {} {} {}", r, rl, rr);
            }
            ops::DIV => {
                let rl = instruction.left;
                let rr = instruction.right;
                let r = instruction.target;
                println!("div {} {} {}", r, rl, rr);
            }
            ops::CAL => {
                let rl = instruction.left as u32;
                let rr = instruction.right as u32;
                let r = instruction.target as u32;
                let addr = functions[(r | rl << 8 | rr << 16) as usize];
                println!("call 0x{:x}", addr);
            }
            ops::RET => println!("ret"),
            ops::MOV => {
                let rl = instruction.left;
                let r = instruction.target;
                println!("mov {} {}", r, rl);
            }
            ops::MVO => {
                let rl = instruction.left;
                let rr = instruction.right;
                let r = instruction.target;
                println!("mvo {} {} {}", r, rl, rr);
            }
            _ => println!("Invalid instruction")
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

#[cfg(target_arch = "x86_64")]
macro_rules! do_and_replace_pc {
    ($vm:expr, $jmptable:expr, $name:expr, $action:expr) => {
        unsafe {
            asm!(concat!($name, ":")
                 :
                 :
                 :
                 : "volatile");
        }

        let pc: usize;

        {
            pc = $action
        }

        dispatch!($vm, pc, $jmptable);
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
        let return_reg = reg::RET as usize + thread.base;
        *registers.get_unchecked_mut(return_reg) = (pc + 1) as i64;

        let instruction = code.get_unchecked(pc);
        let b0 = instruction.target as usize;
        let b1 = instruction.left as usize;
        let b2 = instruction.right as usize;

        let function_index = b0 | b1 << 8 | b2 << 16;
        *functions.get_unchecked(function_index) as usize
    }
}

#[inline(always)]
fn op_ret(thread: &mut Thread) -> usize {
    let registers = &mut thread.registers;
    let pc = unsafe {
        *registers.get_unchecked(reg::RET as usize + thread.base) as usize
    };
    thread.base -= 256;
    pc
}

#[inline(always)]
fn op_mov(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        *registers.get_unchecked_mut(r) = *registers.get_unchecked(rl);
    }
    pc + 1
}

#[inline(always)]
fn op_mvo(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let offset = instruction.right as usize;
        let r = instruction.target as usize + thread.base + offset;
        *registers.get_unchecked_mut(r) = *registers.get_unchecked(rl);
    }
    pc + 1
}

#[inline(never)]
fn run(thread: &mut Thread, entry_point: usize) {
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
    ops[ops::MOV as usize] = label_addr!("op_mov");
    ops[ops::MVO as usize] = label_addr!("op_mvo");

    let mut pc: usize = entry_point;

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

    do_and_replace_pc!(&thread, ops, "op_ret", {
        op_ret(thread)
    });

    do_and_dispatch!(&thread, ops, "op_mov", pc, {
        pc = op_mov(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_mvo", pc, {
        pc = op_mvo(thread, pc);
    });

    label!("op_hlt");
}

fn compile(program: &str) -> (Vec<u64>,Vec<i64>,Vec<Instruction>,usize) {
    use ast::Expression::*;
    let expressions = parser::parse_expressions(program).unwrap();
    let mut functions: Vec<u64> = Vec::new();
    let mut constants: Vec<i64> = Vec::new();
    let mut instructions: Vec<Instruction> = Vec::new();
    let mut function_mapping: HashMap<String, u32> = HashMap::new();
    let variables: HashMap<String, (Type, Register)> = HashMap::new();

    // Process function definitions first
    let filtered = expressions
        .iter()
        .filter(|&x| match *x {
            FunctionDefinition(_,_,_) => true,
            _ => false
        });
    for expression in filtered {
        assemble(&expression,
                 reg::VAL,
                 &mut constants,
                 &mut functions,
                 &mut instructions,
                 &mut function_mapping,
                 &variables);
    }

    // Now, process other expressions
    let entry_point = instructions.len() as usize;
    let filtered = expressions
        .iter()
        .filter(|&x| match *x {
            FunctionDefinition(_,_,_) => false,
            _ => true
        });
    for expression in filtered {
        assemble(&expression,
                 reg::VAL,
                 &mut constants,
                 &mut functions,
                 &mut instructions,
                 &mut function_mapping,
                 &variables);
    }

    instructions.push(Instruction {
        opcode: ops::HLT,
        target: 0,
        left: 0,
        right: 0
    });

    (functions,constants,instructions,entry_point)
}

fn main() {
    let program = concat!(
//        "(def div (a b) (/ a b))",
        "(def mul (a b) (* a b))",
        "(def add (a b c) (+ a (+ b c)))",
        "(def neg (a) (- 0 a))",
        "(neg (add 10 20 (mul 5 6)))"
//        "(neg (add 10 20 (div 16 (mul 2 2))))"
    );

    let (f, c, i, e) = compile(program);
    disassemble(&c, &f, &i);

    let mut registers: [i64; 65536] = [0; 65536];
    let mut thread = Thread {
        functions: &f,
        constants: &c,
        code: &i,
        registers: &mut registers,
        base: 0
    };
    run(&mut thread, e);

    for regs in thread.registers.iter().take(8) {
        println!("reg {}", regs);
    }
}

#[cfg(test)]
mod integers {
    use super::*;

    #[test]
    fn const_add() {
        let (f, c, i, e) = compile("(+ 1 (+ 2 123456))");

        let mut registers: [i64; 256] = [0; 256];
        let mut thread = Thread {
            functions: &f,
            constants: &c,
            code: &i,
            registers: &mut registers,
            base: 0
        };
        run(&mut thread, e);

        assert!(thread.registers[reg::VAL as usize] == 123459);
    }

    #[test]
    fn const_sub() {
        let (f, c, i, e) = compile("(- 0 (- 123456 3))");

        let mut registers: [i64; 256] = [0; 256];
        let mut thread = Thread {
            functions: &f,
            constants: &c,
            code: &i,
            registers: &mut registers,
            base: 0
        };
        run(&mut thread, e);

        assert!(thread.registers[reg::VAL as usize] == -123453);
    }

    #[test]
    fn const_mul() {
        let (f, c, i, e) = compile("(* 36 (* 36 36))");

        let mut registers: [i64; 256] = [0; 256];
        let mut thread = Thread {
            functions: &f,
            constants: &c,
            code: &i,
            registers: &mut registers,
            base: 0
        };
        run(&mut thread, e);

        assert!(thread.registers[reg::VAL as usize] == 46656);
    }

    #[test]
    fn const_div() {
        let (f, c, i, e) = compile("(/ (/ 65536 2) 2)");

        let mut registers: [i64; 256] = [0; 256];
        let mut thread = Thread {
            functions: &f,
            constants: &c,
            code: &i,
            registers: &mut registers,
            base: 0
        };
        run(&mut thread, e);

        assert!(thread.registers[reg::VAL as usize] == 16384);
    }

    #[test]
    fn combined() {
        let (f, c, i, e) = compile("(/ (+ (- 1000002 2) (- (* 500000 2) 0)) 2)");

        let mut registers: [i64; 256] = [0; 256];
        let mut thread = Thread {
            functions: &f,
            constants: &c,
            code: &i,
            registers: &mut registers,
            base: 0
        };
        run(&mut thread, e);

        assert!(thread.registers[reg::VAL as usize] == 1000000);
    }

    #[test]
    fn calls_noargs() {
        let program = concat!(
            "(def div () (/ 100 4))",
            "(def mul () (* 2 3))",
            "(def add () (+ 4 5))",
            "(def sub () (- 1000001 1000000))",
            "(+ (div) (+ (mul) (+ (add) (sub))))"
        );

        let (f, c, i, e) = compile(program);

        let mut registers: [i64; 1536] = [0; 1536];
        let mut thread = Thread {
            functions: &f,
            constants: &c,
            code: &i,
            registers: &mut registers,
            base: 0
        };
        run(&mut thread, e);

        assert!(thread.registers[reg::VAL as usize] == 41);
    }

    #[test]
    fn calls_args() {
        let program = concat!(
            "(def div (a b) (/ a b))",
            "(def mul (a b) (* a b))",
            "(def add (a b c) (+ a (+ b c)))",
            "(def neg (a) (- 0 a))",
            "(neg (add 10 20 (div 16 (mul 2 2))))"
        );

        let (f, c, i, e) = compile(program);

        let mut registers: [i64; 1536] = [0; 1536];
        let mut thread = Thread {
            functions: &f,
            constants: &c,
            code: &i,
            registers: &mut registers,
            base: 0
        };
        run(&mut thread, e);

        assert!(thread.registers[reg::VAL as usize] == -34);
    }
}
