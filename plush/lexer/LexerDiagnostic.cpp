// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#include "bits/utf8.h"
#include "lexer/LexerDiagnostic.h"

namespace plush {

Diagnostic::Level LexerDiagnostic::level() const {
 if (std::holds_alternative<UnexpectedChar>(mKind) ||
     std::holds_alternative<UnexpectedEndOfInput>(mKind))
  return Level::ERROR;
 else
  assert(!"Unhandled kind");
}

SourceInfo *LexerDiagnostic::sourceInfo() const { return mSourceInfo; }

std::optional<SourceRegion> LexerDiagnostic::sourceRegion() const {
 return mSourceRegion;
}

Doc LexerDiagnostic::doc() const {
 using namespace doc;

 if (UnexpectedChar const *e = std::get_if<UnexpectedChar>(&mKind))
  return text("Unexpected character") + colon + hpad() + char_(e->char_);
 else if (std::get_if<UnexpectedEndOfInput>(&mKind))
  return text("Unexpected end of input");
 else
  assert(!"Unhandled kind");
}

} // namespace plush
