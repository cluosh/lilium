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
#ifndef VM_OPCODES_H_
#define VM_OPCODES_H_

#define OP_NOP 0
#define OP_HLT 1
#define OP_ADD 2
#define OP_SUB 3
#define OP_MUL 4
#define OP_LOA 5
#define OP_LOV 6
#define OP_STR 7

#define REG_00 0
#define REG_01 1

#endif  // VM_OPCODES_H_
