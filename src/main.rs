#![feature(try_from)]
#![feature(asm)]
#[macro_use]
extern crate serde_derive;
extern crate serde;
extern crate bincode;
extern crate lalrpop_util;

mod compiler;
mod vm;

use std::convert::TryFrom;
use std::collections::HashMap;
use bincode::{serialize, deserialize, Infinite};
use vm::*;

struct Tail<'a> {
    name: &'a str,
    address: usize,
    tailing: bool
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
            ops::AND => {
                let rl = instruction.left;
                let rr = instruction.right;
                let r = instruction.target;
                println!("and {} {} {}", r, rl, rr);
            }
            ops::OR => {
                let rl = instruction.left;
                let rr = instruction.right;
                let r = instruction.target;
                println!("or {} {} {}", r, rl, rr);
            }
            ops::NOT => {
                let rl = instruction.left;
                let r = instruction.target;
                println!("not {} {}", r, rl);
            }
            ops::EQ => {
                let rl = instruction.left;
                let rr = instruction.right;
                let r = instruction.target;
                println!("eq {} {} {}", r, rl, rr);
            }
            ops::LT => {
                let rl = instruction.left;
                let rr = instruction.right;
                let r = instruction.target;
                println!("lt {} {} {}", r, rl, rr);
            }
            ops::LE => {
                let rl = instruction.left;
                let rr = instruction.right;
                let r = instruction.target;
                println!("le {} {} {}", r, rl, rr);
            }
            ops::GT => {
                let rl = instruction.left;
                let rr = instruction.right;
                let r = instruction.target;
                println!("gt {} {} {}", r, rl, rr);
            }
            ops::GE => {
                let rl = instruction.left;
                let rr = instruction.right;
                let r = instruction.target;
                println!("ge {} {} {}", r, rl, rr);
            }
            ops::NEQ => {
                let rl = instruction.left;
                let rr = instruction.right;
                let r = instruction.target;
                println!("neq {} {} {}", r, rl, rr);
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
            ops::JMF => {
                let rl = instruction.left as u32;
                let rr = instruction.right as u32;
                let r = instruction.target as u32;
                let addr = r | rl << 8 | rr << 16;
                println!("jmp 0x{:x}", addr);
            }
            ops::JTF => {
                let rl = instruction.left as u32;
                let rr = instruction.right as u32;
                let r = instruction.target;
                let addr = rl | rr << 8;
                println!("jmt {} 0x{:x}", r, addr);
            }
            ops::WRI => {
                let rl = instruction.left;
                let r = instruction.target;
                println!("write {} {}", r, rl);
            }
            ops::RDI => {
                let r = instruction.target;
                println!("read {}", r);
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
fn op_and(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = (left != 0 && right != 0) as i64;
    }
    pc + 1
}

#[inline(always)]
fn op_or(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = (left != 0 || right != 0) as i64;
    }
    pc + 1
}

#[inline(always)]
fn op_not(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        *registers.get_unchecked_mut(r) = (left == 0) as i64;
    }
    pc + 1
}

#[inline(always)]
fn op_eq(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = (left == right) as i64;
    }
    pc + 1
}

#[inline(always)]
fn op_lt(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = (left < right) as i64;
    }
    pc + 1
}

#[inline(always)]
fn op_le(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = (left <= right) as i64;
    }
    pc + 1
}

#[inline(always)]
fn op_gt(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = (left > right) as i64;
    }
    pc + 1
}

#[inline(always)]
fn op_ge(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = (left >= right) as i64;
    }
    pc + 1
}

#[inline(always)]
fn op_neq(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = (left != right) as i64;
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

#[inline(always)]
fn op_jmf(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let b0 = instruction.target as usize;
        let b1 = instruction.left as usize;
        let b2 = instruction.right as usize;
        let offset = b0 | b1 << 8 | b2 << 16;
        pc + offset
    }
}

