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
#include <iostream>
#include <string>

#include "lang/parser/scanner.h"

namespace Parser {

/**
 * @brief Parse an integer
 * @param txt Text to be interpreted as integer
 * @return 64-bit integer
 */
std::int64_t Scanner::parse_integer(const char *txt) {
  // TODO(cluosh): Exception handling?
  return stoll(std::string(txt));
}

/**
 * @brief Parse a floating point value
 * @param txt Text to be interpreted as floating point value
 * @return Single precision float
 */
float Scanner::parse_float(const char *txt) {
  // TODO(cluosh): Exception handling?
  return stof(std::string(txt));
}

/**
 * @brief Parse a floating point value with double precision
 * @param txt Text to be interpreted as floating point value
 * @return Double precision float
 */
double Scanner::parse_double(const char *txt) {
  // TODO(cluosh): Exception handling?
  return stod(std::string(txt));
}

}  // namespace Parser
