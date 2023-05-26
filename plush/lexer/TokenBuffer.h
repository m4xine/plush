// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_LEXER_TOKENBUFFER_H
#define PLUSH_LEXER_TOKENBUFFER_H

#include <utility>
#include <vector>

#include "basic/SourceManager.h"
#include "basic/Token.h"

namespace plush {

// Contains parsed tokens along with their originating source entity.
class TokenBuffer {
 // Origin source entity.
 SourceInfo *mSourceInfo;
 // Contains parsed tokens.
 std::vector<Token> mTokens;

public:
 using Iterator      = typename std::vector<Token>::iterator;
 using ConstIterator = typename std::vector<Token>::const_iterator;

 TokenBuffer(SourceInfo *sourceInfo, std::vector<Token> &&tokens)
   : mSourceInfo {sourceInfo}, mTokens {std::move(tokens)} {}

 constexpr SourceInfo               *sourceInfo() const { return mSourceInfo; }
 constexpr std::vector<Token>       &tokens() { return mTokens; }
 constexpr std::vector<Token> const &tokens() const { return mTokens; }

 Iterator      begin() { return mTokens.begin(); }
 ConstIterator begin() const { return mTokens.cbegin(); }
 ConstIterator cbegin() const { return mTokens.cbegin(); }
 Iterator      end() { return mTokens.end(); }
 ConstIterator end() const { return mTokens.cend(); }
 ConstIterator cend() const { return mTokens.cend(); }
};

} // namespace plush

#endif // PLUSH_LEXER_TOKENBUFFER_H