#[inline(always)]
fn op_jmb(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let b0 = instruction.target as usize;
        let b1 = instruction.left as usize;
        let b2 = instruction.right as usize;
        let offset = b0 | b1 << 8 | b2 << 16;
        pc - offset
    }
}

#[inline(always)]
fn op_jtf(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize;
        let rr = instruction.right as usize;
        let r = instruction.target as usize + thread.base;
        let offset = rl | rr << 8;
        if *registers.get_unchecked(r) != 0 {
            pc + offset
        } else {
            pc + 1
        }
    }
}

#[inline(always)]
fn op_wri(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        *registers.get_unchecked_mut(r) = left;

        println!("{}", left);
    }
    pc + 1
}

#[inline(always)]
fn op_rdi(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let r = instruction.target as usize + thread.base;

        let mut input_text = String::new();
        std::io::stdin()
            .read_line(&mut input_text)
            .expect("Could not read from stdio");
        *registers.get_unchecked_mut(r) = match input_text.trim().parse::<i64>() {
            Ok(i) => i,
            _ => panic!("Could not read integer")
        };
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
    ops[ops::AND as usize] = label_addr!("op_and");
    ops[ops::OR  as usize] = label_addr!("op_or");
    ops[ops::NOT as usize] = label_addr!("op_not");
    ops[ops::EQ  as usize] = label_addr!("op_eq");
    ops[ops::LT  as usize] = label_addr!("op_lt");
    ops[ops::LE  as usize] = label_addr!("op_le");
    ops[ops::GT  as usize] = label_addr!("op_gt");
    ops[ops::GE  as usize] = label_addr!("op_ge");
    ops[ops::NEQ as usize] = label_addr!("op_neq");
    ops[ops::CAL as usize] = label_addr!("op_cal");
    ops[ops::RET as usize] = label_addr!("op_ret");
    ops[ops::MOV as usize] = label_addr!("op_mov");
    ops[ops::MVO as usize] = label_addr!("op_mvo");
    ops[ops::JMF as usize] = label_addr!("op_jmf");
    ops[ops::JMB as usize] = label_addr!("op_jmb");
    ops[ops::JTF as usize] = label_addr!("op_jtf");
    ops[ops::WRI as usize] = label_addr!("op_wri");
    ops[ops::RDI as usize] = label_addr!("op_rdi");

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

    do_and_dispatch!(&thread, ops, "op_and", pc, {
        pc = op_and(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_or", pc, {
        pc = op_or(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_not", pc, {
        pc = op_not(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_eq", pc, {
        pc = op_eq(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_lt", pc, {
        pc = op_lt(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_le", pc, {
        pc = op_le(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_gt", pc, {
        pc = op_gt(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_ge", pc, {
        pc = op_ge(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_neq", pc, {
        pc = op_neq(thread, pc);
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

    do_and_dispatch!(&thread, ops, "op_jmf", pc, {
        pc = op_jmf(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_jmb", pc, {
        pc = op_jmb(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_jtf", pc, {
        pc = op_jtf(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_wri", pc, {
        pc = op_wri(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_rdi", pc, {
        pc = op_rdi(thread, pc);
    });

    label!("op_hlt");
}

fn main() {
    use std::io::Read;
    use std::io::Write;

    let args: Vec<_> = std::env::args().collect();
    let mut args = args.iter();
    args.next().unwrap();
    let mode = args.next();
    if let Some(m) = mode {
        match m.as_ref() {
            "c" => {
                if let Some(fname) = args.next() {
                    let mut file = std::fs::File::open(&fname)
                        .expect("Unable to open the file");
                    let mut contents = String::new();
                    file.read_to_string(&mut contents)
                        .expect("Unable to read the file");

                    let (f, c, i, e) = compile(&contents);
                    let m = Module {
                        functions: f,
                        constants: c,
                        entry_point: e as u64,
                        code: i
                    };

                    let bc = std::fs::File::create(fname.to_string() + ".bc")
                        .expect("Could not create bytecode output file");
                    let mut writer = std::io::BufWriter::new(bc);
                    let encoded: Vec<u8> = serialize(&m, Infinite).unwrap();
                    if let Err(_) = writer.write_all(&encoded) {
                        println!("Could not create bytecode file");
                    }
                } else {
                    println!("Please specify filename");
                }
            }
            "d" => {
                if let Some(fname) = args.next() {
                    let mut file = std::fs::File::open(&fname)
                        .expect("Unable to open the file");
                    let mut contents: Vec<u8> = Vec::new();
                    file.read_to_end(&mut contents)
                        .expect("Unable to read the file");

                    let m: Module = deserialize(&contents)
                        .expect("Invalid bytecode module");
                    disassemble(&m.constants, &m.functions, &m.code);
                } else {
                    println!("Please specify filename");
                }
            }
            "r" => {
                if let Some(fname) = args.next() {
                    let mut file = std::fs::File::open(fname)
                        .expect("Unable to open the file");
                    let mut contents: Vec<u8> = Vec::new();
                    file.read_to_end(&mut contents)
                        .expect("Unable to read the file");

                    let m: Module = deserialize(&contents)
                        .expect("Invalid bytecode module");
                    let mut registers: [i64; 65536] = [0; 65536];
                    let mut thread = Thread {
                        functions: &m.functions,
                        constants: &m.constants,
                        code: &m.code,
                        registers: &mut registers,
                        base: 0
                    };
                    run(&mut thread, m.entry_point as usize);
                }
            }
            _ => {
                println!("Unknown mode");
            }
        }
    } else {
        println!("Specify mode (c, d) or filename.");
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

    #[test]
    fn calls_vars() {
        let program = concat!(
            "(def fun (a b) (let ((c (* a b)) (d (+ a b))) (+ (- c d) (* d c))))",
            "(def neg (a) (- 0 a))",
            "(neg (fun 10 20))"
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

        assert!(thread.registers[reg::VAL as usize] == -6170);
    }

    #[test]
    fn conditional_const() {
        let program = concat!(
            "(def asub (a b) (if (> a b) ((- a b)) ((- b a))))",
            "(asub 100 200)"
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

        assert!(thread.registers[reg::VAL as usize] == 100);

        let program = concat!(
            "(def asub (a b) (if (> a b) ((- a b)) ((- b a))))",
            "(asub 200 100)"
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

        assert!(thread.registers[reg::VAL as usize] == 100);
    }
}

#[cfg(test)]
mod logic {
    use super::*;

    #[test]
    fn const_and() {
        let (f, c, i, e) = compile("(& 1 0)");

        let mut registers: [i64; 256] = [0; 256];
        let mut thread = Thread {
            functions: &f,
            constants: &c,
            code: &i,
            registers: &mut registers,
            base: 0
        };
        run(&mut thread, e);

        assert!(thread.registers[reg::VAL as usize] == 0);
    }

    #[test]
    fn const_or() {
        let (f, c, i, e) = compile("(| 1 0)");

        let mut registers: [i64; 256] = [0; 256];
        let mut thread = Thread {
            functions: &f,
            constants: &c,
            code: &i,
            registers: &mut registers,
            base: 0
        };
        run(&mut thread, e);

        assert!(thread.registers[reg::VAL as usize] == 1);
    }

    #[test]
    fn conditional() {
        let program = concat!(
            "(def fun (a b)",
            "  (if ",
            "     (> a 0)",
            "     ((fun (- a 1) (+ b 1)))",
            "     ((+ b 1))))",
            "(fun 20 2)"
        );

        let (f, c, i, e) = compile(program);

        let mut registers: [i64; 6144] = [0; 6144];
        let mut thread = Thread {
            functions: &f,
            constants: &c,
            code: &i,
            registers: &mut registers,
            base: 0
        };
        run(&mut thread, e);

        assert!(thread.registers[reg::VAL as usize] == 23);
    }
}
