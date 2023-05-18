// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_BASIC_TOKENKINDS_H
#define PLUSH_BASIC_TOKENKINDS_H

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>

#include "basic/IdTable.fwd.h"

namespace plush::token {

// Representation of a binary operator in Plush.
class BinOp final {
public:
 // Kind of binary operator.
 enum Kind : std::uint8_t {
#define PLUSH_BINOP(KIND, ...) KIND,
#include "basic/TokenKinds.def"
  // Total number of every binary operator.
  _SIZE
 };

 // Total number of every binary operator.
 constexpr static std::size_t KIND_SIZE {_SIZE};

 // Associativity of a binary operator.
 enum Assoc : std::uint8_t {
  // Left associativity.
  LASSOC,
  // Right associativity.
  RASSOC
 };

private:
 // Kind of binary operator.
 Kind mKind;
 // Associativity of a binary operator.
 Assoc mAssoc;
 // Precedence of a binary operator.
 std::uint8_t mPrec;
 // String representation of a binary operator.
 std::string_view mStringRep;

 // Table of every binary operator's associativity.
 constexpr static std::array<Assoc, KIND_SIZE> BINOP_ASSOC_TABLE {
#define PLUSH_BINOP(KIND, STRINGREP, ASSOC, ...) ASSOC,
#include "basic/TokenKinds.def"
 };

 // Table of every binary operator's precedence.
 constexpr static std::array<std::uint8_t, KIND_SIZE> BINOP_PREC_TABLE {
#define PLUSH_BINOP(KIND, STRINGREP, ASSOC, PREC, ...) PREC,
#include "basic/TokenKinds.def"
 };

 // Table of every binary operator's string representation.
 constexpr static std::array<std::string_view, KIND_SIZE>
   BINOP_STRINGREP_TABLE {[] {
    using namespace std::literals::string_view_literals;
    return std::array<std::string_view, KIND_SIZE> {
#define PLUSH_BINOP(KIND, STRINGREP, ...) STRINGREP##sv,
#include "basic/TokenKinds.def"
    };
   }()};

 template <std::underlying_type_t<Kind>... Ks>
 constexpr static std::array<BinOp, KIND_SIZE> makeAll(
   std::integer_sequence<std::underlying_type_t<Kind>, Ks...>) {
  static_assert(sizeof...(Ks) == KIND_SIZE && ((Ks < _SIZE) && ...));
  return {BinOp {static_cast<Kind>(Ks)}...};
 }

public:
 // Constructs every binary operator in Plush.
 constexpr static std::array<BinOp, KIND_SIZE> makeAll() {
  return makeAll(
    std::make_integer_sequence<std::underlying_type_t<Kind>, _SIZE> {});
 }

 constexpr BinOp(enum Kind kind)
   : mKind {kind}, mAssoc {BINOP_ASSOC_TABLE[kind]},
     mPrec {BINOP_PREC_TABLE[kind]}, mStringRep {BINOP_STRINGREP_TABLE[kind]} {}

 constexpr Kind             kind() const { return mKind; }
 constexpr Assoc            assoc() const { return mAssoc; }
 constexpr std::uint8_t     prec() const { return mPrec; }
 constexpr std::string_view stringRep() const { return mStringRep; }
};

// Every binary operator in Plush.
constexpr static std::array<BinOp, BinOp::KIND_SIZE> BINOPS {BinOp::makeAll()};

// Representation of a punctuator in Plush.
class Punctuator final {
public:
 enum Kind : std::uint8_t {
#define PLUSH_PUNCTUATOR(KIND, ...) KIND,
#include "basic/TokenKinds.def"
  // Total number of every punctuator
  _SIZE
 };

 // Total number of every punctuator.
 constexpr static std::size_t KIND_SIZE {_SIZE};

private:
 // Kind of punctuator.
 Kind mKind;
 // String representation of a punctuator.
 std::string_view mStringRep;

 // Table of every punctuator's string representation.
 constexpr static std::array<std::string_view, KIND_SIZE>
   PUNCTUATOR_STRINGREP_TABLE {[] {
    using namespace std::literals::string_view_literals;
    return std::array<std::string_view, KIND_SIZE> {
#define PLUSH_PUNCTUATOR(KIND, STRINGREP, ...) STRINGREP##sv,
#include "basic/TokenKinds.def"
    };
   }()};

