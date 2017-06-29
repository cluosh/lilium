pub enum Expression {
    Integer(i64),
    Variable(String),
    BinaryOp(String, Box<Expression>, Box<Expression>),
    Function(String, Vec<Expression>),
    FunctionDefinition(String, Vec<String>, Vec<Expression>),
    VariableAssignment(Vec<(String, Expression)>, Vec<Expression>),
    Conditional(Box<Expression>,Vec<Expression>,Vec<Expression>)
}
