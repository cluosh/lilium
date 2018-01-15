extern crate bincode;
extern crate lilium;

use std::env;
use std::io::{Read, Write, Error, ErrorKind, Result};
use bincode::{serialize, Infinite};
use lilium::compile;

fn compile_file(file_name: &str) -> Result<()> {
    let mut file = std::fs::File::open(&file_name)?;
    let mut contents = String::new();
    file.read_to_string(&mut contents)?;

    let m = compile(&contents);
    let mut bc_name = file_name.to_string();
    bc_name.push_str(".bc");
    let bc = std::fs::File::create(bc_name)?;
    let mut writer = std::io::BufWriter::new(bc);
    let encoded: Vec<u8> = serialize(&m, Infinite)
        .map_err(|err| Error::new(ErrorKind::Other, err))?;
    writer.write_all(&encoded)?;

    Ok(())
}

fn main() {
    let mut args = env::args();
    if let Some(file_name) = args.nth(1) {
        if let Err(e) = compile_file(&file_name) {
            println!("Error during compilation: {}", e);
        }
    } else {
        println!("Usage: lcc lilium_file.l");
    }
}