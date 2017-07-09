/// Compiler module exports
pub mod ast;
mod codegen;
mod parser;

use vm::Module;

pub fn compile(program: &str) -> Module {
    let expressions = parser::parse_expressions(program).unwrap();
    codegen::generate(&expressions)
}
