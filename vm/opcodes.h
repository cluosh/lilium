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

/* Machine control instructions */
#define OP_NOP    0x00 /* nop */
#define OP_HALT   0x01 /* halt */

/* Load/store instructions */
#define OP_LOAD   0x02 /* load reg reg */
#define OP_STORE  0x03 /* store reg reg */
#define OP_LOADI  0x04 /* loadi reg hw */

/* Move instructions */
#define OP_MOVE   0x05 /* move reg reg */
#define OP_MOVEW  0x06 /* movew reg reg */
#define OP_MOEVH  0x07 /* moveh reg reg */

/* Arithmetic instructions */
#define OP_SHIFTL 0x08 /* shiftl reg reg reg */
#define OP_SHIFTR 0x09 /* shiftr reg reg reg */
#define OP_ADD    0x0A /* add reg reg reg */
#define OP_SUB    0x0B /* sub reg reg reg */
#define OP_MUL    0x0C /* mul reg reg reg */
#define OP_DIV    0x0D /* div reg reg reg */
#define OP_NEG    0x0E /* neg reg reg */

/* Logical instructions */
#define OP_AND    0x0F /* and reg reg reg */
#define OP_OR     0x10 /* or reg reg reg */
#define OP_XOR    0x11 /* xor reg reg reg */
#define OP_NOT    0x12 /* not reg reg */

/* Comparison instructions */
#define OP_CMP    0x13 /* cmp reg reg reg */
#define OP_LT     0x14 /* lt reg reg reg */
#define OP_LE     0x15 /* le reg reg reg */
#define OP_GT     0x16 /* gt reg reg reg */
#define OP_GE     0x17 /* ge reg reg reg */

/* Jump/branch instructions */
#define OP_JMP    0x18 /* jmp hw */
#endif  // VM_OPCODES_H_
