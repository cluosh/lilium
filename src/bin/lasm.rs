extern crate bincode;
extern crate lilium;

use std::env;
use std::io::{Read, Error, ErrorKind, Result};
use bincode::deserialize;
use lilium::{Module, disassemble};

fn disassemble_file(file_name: &str) -> Result<()> {
    let mut file = std::fs::File::open(&file_name)?;
    let mut contents: Vec<u8> = Vec::new();
    file.read_to_end(&mut contents)?;

    let m: Module = deserialize(&contents)
        .map_err(|err| Error::new(ErrorKind::Other, err))?;
    disassemble(&m.constants, &m.functions, &m.code);

    Ok(())
}

fn main() {
    let mut args = env::args();
    if let Some(file_name) = args.nth(1) {
        if let Err(e) = disassemble_file(&file_name) {
            println!("Error during disassembly: {}", e);
        }
    } else {
        println!("Usage: lasm lilium_bytecode.bc");
    }
}