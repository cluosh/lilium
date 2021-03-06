/// Type definitions and serializations of types used in the VM and in other modules

#[derive(Serialize, Deserialize, Clone)]
pub struct Instruction {
    pub opcode: Opcode,
    pub target: Register,
    pub left: Register,
    pub right: Register
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

/// Definition of the register type and a list of special registers
pub type Register = u8;
pub mod reg {
    use super::*;
    pub const RET: Register = 0;
    pub const VAL: Register = 1;
}

/// Definition of the opcode type and a listing of valid operations
pub type Opcode = u8;
pub mod ops {
    use super::*;
    pub const HLT: Opcode =  0;
    pub const LD:  Opcode =  1;
    pub const LDB: Opcode =  2;
    pub const LDR: Opcode =  3;
    pub const ADD: Opcode =  4;
    pub const SUB: Opcode =  5;
    pub const MUL: Opcode =  6;
    pub const DIV: Opcode =  7;
    pub const AND: Opcode =  8;
    pub const OR:  Opcode =  9;
    pub const NOT: Opcode = 10;
    pub const EQ:  Opcode = 11;
    pub const LT:  Opcode = 12;
    pub const LE:  Opcode = 13;
    pub const GT:  Opcode = 14;
    pub const GE:  Opcode = 15;
    pub const NEQ: Opcode = 16;
    pub const CAL: Opcode = 17;
    pub const TLC: Opcode = 18;
    pub const RET: Opcode = 19;
    pub const MOV: Opcode = 20;
    pub const MVO: Opcode = 21;
    pub const JMF: Opcode = 22;
    pub const JMB: Opcode = 23;
    pub const JTF: Opcode = 24;
    pub const WRI: Opcode = 25;
    pub const RDI: Opcode = 26;
}

/// A listing of possible types
pub type Type = u8;
pub mod types {
    use super::*;
    pub const INT: Type = 0;
    //pub const FLOAT: Type = 0;
    //pub const INTLIST: Type = 0;
    //pub const FLOATLIST: Type = 0;
}