// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_BITS_UTF8_H
#define PLUSH_BITS_UTF8_H

#include <cassert>
#include <cstdint>
#include <iterator>
#include <ostream>

namespace plush {

namespace utf8 {

// Gets the size of a codepoint at a specificed point within a UTF-8 encoded
// string.
constexpr std::uint_fast8_t codepointSize(char const *codepoint) {
 unsigned char const c {static_cast<unsigned char>(*codepoint)};

 bool const table[4] {
   (c & 0b10000000) == 0b00000000,
   (c & 0b11100000) == 0b11000000,
   (c & 0b11110000) == 0b11100000,
   (c & 0b11111000) == 0b11110000,
 };

 std::uint_fast8_t size {0};
 for (std::uint_fast8_t i = 0; i < 4; ++i)
  if (table[i]) size = i;
 return size + 1;
}

// Decodes a UTF-8 encoded glyph to UTF-32 via its codepoint.
constexpr char32_t decode(char const *codepoint) {
 char32_t const c {static_cast<char32_t>(*codepoint)};

 switch (codepointSize(codepoint)) {
  case 1:
   return c;
  case 2:
   return ((c & 31) << 6) | (codepoint[1] & 63);
  case 3:
   return ((c & 15) << 12) | ((codepoint[1] & 63) << 6) | (codepoint[2] & 63);
  case 4:
   return ((c & 7) << 18) | ((codepoint[1] & 63) << 12) |
          ((codepoint[2] & 63) << 6) | (codepoint[3] & 63);
 }

 assert(!"Impossible");
}

// Converts a UTF-32 glyph to UTF-8 and returns the number of bytes written.
constexpr std::size_t encode(char out[4], char32_t c) {
 if (c <= 0x7F) {
  out[0] = c;
  return 1;
 } else if (c <= 0x7FF) {
  out[0] = 0xC0 | (c >> 6);
  out[1] = 0x80 | (c & 0x3F);
  return 2;
 } else if (c <= 0xFFFF) {
  out[0] = 0xE0 | (c >> 12);
  out[1] = 0x80 | ((c >> 6) & 0x3F);
  out[2] = 0x80 | (c & 0x3F);
  return 3;
 } else if (c <= 0x10FFFF) {
  out[0] = 0xF0 | (c >> 18);
  out[1] = 0x80 | ((c >> 12) & 0x3F);
  out[2] = 0x80 | ((c >> 6) & 0x3F);
  out[3] = 0x80 | (c & 0x3F);
  return 4;
 } else
  assert(!"Invalid character");
}

std::ostream &operator<<(std::ostream &stream, char32_t c);

// UTF-8 iterator wrapper for forward char iterator types.
template <class FwdCharIt>
class ConstUtf8IteratorFor {
 // Internal iterator.
 FwdCharIt mInnerIt;

public:
 using value_type        = char32_t;
 using pointer           = char32_t const *;
 using reference         = char32_t const &;
 using difference_type   = std::ptrdiff_t;
 using iterator_category = std::forward_iterator_tag;
 using Base              = FwdCharIt;

 constexpr ConstUtf8IteratorFor(FwdCharIt &&it)
   : mInnerIt {std::forward<FwdCharIt>(it)} {}
 constexpr ConstUtf8IteratorFor(FwdCharIt const &it) : mInnerIt {it} {}

 constexpr char32_t operator*() const { return decode(&*mInnerIt); }
 constexpr bool     operator==(ConstUtf8IteratorFor const &it) const {
  return mInnerIt == it.mInnerIt;
 }
 constexpr bool operator!=(ConstUtf8IteratorFor const &it) const {
  return !operator==(it);
 }
 constexpr ConstUtf8IteratorFor operator++() {
  std::advance(mInnerIt, codepointSize(&*mInnerIt));
  return *this;
 }
 constexpr ConstUtf8IteratorFor operator++(int) {
  ConstUtf8IteratorFor temp {*this};
  std::advance(mInnerIt, codepointSize(&*mInnerIt));
  return temp;
 }

 constexpr FwdCharIt       &base() { return mInnerIt; }
 constexpr FwdCharIt const &base() const { return mInnerIt; }
};

} // namespace utf8

} // namespace plush

#endif // PLUSH_BITS_UTF8_H
