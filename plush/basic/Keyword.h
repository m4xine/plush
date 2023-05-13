// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_BASIC_KEYWORD_H
#define PLUSH_BASIC_KEYWORD_H

#include <array>
#include <cstdint>
#include <string_view>
#include <utility>

namespace plush {

// Representation of a keyword in Plush.
class Keyword {
public:
 // Kind of keyword.
 using Kind = std::uint8_t;
 enum : Kind {
  // 'if'
  IF = 0,
  // 'do'
  DO,
  // 'let'
  LET,
  // 'module'
  MODULE,
  // 'import'
  IMPORT,
  // 'as'
  AS,
  // Total number of keyword kinds.
  _SIZE
 };

 // Total number of all keyword kinds.
 constexpr static std::size_t KIND_SIZE {static_cast<std::size_t>(_SIZE)};

private:
 // Kind of keyword.
 Kind mKind;
 // String representation of a keyword.
 std::string_view mStringRep;

 // Array of every keyword kind's string representation.
 constexpr static std::array<std::string_view, KIND_SIZE>
   KEYWORD_STRINGS_TABLE {[] {
    using namespace std::literals::string_view_literals;
    return std::array<std::string_view, KIND_SIZE> {
      // NOTE(m4xine): These __must__ remain in the same order as defined above.
      "if"sv, "do"sv, "let"sv, "module"sv, "import"sv, "as"sv};
   }()};

public:
 constexpr Keyword(Kind kind)
   : mKind {kind},
     mStringRep {KEYWORD_STRINGS_TABLE[static_cast<std::size_t>(kind)]} {}

 constexpr Kind             kind() const { return mKind; }
 constexpr std::string_view stringRep() const { return mStringRep; }
};

namespace {

template <std::size_t... Is>
constexpr std::array<Keyword, sizeof...(Is)> makeKeywords(
  std::index_sequence<Is...>) {
 return {Keyword {static_cast<Keyword::Kind>(Is)}...};
}

constexpr std::array<Keyword, Keyword::KIND_SIZE> makeKeywords() {
 return makeKeywords(std::make_index_sequence<Keyword::KIND_SIZE> {});
}

} // namespace

// Every keyword in Plush.
constexpr std::array<Keyword, Keyword::KIND_SIZE> KEYWORDS {makeKeywords()};

} // namespace plush

#endif // PLUSH_BASIC_KEYWORD_H
