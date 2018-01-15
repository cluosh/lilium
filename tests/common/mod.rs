macro_rules! run_program {
    ($program:expr, $registers:expr) => {
        {
            let Module {
                functions: f,
                constants: c,
                entry_point: e,
                code: i
            } = compile($program);

            let mut registers: [i64; $registers] = [0; $registers];
            let mut thread = Thread {
                functions: &f,
                constants: &c,
                code: &i,
                registers: &mut registers,
                base: 0
            };
            run(&mut thread, e as usize);

            thread.registers[reg::VAL as usize]
        }
    }
}
