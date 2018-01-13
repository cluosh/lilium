#![feature(try_from)]
#![feature(asm)]

#![allow(inline_always)]
#![allow(unused_assignments)]

#[macro_use]
extern crate serde_derive;
extern crate serde;
extern crate bincode;
extern crate lalrpop_util;

mod compiler;
mod vm;

#[cfg(test)]
mod tests;

use bincode::{serialize, deserialize, Infinite};
use vm::*;
use vm::dispatch::run;
use compiler::compile;
use compiler::bytecode::disassemble;

fn main() {
    use std::io::Read;
    use std::io::Write;

    let args: Vec<_> = std::env::args().collect();
    let mut args = args.iter();
    args.next().expect("Program name has been altered");

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

                    let m = compile(&contents);
                    let mut bc_name = fname.to_string();
                    bc_name.push_str(".bc");
                    let bc = std::fs::File::create(bc_name)
                        .expect("Could not create bytecode output file");
                    let mut writer = std::io::BufWriter::new(bc);
                    let encoded: Vec<u8> = serialize(&m, Infinite)
                        .expect("Could not serialize module into bytecode");
                    if writer.write_all(&encoded).is_err() {
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
