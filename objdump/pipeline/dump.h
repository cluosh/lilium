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
#ifndef OBJDUMP_PIPELINE_DUMP_H_
#define OBJDUMP_PIPELINE_DUMP_H_

#include <ostream>
#include <string>

#include "vm/data/program_buffer.h"

// Forward declarations
namespace VM {
namespace Data {
struct ProgramBuffer;
}  // namespace Data
}  // namespace VM

namespace OBJDUMP {
namespace Pipeline {

/**
 * A dumping stage of the pipeline.
 */
class Dump {
 private:
  std::ostream out;

 public:
  Dump(const std::ostream &output);
  void execute(const std::string &module,
               const VM::Data::ProgramBuffer &programBuffer,
               const std::vector<VM::Data::FunctionTableEntry> &functions);
};

}  // namespace Pipeline
}  // namespace OBJDUMP

#endif  // OBJDUMP_PIPELINE_DUMP_H_
