// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_LEXER_LEXSTATE_H
#define PLUSH_LEXER_LEXSTATE_H

#include "basic/DiagnosticsManager.h"
#include "basic/SourceManager.h"
#include "basic/Token.h"
#include "bits/utf8.h"
#include "lexer/LexerDiagnostic.h"

namespace plush {

// Lexer state representation.
class LexState final {
public:
 using ConstIterator = utf8::ConstUtf8IteratorFor<SourceInfo::ConstIterator>;

private:
 SourceInfo *mSourceInfo;
 IdTable    &mIdTable;
 // Current source location.
 SourceLoc mLoc;
 // Current lexing position.
 ConstIterator mIt;
 // Iterator pointing to the end of the source entity being lexed.
 ConstIterator const mEndIt;

public:
 LexState(SourceInfo *sourceInfo, IdTable &idTable);

 constexpr SourceInfo *sourceInfo() const { return mSourceInfo; }
 constexpr IdTable    &idTable() const { return mIdTable; }
 constexpr bool        valid() const { return mIt != mEndIt; }
 constexpr char32_t    cur() const {
  assert(valid());
  return *mIt;
 }
 constexpr void advance() {
  switch (cur()) {
   case '\n':
    ++mLoc.line;
    mLoc.column = 0;
    break;
   default:
    ++mLoc.column;
    break;
  }
  ++mIt;
 }
 constexpr void advance(std::size_t n) {
  for (std::size_t i = 0; i < n; ++i) advance();
 }
 constexpr SourceLoc     loc() const { return mLoc; }
 constexpr ConstIterator it() const { return mIt; }
 constexpr ConstIterator end() const { return mEndIt; }

 template <class K>
 LexerDiagnostic makeDiagnostic(SourceRegion srcRegion, K &&kind) const {
  return {mSourceInfo, srcRegion, std::forward<K>(kind)};
 }

 template <class K>
 LexerDiagnostic makeDiagnosticHere(K &&kind) const {
  return makeDiagnostic({mLoc, mLoc}, std::forward<K>(kind));
 }

 constexpr operator bool() const { return valid(); }
 constexpr char32_t  operator*() const { return cur(); }
 constexpr LexState &operator++() {
  advance();
  return *this;
 }
 constexpr LexState &operator+=(std::size_t n) {
  advance(n);
  return *this;
 }
};

} // namespace plush

#endif // PLUSH_LEXER_LEXSTATE_H
