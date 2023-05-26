// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_LEXER_LEXLET_H
#define PLUSH_LEXER_LEXLET_H

#include <cstdint>
#include <utility>
#include <variant>

#include "basic/Token.h"
#include "lexer/LexState.h"
#include "lexer/LexerDiagnostic.h"

namespace plush {

// Wrapper for a lexlet function and its corresponding label, providing error
// context.
template <class Output>
class BasicLexlet final {
public:
 class Result final {
 public:
  // Lexlet couldn't parse anything, did nothing.
  struct Nothing {
   constexpr explicit Nothing() {}
  };

  // Lexlet parsed without an output.
  struct Parsed {
   constexpr explicit Parsed() {}
  };

  // Lexlet parsed with an output.
  struct ParsedWithOutput {
   Output value;
  };

 private:
  std::variant<Nothing, Parsed, ParsedWithOutput, LexerDiagnostic> mKind;

 public:
  Result() : mKind {Nothing {}} {}
  Result(Parsed) : mKind {Parsed {}} {}
  Result(Output &&output)
    : mKind {ParsedWithOutput {std::forward<Output>(output)}} {}
  Result(LexerDiagnostic &&diagnostic) : mKind {std::move(diagnostic)} {}

  template <class Kind>
  Kind *getIf() {
   return std::get_if<Kind>(&mKind);
  }
 };

 static inline Result nothing {};
 static Result        parsed() { return {typename Result::Parsed {}}; }
 static Result        parsed(Output &&output) {
  return {std::forward<Output>(output)};
 }
 static Result diagnostic(LexerDiagnostic &&diag) { return {std::move(diag)}; }

 // Lexlet function type.
 using Fn = Result (*)(LexState &);

private:
 // Lexlet label.
 std::string_view mLabel;
 // Lexlet function.
 Fn mFn;

public:
 constexpr BasicLexlet(std::string_view label, Fn fn)
   : mLabel {label}, mFn {fn} {}

 constexpr std::string_view label() const { return mLabel; }
 Result operator()(LexState &state) const { return mFn(state); }
};

using Lexlet = BasicLexlet<Token>;

} // namespace plush

#endif // PLUSH_LEXER_LEXLET_H
