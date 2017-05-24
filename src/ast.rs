use std::fmt::{Debug, Formatter, Error};

pub enum Expression {
    Integer(i64),
    BinaryOp(String, Box<Expression>, Box<Expression>),
    Function(String, Vec<Expression>),
}

impl Debug for Expression {
    fn fmt(&self, fmt: &mut Formatter) -> Result<(), Error> {
        use self::Expression::*;
        match *self {
            Integer(n) => write!(fmt, "{:?}", n),
            BinaryOp(ref op, ref l, ref r) => write!(fmt, "({} {:?} {:?})", op, l, r),
            Function(ref name, ref ops) => write!(fmt, "({} {:?})", name, ops),
        }
    }
}
