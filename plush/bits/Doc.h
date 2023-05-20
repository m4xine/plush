// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

// Provides a basic markup DSL with configurable styling for displaying readable
// user-friendly information.

#pragma once

#ifndef PLUSH_BITS_DOC_H
#define PLUSH_BITS_DOC_H

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace plush {

class Doc;
class DocStyle;

namespace doc {

// Base class for all document types.
class DocBase {
 friend class ::plush::Doc;

public:
 virtual ~DocBase() {}

 // Renders the document to an output string stream.
 virtual void render(DocStyle const &style, std::ostringstream &oss) const = 0;
 // Displays a document to the provided output stream.
 void display(DocStyle const &style, std::ostream &out = std::cout) const;

 // Renders the document to a string.
 std::string toString(DocStyle const &style) const;
};

// Text document.
class Text : public DocBase {
 std::string mText;

public:
 Text(std::string &&text);
 Text(std::string const &text);

 virtual void render(DocStyle const     &style,
                     std::ostringstream &oss) const override;
};

Text text(std::string &&text);
Text text(std::string const &text);

// Colon document.
class Colon : public DocBase {
public:
 Colon();

 virtual void render(DocStyle const     &style,
                     std::ostringstream &oss) const override;
};

inline Colon const colon {};

// Left parenthesis document.
class LParen : public DocBase {
public:
 LParen();

 virtual void render(DocStyle const     &style,
                     std::ostringstream &oss) const override;
};

inline LParen const lparen {};

// Right parenthesis document.
class RParen : public DocBase {
public:
 RParen();

 virtual void render(DocStyle const     &style,
                     std::ostringstream &oss) const override;
};

inline RParen const rparen {};

// Horizontal padding document.
class HPadding : public DocBase {
 std::size_t mCount;

public:
 HPadding(std::size_t count = 1);

 virtual void render(DocStyle const     &style,
                     std::ostringstream &oss) const override;
};

HPadding hpad(std::size_t count = 1);

// Collection of documents.
class Docs : public DocBase {
 std::vector<std::shared_ptr<DocBase>> mDocs;

public:
 template <class... Ds,
           typename = std::enable_if_t<
             true && (std::is_base_of_v<DocBase, std::decay_t<Ds>> && ...)>>
 Docs(Ds &&...docs)
   : mDocs {std::make_shared<std::decay_t<Ds>>(std::forward<Ds>(docs))...} {}

 template <class D, typename = std::enable_if_t<
                      std::is_base_of_v<DocBase, std::decay_t<D>>>>
 Docs &append(D &&doc) {
  mDocs.push_back(std::make_shared<std::decay_t<D>>(std::forward<D>(doc)));
  return *this;
 }

 template <class D>
 Docs &operator+=(D &&doc) {
  return append(std::forward<D>(doc));
 }

 virtual void render(DocStyle const     &style,
                     std::ostringstream &oss) const override;
};

template <class... Ds>
Docs docs(Ds &&...ds) {
 return {std::forward<Ds>(ds)...};
}

template <class D1, class D2>
Docs operator+(D1 &&d1, D2 &&d2) {
 return {std::forward<D1>(d1), std::forward<D2>(d2)};
}

} // namespace doc

// Configurable styling for documents when displayed/rendered.
class DocStyle {
public:
};

constexpr inline DocStyle defaultDocStyle {};

// A user-friendly renderable document. Acts as a wrapper type for any
// derivative of doc::DocBase.
class Doc final : public doc::DocBase {
 std::shared_ptr<doc::DocBase> mDoc;

public:
 template <class Derived, typename = std::enable_if_t<std::is_base_of_v<
                            doc::DocBase, std::decay_t<Derived>>>>
 Doc(Derived &&derived)
   : mDoc {std::make_shared<std::decay_t<Derived>>(
       std::forward<Derived>(derived))} {}

 void render(DocStyle const &style, std::ostringstream &oss) const override;
};

} // namespace plush

#endif // PLUSH_BITS_DOC_H
