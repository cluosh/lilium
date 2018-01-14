mod codegen;
mod parser;

use common::Module;

pub fn compile(program: &str) -> Module {
    let expressions = parser::parse_expressions(program).unwrap();
    codegen::generate(&expressions)
}
