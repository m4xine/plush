// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#include <cassert>

#include "basic/IdTable.h"
#include "bits/hash.h"

namespace plush {

IdInfo::IdInfo(std::string                &&stringRep,
               std::optional<Keyword::Kind> optKeywordKind, IdTable &idTableRef)
  : mStringRep {std::move(stringRep)}, mOptKeywordKind {optKeywordKind},
    mIdTableRef {idTableRef} {}

IdInfo *IdTable::add(IdInfo &&idInfo, std::size_t idHash) {
 IdInfo *newIdInfo {new IdInfo {std::move(idInfo)}};
 mHashes.push_back(idHash);
 mEntries.push_back(newIdInfo);
 return newIdInfo;
}

IdInfo *IdTable::add(IdInfo &&idInfo) {
 std::size_t idHash = hash::fnv1a(idInfo.stringRep());
 return add(std::move(idInfo), idHash);
}

void IdTable::addKeywords() {
 for (auto &kw : KEYWORDS)
  add({std::string {kw.stringRep()}, {kw.kind()}, *this});
}

IdTable::IdTable() { addKeywords(); }

IdTable::~IdTable() {
 for (auto entryIdInfo : mEntries) delete entryIdInfo;
}

[[nodiscard]] IdInfo *IdTable::get(std::string_view id) {
 assert(mHashes.size() == mEntries.size() &&
        "mHashes and mEntries should be the same length");

 std::size_t idHash = hash::fnv1a(id);

 // Attempt to find a hash that matches the provided string's hash.
 for (std::size_t i = 0; i < mHashes.size(); ++i) {
  std::size_t entryHash = mHashes[i];

  // If there's a match, compare the strings to avoid collisions.
  if (idHash == entryHash) {
   IdInfo *entryIdInfo = mEntries[i];
   if (id == entryIdInfo->stringRep()) return entryIdInfo;
  }
 }

 // No identifier was found, create a new one.
 return add({std::string {id}, std::nullopt, *this}, idHash);
}

} // namespace plush
