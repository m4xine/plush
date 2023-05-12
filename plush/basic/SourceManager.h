// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_BASIC_SOURCEMANAGER_H
#define PLUSH_BASIC_SOURCEMANAGER_H

#include <string>
#include <variant>
#include <vector>

#include "basic/FileManager.h"

namespace plush {

class SourceInfo;
class SourceManager;

// Representation of location within a source entity (file, shell, stdin)
// consisting of a line and number (starting at 0.)
struct SourceLoc {
 std::size_t line, column;

 constexpr SourceLoc(std::size_t ln, std::size_t col)
   : line {ln}, column {col} {}

 constexpr bool operator>(SourceLoc const &loc) const {
  return line > loc.line || (line == loc.line && column > loc.column);
 }
 constexpr bool operator<(SourceLoc const &loc) const {
  return !operator>(loc);
 }
 constexpr bool operator>=(SourceLoc const &loc) const {
  return operator>(loc) || operator==(loc);
 }
 constexpr bool operator<=(SourceLoc const &loc) const {
  return operator<(loc) || operator==(loc);
 }
 constexpr bool operator==(SourceLoc const &loc) const {
  return line == loc.line && column == loc.column;
 }
 constexpr bool operator!=(SourceLoc const &loc) const {
  return !operator==(loc);
 }
};

// Representation of a spanned region consisting of a beginning and ending
// location within a source entity (file, shell input, stdin.)
class SourceRegion {
 SourceLoc mBeginLoc, mEndLoc;

public:
 SourceRegion(SourceLoc &&beginLoc, SourceLoc &&endLoc);
 SourceRegion(SourceLoc const &beginLoc, SourceLoc const &endLoc);

 constexpr SourceLoc const &beginLoc() const { return mBeginLoc; }
 constexpr SourceLoc const &endLoc() const { return mEndLoc; }
};

// Extends SourceRegion with a pointer to the origin SourceInfo.
class SourceRegionInfo final : public SourceRegion {
 friend class SourceInfo;
 friend class SourceManager;

 // Pointer to the parent SourceInfo.
 SourceInfo *mSourceInfoPtr;

 SourceRegionInfo(SourceRegion &&srcRegion, SourceInfo *sourceInfo);
 SourceRegionInfo(SourceRegion const &srcRegion, SourceInfo *sourceInfo);

public:
 constexpr SourceInfo *sourceInfo() const { return mSourceInfoPtr; }
};

// Representation of a source entity (file, shell input, stdin) capable of
// being parsed and referred to later for tracking within diagnostics.
class SourceInfo final {
 friend class SourceManager;

 struct File {
  FileInfo *fileInfo;
 };

 struct Shell {
  std::string input;
 };

 struct StdIn {
  std::string input;
 };

 std::variant<File, Shell, StdIn> mVariant;
 // Reference to the parent SourceManager.
 SourceManager &mSourceManagerRef;

 constexpr SourceInfo(File &&file, SourceManager &srcMgrRef)
   : mVariant {std::move(file)}, mSourceManagerRef {srcMgrRef} {}
 constexpr SourceInfo(Shell &&shell, SourceManager &srcMgrRef)
   : mVariant {std::move(shell)}, mSourceManagerRef {srcMgrRef} {}
 constexpr SourceInfo(StdIn &&stdIn, SourceManager &srcMgrRef)
   : mVariant {std::move(stdIn)}, mSourceManagerRef {srcMgrRef} {}

public:
 std::string const &sourceContent() const;

 // Create a SourceRegionInfo (managed by the parent SourceManager) with this
 // SourceInfo and the provided SourceRegion.
 [[nodiscard]] SourceRegionInfo *makeSourceRegionInfo(SourceRegion &&srcRegion);
 // Create a SourceRegionInfo (managed by the parent SourceManager) with this
 // SourceInfo and the provided SourceRegion.
 [[nodiscard]] SourceRegionInfo *makeSourceRegionInfo(
   SourceRegion const &srcRegion);
};

// Manages source entities and associated tracking information.
class SourceManager final {
 friend class SourceInfo;

 // Contains each source entity, individually allocated to avoid pointer
 // invalidation.
 std::vector<SourceInfo *> mSources;
 // Contains each source region associated with a stored source entity,
 // individually allocated to avoid pointer invalidation.
 std::vector<SourceRegionInfo *> mSourceRegions;

 // Add and allocate a new SourceInfo and store it. Returns a pointer
 // to the new allocation. Not to be used externally.
 [[nodiscard]] SourceInfo *addSourceInfo(SourceInfo &&srcInfo);
 // Add and allocate a new SourceRegionInfo and store it. Returns a pointer to
 // the new allocation. Not to be used externally.
 [[nodiscard]] SourceRegionInfo *addSourceRegionInfo(
   SourceRegionInfo &&srcRegioninfo);

 // Allocates and stores a new SourceRegionInfo from the provided SourceRegion
 // and SourceInfo. Returns a pointer to the newly allocated SourceRegionInfo.
 // To be used by SourceInfo.
 [[nodiscard]] SourceRegionInfo *makeSourceRegionInfo(SourceRegion &&srcRegion,
                                                      SourceInfo    *srcInfo);
 // Allocates and stores a new SourceRegionInfo from the provided SourceRegion
 // and SourceInfo. Returns a pointer to the newly allocated SourceRegionInfo.
 // To be used by SourceInfo.
 [[nodiscard]] SourceRegionInfo *makeSourceRegionInfo(
   SourceRegion const &srcRegion, SourceInfo *srcInfo);

public:
 SourceManager();
 // Forbid copying and/or moving to avoid invalidating SourceInfo,
 // SourceRegionInfo, and SourceManager pointers/references when
 // moving/destructing.
 SourceManager(SourceManager &&)                 = delete;
 SourceManager(SourceManager const &)            = delete;
 SourceManager &operator=(SourceManager &&)      = delete;
 SourceManager &operator=(SourceManager const &) = delete;
 ~SourceManager();

 // Add a file as a source entity.
 [[nodiscard]] SourceInfo *addFile(FileInfo *fileInfo);
 // Add shell input as a source entity.
 [[nodiscard]] SourceInfo *addShellInput(std::string &&input);
 // Add shell input as a source entity.
 [[nodiscard]] SourceInfo *addShellInput(std::string const &input);
 // Add stdin input as a source entity.
 [[nodiscard]] SourceInfo *addStdInInput(std::string &&input);
 // Add stdin input as a source entity.
 [[nodiscard]] SourceInfo *addStdInInput(std::string const &input);
};

} // namespace plush

#endif // PLUSH_BASIC_SOURCEMANAGER_H
