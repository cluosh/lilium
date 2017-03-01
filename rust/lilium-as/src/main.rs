/// Main hub for the lilium bytecode assembler
use std::io::prelude::*;
use std::io::BufReader;
use std::fs::File;
use std::env;
use std::fmt;

/// Instruction, all instructions are exactly 4 byte long
struct Instruction {
    opcode: u8,
    a0: u8,
    a1: u8,
    a2: u8,
}

/// Print the (byte) representation of an instruction
impl fmt::Display for Instruction {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        // Return the hexadecimal representation of the instruction
        write!(f,
               "{0:02x}{1:02x}{2:02x}{3:02x}",
               self.opcode,
               self.a0,
               self.a1,
               self.a2)
    }
}

/// Parse a single line of an assembly file:
/// * Check whether a label has been defined.
/// * Check whether an instruction with arguments has been specified.
/// The processed information is emitted directly to the standard output.
///
/// Arguments
/// * **line**: the line to be parsed
fn parse_line(line: &str) {
    let mut iter = line.split_whitespace();
    if let Some(mut token) = iter.next() {
        // Handle labels as a seperate case
        if token.ends_with(':') {
            process_label(token);

            // Skip label, we don't need it anymore
            token = match iter.next() {
                Some(next_token) => next_token,
                None => return,
            };
        }

        // Handle instruction/annotation groups
        match token {
            x if x.ends_with('i') => process_intermediate(token, &mut iter),
            _ => process_instruction(token, &mut iter),
        }
    }
}

/// Deal with label inputs and store cross-references.
///
/// Arguments
/// * **token**: the token corresponding to the label
fn process_label(token: &str) {
    // TODO: Label processing
}

/// Process an instruction including its arguments, processed instructions
/// are emitted to standard output.
///
/// Arguments:
/// * **instruction**: the token corresponding to the instruction name
/// * **argument**: iterator for the rest of the instruction arguments
fn process_instruction<'a, I>(instruction: &str, arguments: &mut I)
    where I: Iterator<Item = &'a str>
{
    // Error messages
    let missing_args = "Missing arguments for instruction";
    let invalid_reg = "Invalid register specification";
    let numeric_arg = "Expected numeric argument";

    // Parse instructions
    match instruction {
        "hlt" => {
            let inst = Instruction {
                opcode: 0x00,
                a0: 0x00,
                a1: 0x00,
                a2: 0x00,
            };
            println!("{}", inst);
        }
        "nop" => {
            let inst = Instruction {
                opcode: 0x01,
                a0: 0x00,
                a1: 0x00,
                a2: 0x00,
            };
            println!("{}", inst);
        }
        "add" => {
            // Load name of destination register
            let reg_name = arguments.next().expect(missing_args);
            let dest: String = reg_name.chars().skip(1).collect();
            let dest: u8 = dest.parse().expect(invalid_reg);

            // Load name of first operand
            let reg_name = arguments.next().expect(missing_args);
            let fst: String = reg_name.chars().skip(1).collect();
            let fst: u8 = fst.parse().expect(invalid_reg);

            // Load name of second operand
            let reg_name = arguments.next().expect(missing_args);
            let snd: String = reg_name.chars().skip(1).collect();
            let snd: u8 = snd.parse().expect(invalid_reg);

            // Create add instruction with three operands
            let inst = Instruction {
                opcode: 0x02,
                a0: dest,
                a1: fst,
                a2: snd
            };
            println!("{}", inst);
        }
        _ => panic!("Invalid instruction: {}", instruction),
    }
}

/// Process an instruction using intermediate values, including its arguments,
/// processed instructions are emitted to standard output.
///
/// Arguments:
/// * **instruction**: the token corresponding to the instruction name
/// * **argument**: iterator for the rest of the instruction arguments
fn process_intermediate<'a, I>(instruction: &str, arguments: &mut I)
    where I: Iterator<Item = &'a str>
{
    // Error messages
    let missing_args = "Missing arguments for instruction";
    let invalid_reg = "Invalid register specification";
    let numeric_arg = "Expected numeric argument";

    // Parse instructions
    match instruction {
        "ldi" => {
            // Load register name
            let reg_name = arguments.next().expect(missing_args);
            let reg: String = reg_name.chars().skip(1).collect();
            let reg: u8 = reg.parse().expect(invalid_reg);

            // Load 16-bit number
            let num = arguments.next().expect(missing_args);
            let num: u16 = num.parse().expect(numeric_arg);
            let lo = num as u8;
            let hi = (num >> 8) as u8;

            // Load register name and intermediate value
            let inst = Instruction {
                opcode: 0x02,
                a0: reg,
                a1: lo,
                a2: hi,
            };
            println!("{}", inst);
        }
        _ => panic!("Invalid instruction: {}", instruction),
    }
}

fn main() {
    if let Some(input) = env::args().nth(1) {
        // Try to read file specified as argument
        let file = match File::open(input) {
            Ok(file) => file,
            Err(e) => {
                println!("Error opening input file: {}", e);
                return;
            }
        };

        // Read assembler file line by line
        let reader = BufReader::new(&file);
        for line_result in reader.lines() {
            let line = match line_result {
                Ok(line) => line,
                Err(e) => {
                    println!("Error reading input file: {}", e);
                    return;
                }
            };
            parse_line(&line);
        }
    } else {
        println!("No input file has been specified.");
    }
}
