// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#include "bits/Doc.h"
#include "bits/utf8.h"

namespace plush {

namespace doc {

void DocBase::display(DocStyle const &style, std::ostream &out) const {
 std::cout << toString(style) << '\n';
}

std::string DocBase::toString(DocStyle const &style) const {
 std::ostringstream oss;
 render(style, oss);
 return oss.str();
}

Char::Char(char32_t char_) : mChar {char_} {}

Char char_(char32_t char_) { return {char_}; }

void Char::render(DocStyle const &style, std::ostringstream &oss) const {
 utf8::operator<<(oss, mChar);
}

Text::Text(std::string &&text) : mText {std::move(text)} {}
Text::Text(std::string const &text) : mText {text} {}

void Text::render(DocStyle const &style, std::ostringstream &oss) const {
 oss << mText;
}

Text text(std::string &&text) { return {std::move(text)}; }
Text text(std::string const &text) { return {text}; }

Colon::Colon() {}

void Colon::render(DocStyle const &style, std::ostringstream &oss) const {
 oss << ':';
}

LParen::LParen() {}

void LParen::render(DocStyle const &style, std::ostringstream &oss) const {
 oss << '(';
}

RParen::RParen() {}

void RParen::render(DocStyle const &style, std::ostringstream &oss) const {
 oss << ')';
}

HPadding::HPadding(std::size_t count) : mCount {count} {}

void HPadding::render(DocStyle const &style, std::ostringstream &oss) const {
 for (std::size_t i = 0; i < mCount; ++i) oss << ' ';
}

HPadding hpad(std::size_t count) { return {count}; }

void Docs::render(DocStyle const &style, std::ostringstream &oss) const {
 for (auto &doc : mDocs) doc->render(style, oss);
}

} // namespace doc

void Doc::render(DocStyle const &style, std::ostringstream &oss) const {
 mDoc->render(style, oss);
}

} // namespace plush
