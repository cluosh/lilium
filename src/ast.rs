use std::fmt::{Debug, Formatter, Error};

pub enum Expression {
    Integer(i64),
    Function(String, Vec<Expression>),
}

impl Debug for Expression {
    fn fmt(&self, fmt: &mut Formatter) -> Result<(), Error> {
        use self::Expression::*;
        match *self {
            Integer(n) => write!(fmt, "{:?}", n),
            Function(ref name, ref ops) => write!(fmt, "({} {:?})", name, ops),
        }
    }
}
