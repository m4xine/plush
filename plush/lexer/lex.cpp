// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#include "bits/char.h"
#include "lexer/LexState.h"
#include "lexer/LexerDiagnostic.h"
#include "lexer/Lexlet.h"
#include "lexer/lex.h"

namespace plush {

constexpr static Lexlet lexWhitespace {
  "whitespace", [](LexState &s) -> Lexlet::Result {
   SourceLoc beginLoc {s.loc()};
   SourceLoc endLoc {[&] {
    while (s && isWhitespace(*s)) ++s;
    return s.loc();
   }()};

   return ((beginLoc == endLoc) ? Lexlet::nothing : Lexlet::parsed());
  }};

constexpr static Lexlet lexComment {
  "comment", [](LexState &s) -> Lexlet::Result {
#define PLUSH_LINECOMMENT '#'
   SourceLoc beginLoc {s.loc()};
   SourceLoc endLoc {[&] {
    if (PLUSH_LINECOMMENT == *s)
     while (s && '\n' != *s) ++s;
    return s.loc();
   }()};

   return ((beginLoc == endLoc) ? Lexlet::nothing : Lexlet::parsed());
#undef PLUSH_LINECOMMENT
  }};

constexpr static Lexlet lexId {
  "identifier", [](LexState &s) -> Lexlet::Result {
#define PLUSH_IDHEAD(x) \
 (((x) >= 'a' && (x) <= 'z') || ((x) >= 'A' && (x) <= 'Z') || (x) == '_')
#define PLUSH_IDTAIL(x) (PLUSH_IDHEAD(x) || ((x) >= '0' && (x) <= '9'))
   if (PLUSH_IDHEAD(*s)) {
    typename LexState::ConstIterator::Base beginIt {s.it().base()};
    SourceLoc                              beginLoc {s.loc()};
    typename LexState::ConstIterator::Base endIt {[&] {
     ++s; // Skip over identifier head.
     while (s && PLUSH_IDTAIL(*s)) ++s;
     return s.it().base();
    }()};
    SourceLoc                              endLoc {s.loc()};
    SourceRegionInfo                      *srcRegionInfo {
      s.sourceInfo()->makeSourceRegionInfo({beginLoc, endLoc})};
    IdInfo *idInfo {s.idTable().get(
      {&*beginIt, static_cast<std::size_t>(std::distance(beginIt, endIt))})};

    return Lexlet::parsed({srcRegionInfo, token::Id {idInfo}});
   }

   return Lexlet::nothing;
#undef PLUSH_IDHEAD
#undef PLUSH_IDTAIL
  }};

constexpr static Lexlet lexPunctuator {
  "punctuator", [](LexState &s) -> Lexlet::Result {
   std::string_view src {
     &*s.it().base(),
     static_cast<std::size_t>(std::distance(s.it().base(), s.end().base()))};

   for (token::Punctuator const &punct : token::PUNCTUATORS) {
    std::string_view stringRep {punct.stringRep()};
    if (0 == src.compare(0, stringRep.size(), stringRep)) {
     SourceLoc         beginLoc {s.loc()};
     SourceLoc         endLoc {[&] {
      s += stringRep.size(); // Skip over punctuator.
      return s.loc();
     }()};
     SourceRegionInfo *srcRegionInfo {
       s.sourceInfo()->makeSourceRegionInfo({beginLoc, endLoc})};

     return Lexlet::parsed({srcRegionInfo, punct});
    }
   }

   return Lexlet::nothing;
  }};

constexpr static Lexlet lexBinOp {
  "binop", [](LexState &s) -> Lexlet::Result {
   std::string_view src {
     &*s.it().base(),
     static_cast<std::size_t>(std::distance(s.it().base(), s.end().base()))};

   for (auto &binOp : token::BINOPS) {
    std::string_view stringRep {binOp.stringRep()};
    if (0 == src.compare(0, stringRep.size(), stringRep)) {
     SourceLoc         beginLoc {s.loc()};
     SourceLoc         endLoc {[&] {
      s += stringRep.size(); // Skip over binop.
      return s.loc();
     }()};
     SourceRegionInfo *srcRegionInfo {
       s.sourceInfo()->makeSourceRegionInfo({beginLoc, endLoc})};
     return Lexlet::parsed({srcRegionInfo, binOp});
    }
   }

   return Lexlet::nothing;
  }};

constexpr static Lexlet lexString {
  "string", [](LexState &s) -> Lexlet::Result {
   if (*s == '"') {
    SourceLoc          beginLoc {s.loc()};
    std::ostringstream oss;

    ++s; // Skip over "
    while (s && *s != '"') {
     char32_t c {*s};
     ++s; // Skip over the character.

     // If the character is a backslash, attempt to parse an escape sequence.
     if ('\\' == c) {
      if (!s) break;
      switch (*s) {
       /* Newline */ case 'n':
        oss << '\n';
        ++s;
        break;
       /* Escaped quotation mark */ case '"':
        oss << '"';
        ++s;
        break;
       /* Escaped backslash */ case '\\':
        oss << '\\';
        ++s;
        break;
      }
     } else
      utf8::operator<<(oss, c);
    }

    if (!s)
     // End of input, no matching quotation mark was found.
     return Lexlet::diagnostic(s.makeDiagnostic(
       {beginLoc, s.loc()}, LexerDiagnostic::UnexpectedEndOfInput {}));
    else {
     // Successfully parsed the string. The loop above stops when a matching
     // quotation mark is found, skip over it.
     assert(*s == '"');
     ++s;
    }

    SourceLoc         endLoc {s.loc()};
    SourceRegionInfo *srcRegionInfo {
      s.sourceInfo()->makeSourceRegionInfo({beginLoc, endLoc})};

    return Lexlet::parsed({srcRegionInfo, token::String {oss.str()}});
   }

   return Lexlet::nothing;
  }};

static Lexlet::Result lexOnce(LexState &s) {
 constexpr static Lexlet LEXLETS[] {lexWhitespace, lexComment, lexId,
                                    lexPunctuator, lexBinOp,   lexString};

 for (Lexlet const &lexlet : LEXLETS) {
  if (!s)
   // Break if lexOnce was called at endOfInput or a lexlet from LEXLETS
   // consumed the rest.
   break;

  Lexlet::Result result {lexlet(s)};
  if (result.getIf<Lexlet::Result::Nothing>())
   continue;
  else if (result.getIf<Lexlet::Result::Parsed>())
   return Lexlet::parsed();
  else if (auto output = result.getIf<Lexlet::Result::ParsedWithOutput>())
   return Lexlet::parsed(std::move(output->value));
  else if (LexerDiagnostic *diag = result.getIf<LexerDiagnostic>())
   return Lexlet::diagnostic(std::move(*diag));
  else
   assert(!"Unhandled kind");
 }

 if (s)
  return Lexlet::diagnostic(
    s.makeDiagnosticHere(LexerDiagnostic::UnexpectedChar {*s}));
 else
  return Lexlet::diagnostic(
    s.makeDiagnosticHere(LexerDiagnostic::UnexpectedEndOfInput {}));
}

TokenBuffer lex(SourceInfo *sourceInfo, IdTable &idTable,
                DiagnosticsManager &diagMgr) {
 std::vector<Token> tokens;
 LexState           s {sourceInfo, idTable};

 while (s) {
  Lexlet::Result result {lexOnce(s)};
  if (result.getIf<Lexlet::Result::Nothing>())
   assert(!"Impossible");
  else if (result.getIf<Lexlet::Result::Parsed>())
   continue;
  else if (auto output = result.getIf<Lexlet::Result::ParsedWithOutput>())
   tokens.push_back(std::move(output->value));
  else if (LexerDiagnostic *diag = result.getIf<LexerDiagnostic>()) {
   diagMgr.add(std::move(*diag));

   if (diagMgr.errorLimitReached())
    return TokenBuffer {sourceInfo, std::move(tokens)};
   else
    // Attempt to skip over the problematic character(s).
    if (s) ++s;
  }
 }

 return TokenBuffer {sourceInfo, std::move(tokens)};
}

} // namespace plush
