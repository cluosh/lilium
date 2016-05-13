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
#ifndef LANG_AST_VAR_H_
#define LANG_AST_VAR_H_

#include <string>

namespace AST {

/**
 * @brief A list of variables/parameters
 */
class Var {
 private:
  std::string name;
  Var *next = nullptr;

 public:
  explicit Var(std::string name);
  ~Var();

  void set_next(Var *next);
  std::string to_string(std::string indentation);
};

}  // namespace AST

#endif  // LANG_AST_VAR_H_
