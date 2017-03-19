#[macro_use]
extern crate serde_derive;
extern crate bincode;

use std::env;
use std::io::Read;
use std::io::Write;
use std::fs::File;
use bincode::deserialize;

/// A complete bytecode module
#[derive(Deserialize)]
struct Module {
    instructions: Vec<Instruction>,
}

/// Instruction, all instructions are exactly 4 byte long
#[derive(Deserialize)]
struct Instruction {
    opcode: u8,
    a0: u8,
    a1: u8,
    a2: u8,
}

fn main() {
    if let Some(input) = env::args().nth(1) {
        // Reading all the instructions into the vector
        // Possible speed improvement by allocating vector first? has to be profiled
        let mut buffer = Vec::new();
        match File::open(&input) {
            Ok(mut file) => {
                if let Err(e) = file.read_to_end(&mut buffer) {
                    println!("Error while reading file {}: {}", &input, e);
                    return;
                }
            }
            Err(e) => {
                println!("Error opening bytecode module {}: {}", &input, e);
                return;
            }
        }

        // Deserialize the whole bytecode module
        let module: Module = match deserialize(&buffer[..]) {
            Ok(module) => module,
            Err(e) => {
                println!("Error while parsing bytecode module: {}", e);
                return;
            }
        };

        // Dispatch loop
        let mut pc = 0;
        loop {
            // Any way to access vector element without bound check?
            // Possibly unsafe solution for this?
            let instruction = &module.instructions[pc];
            match instruction.opcode {
                0x00 => {
                    println!("HALT");
                    break;
                }
                0x01 => {
                    println!("ADD");
                    pc += 1;
                }
                0x02 => {
                    println!("SUB");
                    pc += 1;
                }
                _ => break,
            }
        }
    } else {
        println!("Usage: ./liliumvm bytecode-module");
    }
}
