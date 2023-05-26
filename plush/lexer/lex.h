// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_LEXER_LEX_H
#define PLUSH_LEXER_LEX_H

#include "basic/DiagnosticsManager.h"
#include "basic/IdTable.h"
#include "basic/Token.h"
#include "bits/Expect.h"
#include "lexer/TokenBuffer.h"

namespace plush {

// Performs lexical analysis on the provided source entity. Encountered errors
// are handled by the diagnostic manager.
TokenBuffer lex(SourceInfo *sourceInfo, IdTable &idTable,
                DiagnosticsManager &diagMgr);

} // namespace plush

#endif // PLUSH_LEXER_LEX_H
