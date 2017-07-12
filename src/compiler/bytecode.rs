use vm::*;
use vm::atoms::*;

pub fn disassemble(constants: &[i64],
                   functions: &[u64],
                   instructions: &[Instruction]) {
    for (pc, instruction) in instructions.iter().enumerate() {
        print!("0x{:05x}: ", pc);

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
