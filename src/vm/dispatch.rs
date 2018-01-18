use std;
use common::*;

#[inline(never)]
pub fn run(thread: &mut Thread, entry_point: usize) {
    let mut ops: [usize; 32] = [label_addr!("op_hlt"); 32];

    ops[ops::HLT as usize] = label_addr!("op_hlt");
    ops[ops::LD  as usize] = label_addr!("op_ld");
    ops[ops::LDB as usize] = label_addr!("op_ldb");
    ops[ops::LDR as usize] = label_addr!("op_ldr");
    ops[ops::ADD as usize] = label_addr!("op_add");
    ops[ops::SUB as usize] = label_addr!("op_sub");
    ops[ops::MUL as usize] = label_addr!("op_mul");
    ops[ops::DIV as usize] = label_addr!("op_div");
    ops[ops::AND as usize] = label_addr!("op_and");
    ops[ops::OR  as usize] = label_addr!("op_or");
    ops[ops::NOT as usize] = label_addr!("op_not");
    ops[ops::EQ  as usize] = label_addr!("op_eq");
    ops[ops::LT  as usize] = label_addr!("op_lt");
    ops[ops::LE  as usize] = label_addr!("op_le");
    ops[ops::GT  as usize] = label_addr!("op_gt");
    ops[ops::GE  as usize] = label_addr!("op_ge");
    ops[ops::NEQ as usize] = label_addr!("op_neq");
    ops[ops::CAL as usize] = label_addr!("op_cal");
    ops[ops::TLC as usize] = label_addr!("op_tlc");
    ops[ops::RET as usize] = label_addr!("op_ret");
    ops[ops::MOV as usize] = label_addr!("op_mov");
    ops[ops::MVO as usize] = label_addr!("op_mvo");
    ops[ops::JMF as usize] = label_addr!("op_jmf");
    ops[ops::JMB as usize] = label_addr!("op_jmb");
    ops[ops::JTF as usize] = label_addr!("op_jtf");
    ops[ops::WRI as usize] = label_addr!("op_wri");
    ops[ops::RDI as usize] = label_addr!("op_rdi");

    let mut pc: usize = entry_point;

    dispatch!(&thread, pc, ops);

    do_and_dispatch!(&thread, ops, "op_ld", pc, {
        pc = op_ld(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_ldb", pc, {
        pc = op_ldb(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_ldr", pc, {
        pc = op_ldr(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_add", pc, {
        pc = op_add(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_sub", pc, {
        pc = op_sub(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_mul", pc, {
        pc = op_mul(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_div", pc, {
        pc = op_div(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_and", pc, {
        pc = op_and(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_or", pc, {
        pc = op_or(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_not", pc, {
        pc = op_not(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_eq", pc, {
        pc = op_eq(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_lt", pc, {
        pc = op_lt(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_le", pc, {
        pc = op_le(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_gt", pc, {
        pc = op_gt(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_ge", pc, {
        pc = op_ge(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_neq", pc, {
        pc = op_neq(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_cal", pc, {
        pc = op_cal(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_tlc", pc, {
        pc = op_tlc(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_ret", pc, {
        pc = op_ret(thread)
    });

    do_and_dispatch!(&thread, ops, "op_mov", pc, {
        pc = op_mov(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_mvo", pc, {
        pc = op_mvo(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_jmf", pc, {
        pc = op_jmf(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_jmb", pc, {
        pc = op_jmb(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_jtf", pc, {
        pc = op_jtf(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_wri", pc, {
        pc = op_wri(thread, pc);
    });

    do_and_dispatch!(&thread, ops, "op_rdi", pc, {
        pc = op_rdi(thread, pc);
    });

    label!("op_hlt");
}

#[inline(always)]
fn op_ld(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let number = instruction.left as u16 | (instruction.right as u16) << 8;

        let r = instruction.target as usize + thread.base;
        *registers.get_unchecked_mut(r) = number as i64;
    }
    pc + 1
}

#[inline(always)]
fn op_ldb(thread: &mut Thread, pc: usize) -> usize {
    let constants = &thread.constants;
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let index = instruction.left as u16 | (instruction.right as u16) << 8;
        let c = *constants.get_unchecked(index as usize);

        let r = instruction.target as usize + thread.base;
        *registers.get_unchecked_mut(r) = c;
    }
    pc + 1
}

#[inline(always)]
fn op_ldr(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let r = code.get_unchecked(pc).target as usize + thread.base;
        let rval = *registers.get_unchecked(reg::VAL as usize + thread.base + 256);
        *registers.get_unchecked_mut(r) = rval;
    }
    pc + 1
}

#[inline(always)]
fn op_add(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = left + right;
    }
    pc + 1
}

#[inline(always)]
fn op_sub(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = left - right;
    }
    pc + 1
}

#[inline(always)]
fn op_mul(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = left * right;
    }
    pc + 1
}

#[inline(always)]
fn op_div(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = left / right;
    }
    pc + 1
}

#[inline(always)]
fn op_and(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = (left != 0 && right != 0) as i64;
    }
    pc + 1
}

#[inline(always)]
fn op_or(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = (left != 0 || right != 0) as i64;
    }
    pc + 1
}

#[inline(always)]
fn op_not(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        *registers.get_unchecked_mut(r) = (left == 0) as i64;
    }
    pc + 1
}

#[inline(always)]
fn op_eq(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = (left == right) as i64;
    }
    pc + 1
}

#[inline(always)]
fn op_lt(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = (left < right) as i64;
    }
    pc + 1
}

#[inline(always)]
fn op_le(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = (left <= right) as i64;
    }
    pc + 1
}

#[inline(always)]
fn op_gt(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = (left > right) as i64;
    }
    pc + 1
}

#[inline(always)]
fn op_ge(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = (left >= right) as i64;
    }
    pc + 1
}

#[inline(always)]
fn op_neq(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let rr = instruction.right as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        let right = *registers.get_unchecked(rr);
        *registers.get_unchecked_mut(r) = (left != right) as i64;
    }
    pc + 1
}

#[inline(always)]
fn op_cal(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let functions = &thread.functions;
    let registers = &mut thread.registers;
    thread.base += 256;
    unsafe {
        let return_reg = reg::RET as usize + thread.base;
        *registers.get_unchecked_mut(return_reg) = (pc + 1) as i64;

        let instruction = code.get_unchecked(pc);
        let b0 = instruction.target as usize;
        let b1 = instruction.left as usize;
        let b2 = instruction.right as usize;

        let function_index = b0 | b1 << 8 | b2 << 16;
        *functions.get_unchecked(function_index) as usize
    }
}

#[inline(always)]
fn op_tlc(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let functions = &thread.functions;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let b0 = instruction.target as usize;
        let b1 = instruction.left as usize;
        let b2 = instruction.right as usize;

        let function_index = b0 | b1 << 8 | b2 << 16;
        *functions.get_unchecked(function_index) as usize
    }
}

#[inline(always)]
fn op_ret(thread: &mut Thread) -> usize {
    let registers = &mut thread.registers;
    let pc = unsafe {
        *registers.get_unchecked(reg::RET as usize + thread.base) as usize
    };
    thread.base -= 256;
    pc
}

#[inline(always)]
fn op_mov(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        *registers.get_unchecked_mut(r) = *registers.get_unchecked(rl);
    }
    pc + 1
}

#[inline(always)]
fn op_mvo(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let offset = instruction.right as usize;
        let r = instruction.target as usize + thread.base + offset;
        *registers.get_unchecked_mut(r) = *registers.get_unchecked(rl);
    }
    pc + 1
}

#[inline(always)]
fn op_jmf(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let b0 = instruction.target as usize;
        let b1 = instruction.left as usize;
        let b2 = instruction.right as usize;
        let offset = b0 | b1 << 8 | b2 << 16;
        pc + offset
    }
}

#[inline(always)]
fn op_jmb(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let b0 = instruction.target as usize;
        let b1 = instruction.left as usize;
        let b2 = instruction.right as usize;
        let offset = b0 | b1 << 8 | b2 << 16;
        pc - offset
    }
}

#[inline(always)]
fn op_jtf(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize;
        let rr = instruction.right as usize;
        let r = instruction.target as usize + thread.base;
        let offset = rl | rr << 8;
        if *registers.get_unchecked(r) == 0 {
            pc + 1
        } else {
            pc + offset
        }
    }
}

#[inline(always)]
fn op_wri(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let rl = instruction.left as usize + thread.base;
        let r = instruction.target as usize + thread.base;
        let left = *registers.get_unchecked(rl);
        *registers.get_unchecked_mut(r) = left;

        println!("{}", left);
    }
    pc + 1
}

#[inline(always)]
fn op_rdi(thread: &mut Thread, pc: usize) -> usize {
    let code = &thread.code;
    let registers = &mut thread.registers;
    unsafe {
        let instruction = code.get_unchecked(pc);
        let r = instruction.target as usize + thread.base;

        let mut input_text = String::new();
        std::io::stdin()
            .read_line(&mut input_text)
            .expect("Could not read from stdio");
        *registers.get_unchecked_mut(r) = match input_text.trim().parse::<i64>() {
            Ok(i) => i,
            _ => panic!("Could not read integer")
        };
    }
    pc + 1
}
