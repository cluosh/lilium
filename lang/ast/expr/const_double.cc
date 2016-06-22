/*
 * Copyright (C) 2016  Michael Pucher (cluosh)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include "lang/ast/expr/const_double.h"

namespace AST {

/**
 * Initialize constant integer node with value.
 *
 * @param value Integer value to be assigned to this node
 * @param next Next expression in list.
 */
ConstDouble::ConstDouble(double value, Expr *next)
    : Expr(VM::TYPE_DOUBLEP, next) {
  this->value = value;
}

}  // namespace AST

