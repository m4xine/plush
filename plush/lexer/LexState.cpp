// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#include "lexer/LexState.h"

namespace plush {

LexState::LexState(SourceInfo *sourceInfo, IdTable &idTable)
  : mSourceInfo {sourceInfo}, mIdTable {idTable}, mIt {sourceInfo->cbegin()},
    mEndIt {sourceInfo->cend()} {}

} // namespace plush