 template <std::underlying_type_t<Kind>... Ks>
 constexpr static std::array<Punctuator, KIND_SIZE> makeAll(
   std::integer_sequence<std::underlying_type_t<Kind>, Ks...>) {
  static_assert(sizeof...(Ks) == KIND_SIZE && ((Ks < _SIZE) && ...));
  return {Punctuator {static_cast<Kind>(Ks)}...};
 }

public:
 // Constructs every punctuator in Plush.
 constexpr static std::array<Punctuator, KIND_SIZE> makeAll() {
  return makeAll(
    std::make_integer_sequence<std::underlying_type_t<Kind>, _SIZE> {});
 }

 constexpr Punctuator(enum Kind kind)
   : mKind {kind}, mStringRep {PUNCTUATOR_STRINGREP_TABLE[kind]} {}

 constexpr Kind             kind() const { return mKind; }
 constexpr std::string_view stringRep() const { return mStringRep; }
};

// Every punctuator in Plush.
constexpr static std::array<Punctuator, Punctuator::KIND_SIZE> PUNCTUATORS {
  Punctuator::makeAll()};

// Representation of a keyword in Plush.
// Only to be used to find out additional information of a keyword via its kind.
class Keyword final {
public:
 enum Kind : std::uint8_t {
#define PLUSH_KEYWORD(KIND, ...) KIND,
#include "basic/TokenKinds.def"
  // Total number of every keyword.
  _SIZE
 };

 // Total number of every keyword.
 constexpr static std::size_t KIND_SIZE {_SIZE};

private:
 // Kind of keyword.
 Kind mKind;
 // String representation of a keyword.
 std::string_view mStringRep;

 // Table of every keyword's string representation.
 constexpr static std::array<std::string_view, KIND_SIZE>
   KEYWORD_STRINGREP_TABLE {[] {
    using namespace std::literals::string_view_literals;
    return std::array<std::string_view, KIND_SIZE> {
#define PLUSH_KEYWORD(KIND, STRINGREP, ...) STRINGREP##sv,
#include "basic/TokenKinds.def"
    };
   }()};

 template <std::underlying_type_t<Kind>... Ks>
 constexpr static std::array<Keyword, KIND_SIZE> makeAll(
   std::integer_sequence<std::underlying_type_t<Kind>, Ks...>) {
  static_assert(sizeof...(Ks) == KIND_SIZE && ((Ks < _SIZE) && ...));
  return {Keyword {static_cast<Kind>(Ks)}...};
 }

public:
 // Constructs every keyword in Plush.
 constexpr static std::array<Keyword, KIND_SIZE> makeAll() {
  return makeAll(
    std::make_integer_sequence<std::underlying_type_t<Kind>, _SIZE> {});
 }

 constexpr Keyword(enum Kind kind)
   : mKind {kind}, mStringRep {KEYWORD_STRINGREP_TABLE[kind]} {}

 constexpr Kind             kind() const { return mKind; }
 constexpr std::string_view stringRep() const { return mStringRep; }
};

// Every keyword in Plush.
constexpr static std::array<Keyword, Keyword::KIND_SIZE> KEYWORDS {
  Keyword::makeAll()};

// Representation of an identifier token in Plush.
// Used in place of Keyword within tokens as additional context is required.
class Id final {
 IdInfo *mId;

public:
 constexpr Id(IdInfo *id) : mId {id} {}

 constexpr IdInfo *id() const { return mId; }

 constexpr IdInfo *operator->() const { return mId; }
 constexpr IdInfo *operator*() const { return mId; }
};

// Representation of a string literal token in Plush.
class String final {
 std::string mString;

public:
 String(std::string &&string) : mString {string} {}
 String(std::string const &string) : mString {string} {}

 constexpr std::string_view string() const { return mString; }

 constexpr std::string const &operator*() const { return mString; }
 constexpr std::string const &operator->() const { return mString; }
};

} // namespace plush::token

#endif // PLUSH_BASIC_TOKENKINDS_H
