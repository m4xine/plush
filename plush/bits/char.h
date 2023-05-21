// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_BITS_CHAR_H
#define PLUSH_BITS_CHAR_H

namespace plush {

// Check if the provided character is a whitespace character.
constexpr bool isWhitespace(char32_t c) {
 // NOTE(m4xine): Unicode characters with property White_Space=yes.
 return (/* CHARACTER TABULATION */ 0x09 == c ||
         /* LINE FEED */ 0x0A == c ||
         /* LINE TABULATION */ 0x0B == c ||
         /* FORM FEED */ 0x0C == c ||
         /* CARRIAGE RETURN */ 0x0D == c ||
         /* SPACE */ 0x20 == c ||
         /* NEXT LINE */ 0x85 == c ||
         /* NO-BREAK SPACE */ 0xA0 == c ||
         /* OGHAM SPACE MARK */ 0x1680 == c ||
         /* EN QUAD */ 0x2000 == c ||
         /* EM QUAD */ 0x2001 == c ||
         /* EN SPACE */ 0x2002 == c ||
         /* EM SPACE */ 0x2003 == c ||
         /* THREE-PER-EM SPACE */ 0x2004 == c ||
         /* FOUR-PER-EM SPACE */ 0x2005 == c ||
         /* SIX-PER-EM SPACE */ 0x2006 == c ||
         /* FIGURE SPACE */ 0x2007 == c ||
         /* PUNCTUATION SPACE */ 0x2008 == c ||
         /* THIN SPACE */ 0x2009 == c ||
         /* HAIR SPACE */ 0x200A == c ||
         /* LINE SEPARATOR */ 0x2028 == c ||
         /* PARAGRAPH SEPARATOR */ 0x2029 == c ||
         /* NARROW NO-BREAK SPACE */ 0x202F == c ||
         /* MEDIUM MATHEMATICAL SPACE */ 0x205F == c ||
         /* IDEOGRAPHIC SPACE */ 0x3000 == c);
}

} // namespace plush

#endif // PLUSH_BITS_CHAR_H
