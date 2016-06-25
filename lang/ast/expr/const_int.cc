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
#include "lang/ast/expr/const_int.h"

namespace AST {

/**
 * Initialize constant integer node with value.
 *
 * @param value Integer value to be assigned to this node
 * @param next Next expression in list
 */
ConstInt::ConstInt(int64_t value, Expr *next) : Const(VM::TYPE_INT, next) {
  this->value = value;
}

/**
 * Attribute a constant integer definition
 *
 * @param attrib_info Info needed for attribution and code generation
 *                    later on
 */
void ConstInt::attribute(AttribInfo *attrib_info) {
  result_reg = attrib_info->next_reg;
  attrib_info->next_reg++;
}

}  // namespace AST

