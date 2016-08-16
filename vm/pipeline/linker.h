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
#ifndef VM_PIPELINE_LINKER_H_
#define VM_PIPELINE_LINKER_H_

namespace VM {

// Forward declarations
namespace Data {
struct ProgramBuffer;
}  // namespace Data

namespace Pipeline {

/**
 * Resolve function addresses and offsets.
 */
class Linker {
 public:
  void execute(Data::ProgramBuffer *buffer);
};

}  // namespace Pipeline
}  // namespace VM

#endif  // VM_PIPELINE_LINKER_H_
