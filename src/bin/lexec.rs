extern crate bincode;
extern crate lilium;

use std::env;
use std::io::{Read, Error, ErrorKind, Result};
use bincode::deserialize;
use lilium::{Module, Thread, run};

fn execute_file(file_name: &str) -> Result<()> {
    let mut file = std::fs::File::open(file_name)?;
    let mut contents: Vec<u8> = Vec::new();
    file.read_to_end(&mut contents)?;

    let m: Module = deserialize(&contents)
        .map_err(|err| Error::new(ErrorKind::Other, err))?;

    let mut registers: [i64; 65536] = [0; 65536];
    let mut thread = Thread {
        functions: &m.functions,
        constants: &m.constants,
        code: &m.code,
        registers: &mut registers,
        base: 0
    };

    run(&mut thread, m.entry_point as usize);

    Ok(())
}

fn main() {
    let mut args = env::args();
    if let Some(file_name) = args.nth(1) {
        if let Err(e) = execute_file(&file_name) {
            println!("Error during execution: {}", e);
        }
    } else {
        println!("Usage: lexec lilium_bytecode.bc");
    }
}