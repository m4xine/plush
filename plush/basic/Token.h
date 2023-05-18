// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_BASIC_TOKEN_H
#define PLUSH_BASIC_TOKEN_H

#include <cassert>
#include <variant>

#include "basic/SourceManager.h"
#include "basic/TokenKinds.h"

namespace plush {

// Representation of a parsed token.
class Token {
 // Source origin of this parsed token.
 SourceRegionInfo *mSourceRegionInfo;
 // Storage associated with the kind of this parsed token.
 std::variant<token::Id, token::Punctuator, token::BinOp, token::String> mKind;

public:
 template <class K>
 constexpr Token(SourceRegionInfo *srcRegionInfo, K &&kind)
   : mSourceRegionInfo {srcRegionInfo}, mKind {std::forward<K>(kind)} {}

 template <class Kind>
 constexpr bool is() const {
  return std::holds_alternative<Kind>(mKind);
 }

 constexpr SourceRegionInfo *sourceRegionInfo() const {
  return mSourceRegionInfo;
 }

 template <class Kind>
 constexpr decltype(auto) get() {
  assert(is<Kind>());
  return *std::get_if<Kind>(&mKind);
 }
 template <class Kind>
 constexpr decltype(auto) get() const {
  assert(is<Kind>());
  return *std::get_if<Kind>(&mKind);
 }
};

} // namespace plush

#endif // PLUSH_BASIC_TOKEN_H
