// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_BASIC_IDTABLE_H
#define PLUSH_BASIC_IDTABLE_H

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "basic/Keyword.h"

namespace plush {

class IdTable;

// Representation of a Plush identifier.
class IdInfo final {
 friend class IdTable;

 // String representation of the identifier.
 std::string mStringRep;
 // Is the identifier a keyword?
 std::optional<Keyword::Kind> mOptKeywordKind;
 // Reference to the parent IdTable.
 [[maybe_unused]] IdTable &mIdTableRef;

 IdInfo(std::string &&stringRep, std::optional<Keyword::Kind> optKeywordKind,
        IdTable &idTableRef);

public:
 constexpr std::string_view stringRep() const { return mStringRep; }
 // Retrieves the identifier's keyword kind. Fails if the identifier is not a
 // keyword.
 constexpr Keyword::Kind keywordKind() const { return *mOptKeywordKind; }

 constexpr bool isKeyword() const { return mOptKeywordKind; }
 constexpr bool isKeyword(Keyword::Kind kind) const {
  return isKeyword() && keywordKind() == kind;
 }
};

// A string interner with additional information associated with identifiers to
// easily allow comparisons between other identifiers and keywords.
class IdTable final {
 // NOTE(m4xine): CXX17 doesn't allow heterogenous lookup for unordered
 // containers (std::unordered_map), hence the rudimentary string map
 // implementation.

 // Every hash of each IdInfo entry, with the indices matching the associated
 // entry within mEntries.
 std::vector<std::size_t> mHashes;
 // Every identifier, individually allocated to avoid pointer invalidation.
 std::vector<IdInfo *> mEntries;

 // Allocate and add a new IdInfo entry and its associated hash. Returns a
 // pointer to the newly allocated IdInfo.
 IdInfo *add(IdInfo &&idInfo, std::size_t hash);
 // Allocate and add a new IdInfo entry and its associated hash. Returns a
 // pointer to the newly allocated IdInfo.
 IdInfo *add(IdInfo &&idInfo);

 // Add every Plush keyword.
 void addKeywords();

public:
 // Construct an IdTable with every Plush keyword.
 IdTable();
 // Forbid copying and/or moving to avoid invalidating IdInfo and IdTable
 // pointers/references when moving/destructing.
 IdTable(IdTable &&)                 = delete;
 IdTable(IdTable const &)            = delete;
 IdTable &operator=(IdTable &&)      = delete;
 IdTable &operator=(IdTable const &) = delete;
 ~IdTable();

 // Lookup an identifier with the provided string. If no identifier exists, a
 // new one will be created.
 [[nodiscard]] IdInfo *get(std::string_view id);
};

} // namespace plush

#endif // PLUSH_BASIC_IDTABLE_H
