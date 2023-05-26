// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_LEXER_LEXERDIAGNOSTIC_H
#define PLUSH_LEXER_LEXERDIAGNOSTIC_H

#include <string>
#include <variant>

#include "basic/DiagnosticsManager.h"

namespace plush {

// Diagnostic specific to the lexer phase.
class LexerDiagnostic final : public Diagnostic {
public:
 // Unexpected character encountered.
 struct UnexpectedChar {
  char32_t char_;
 };

 // Unexpected end of input encountered.
 struct UnexpectedEndOfInput {};

private:
 SourceInfo                                        *mSourceInfo;
 SourceRegion                                       mSourceRegion;
 std::variant<UnexpectedChar, UnexpectedEndOfInput> mKind;

public:
 template <class K>
 LexerDiagnostic(SourceInfo *sourceInfo, SourceRegion srcRegion, K &&kind)
   : mSourceInfo {sourceInfo}, mSourceRegion {srcRegion},
     mKind {std::forward<K>(kind)} {}

 Level                       level() const override;
 SourceInfo                 *sourceInfo() const override;
 std::optional<SourceRegion> sourceRegion() const override;
 Doc                         doc() const override;
};

} // namespace plush

#endif // PLUSH_LEXER_LEXERDIAGNOSTIC_H
