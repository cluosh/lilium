#![feature(try_from)]
#![feature(asm)]
#![feature(use_nested_groups)]
#![allow(unused_assignments)]

#[macro_use]
extern crate serde_derive;
extern crate lalrpop_util;

mod common;
mod compiler;
mod disassembler;
mod vm;

pub use compiler::compile;
pub use disassembler::disassemble;
pub use vm::run;
pub use common::{Instruction, Module, Thread, reg};