// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#include <cassert>

#include "basic/DiagnosticsManager.h"

namespace plush {

std::string Diagnostic::levelToString(Level level) {
 switch (level) {
  case Diagnostic::NOTE:
   return "note";
  case Diagnostic::WARNING:
   return "warning";
  case Diagnostic::ERROR:
   return "error";
 }
}

void DiagnosticsManager::display() const {
 for (auto &diag : mDiagnostics) {
  Diagnostic::Level level {diag->level()};
  std::ostream     *out {nullptr};

  switch (level) {
   case Diagnostic::WARNING:
   case Diagnostic::ERROR:
    out = &std::cerr;
    break;
   default:
    out = &std::cout;
    break;
  }

  {
   using namespace doc;

   Docs d;

   d += text(Diagnostic::levelToString(level));

   if (SourceInfo *srcInfo = diag->sourceInfo(); srcInfo) {
    d += hpad() + lparen;
    if (srcInfo->is<SourceInfo::File>())
     d += text(srcInfo->get<SourceInfo::File>().fileInfo->filePath().string());
    else if (srcInfo->is<SourceInfo::Shell>())
     d += text("shell");
    else if (srcInfo->is<SourceInfo::StdIn>())
     d += text("stdin");
    else
     assert(!"Unhandled variant");
    d += rparen;
   }

   d += colon + hpad() + diag->doc();

   d.display(defaultDocStyle, *out);
  }
 }
}

DiagnosticsManager::DiagnosticsManager(std::size_t errorLimit)
  : mErrorLimit {errorLimit} {}

bool DiagnosticsManager::errorLimitReached() const {
 std::size_t count {0};
 for (auto &diag : mDiagnostics)
  if (diag->level() >= Diagnostic::ERROR) ++count;
 return count >= mErrorLimit;
}

bool DiagnosticsManager::dump() {
 bool bad {errorLimitReached()};
 display();
 mDiagnostics.clear();
 return bad;
}

} // namespace plush
