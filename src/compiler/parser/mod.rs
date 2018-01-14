mod parser;

pub use self::parser::parse_expressions;

pub enum Expression {
    Integer(i64),
    Variable(String),
    BinaryOp(String, Box<Expression>, Box<Expression>),
    UnaryOp(String, Box<Expression>),
    NullaryOp(String),
    Function(String, Vec<Expression>),
    FunctionDefinition(String, Vec<String>, Vec<Expression>),
    VariableAssignment(Vec<(String, Expression)>, Vec<Expression>),
    Conditional(Box<Expression>,Vec<Expression>,Vec<Expression>)
}