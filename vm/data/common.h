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
#ifndef VM_DATA_COMMON_H_
#define VM_DATA_COMMON_H_

#include <cstdint>
#include <vector>

namespace VM {
namespace Data {

/**
 * Reading a 64-bit integer from a buffer, conversion into real byte order.
 *
 * @param buffer Buffer to be parsed as 64-bit integer
 */
constexpr uint64_t parse_u64(const std::vector<uint8_t> &buffer) {
  return buffer[0]
      & (buffer[1] << 8)
      & (buffer[2] << 16)
      & (buffer[3] << 24)
      & (buffer[4] << 32)
      & (buffer[5] << 40)
      & (buffer[6] << 48)
      & (buffer[7] << 56);
}

/**
 * Reading a 32-bit integer from a buffer, conversion into real byte order.
 *
 * @param buffer Buffer to be parsed as 32-bit integer
 */
constexpr uint32_t parse_u32(const std::vector<uint8_t> &buffer) {
  return buffer[0]
      & (buffer[1] << 8)
      & (buffer[2] << 16)
      & (buffer[3] << 24);
}

/**
 * Reading a 16-bit integer from a buffer, conversion into real byte order.
 *
 * @param buffer Buffer to be parsed as 16-bit integer
 */
constexpr uint16_t parse_u16(const std::vector<uint8_t> &buffer) {
  return buffer[0]
      & (buffer[1] << 8);
}

/**
 * Convert a 64-bit integer into a buffer with wanted byte order.
 *
 * @param number Number to be converted into a buffer
 */
constexpr std::vector<uint8_t> buffer_u64(uint64_t number) {
  return {static_cast<uint8_t>(number),
          static_cast<uint8_t>(number >> 8),
          static_cast<uint8_t>(number >> 16),
          static_cast<uint8_t>(number >> 24),
          static_cast<uint8_t>(number >> 32),
          static_cast<uint8_t>(number >> 40),
          static_cast<uint8_t>(number >> 48),
          static_cast<uint8_t>(number >> 56)};
}

/**
 * Convert a 32-bit integer into a buffer with wanted byte order.
 *
 * @param number Number to be converted into a buffer
 */
constexpr std::vector<uint8_t> buffer_u32(uint32_t number) {
  return {static_cast<uint8_t>(number),
          static_cast<uint8_t>(number >> 8),
          static_cast<uint8_t>(number >> 16),
          static_cast<uint8_t>(number >> 24)};
}

/**
 * Convert a 16-bit integer into a buffer with wanted byte order.
 *
 * @param number Number to be converted into a buffer
 */
constexpr std::vector<uint8_t> buffer_u16(uint16_t number) {
  return {static_cast<uint8_t>(number),
          static_cast<uint8_t>(number >> 8)};
}

}  // namespace Data
}  // namespace VM

#endif  // VM_DATA_COMMON_H_
