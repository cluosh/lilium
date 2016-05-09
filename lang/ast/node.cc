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
#include "lang/ast/node.h"

#include <iostream>

namespace AST {

Node::Node(std::string value) {
  this->value = value;
}

Node::~Node() {
  for (auto const &node : nodes) {
    delete node;
  }
}

void Node::add(Node *child) {
  nodes.push_back(child);
}

void Node::print(std::string indent) {
  std::cout << indent << "- " << value;
  for (auto const &node : nodes) {
    node->print(indent + "  ");
  }
}

}  // namespace AST
