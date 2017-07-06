/// Type definitions and serializations of types used in the actual VM
pub mod ops;
pub mod reg;
pub mod types;

#[derive(Serialize, Deserialize)]
pub struct Instruction {
    pub opcode: ops::Opcode,
    pub target: reg::Register,
    pub left: reg::Register,
    pub right: reg::Register
}

#[derive(Serialize, Deserialize)]
pub struct Module {
    pub functions: Vec<u64>,
    pub constants: Vec<i64>,
    pub entry_point: u64,
    pub code: Vec<Instruction>
}

pub struct Thread<'a> {
    pub functions: &'a [u64],
    pub constants: &'a [i64],
    pub code: &'a [Instruction],
    pub registers: &'a mut [i64],
    pub base: usize
}
