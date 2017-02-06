/// Main hub for the lilium bytecode assembler

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

fn process_label(token: &str) {
    println!("LABEL {}", token);
}

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
    parse_line("l1: mov a b");
}
