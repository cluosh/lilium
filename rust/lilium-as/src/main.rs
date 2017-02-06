/// Main hub for the lilium bytecode assembler
use std::io::prelude::*;
use std::io::BufReader;
use std::fs::File;
use std::env;

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
        process_instruction(token, iter);
    }
}

/// Deal with label inputs and store cross-references.
///
/// Arguments
/// * **token**: the token corresponding to the label
fn process_label(token: &str) {
    println!("LABEL {}", token);
}

/// Process an instruction including its arguments, processed instructions
/// are emitted to standard output.
///
/// Arguments:
/// * **instruction**: the token corresponding to the instruction name
/// * **argument**: iterator for the rest of the instruction arguments
fn process_instruction<'a, I>(instruction: &str, arguments: I)
    where I: Iterator<Item = &'a str>
{
    print!("INST {} ARGS ", instruction);
    for arg in arguments {
        print!("{} ", arg);
    }
    println!();
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
        let mut reader = BufReader::new(&file);
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
