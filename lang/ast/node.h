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
#ifndef LANG_AST_NODE_H_
#define LANG_AST_NODE_H_

#include <list>
#include <string>

namespace AST {

/**
 * A node in an abstract syntax tree
 */
class Node {
 private:
  Node *parent;
  std::list<Node *> nodes;
  std::string value;

 public:
  Node(Node *parent, std::string value);
  ~Node();

  void add(Node *child);
  void print(std::string indent);
};

}  // namespace AST

#endif  // LANG_AST_NODE_H_
