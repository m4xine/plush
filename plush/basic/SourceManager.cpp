// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#include <cassert>

#include "basic/SourceManager.h"

namespace plush {

SourceRegion::SourceRegion(SourceLoc &&beginLoc, SourceLoc &&endLoc)
  : mBeginLoc {std::move(beginLoc)}, mEndLoc {std::move(endLoc)} {
 assert(mBeginLoc <= mEndLoc && "mBeginLoc > mEndLoc");
}

SourceRegion::SourceRegion(SourceLoc const &beginLoc, SourceLoc const &endLoc)
  : mBeginLoc {beginLoc}, mEndLoc {endLoc} {
 assert(mBeginLoc <= mEndLoc && "mBeginLoc > mEndLoc");
}

SourceRegionInfo::SourceRegionInfo(SourceRegion &&srcRegion,
                                   SourceInfo    *sourceInfo)
  : SourceRegion {std::move(srcRegion)}, mSourceInfoPtr {sourceInfo} {}

SourceRegionInfo::SourceRegionInfo(SourceRegion const &srcRegion,
                                   SourceInfo         *sourceInfo)
  : SourceRegion {srcRegion}, mSourceInfoPtr {sourceInfo} {}

std::string_view SourceInfo::sourceContent() const {
 if (auto file = std::get_if<File>(&mKind))
  return file->fileInfo->fileContent();
 else if (auto shell = std::get_if<Shell>(&mKind))
  return shell->input;
 else if (auto stdIn = std::get_if<StdIn>(&mKind))
  return stdIn->input;

 assert(!"Unhandled variant");
}

[[nodiscard]] SourceRegionInfo *SourceInfo::makeSourceRegionInfo(
  SourceRegion &&srcRegion) {
 return mSourceManagerRef.makeSourceRegionInfo(std::move(srcRegion), this);
}

[[nodiscard]] SourceRegionInfo *SourceInfo::makeSourceRegionInfo(
  SourceRegion const &srcRegion) {
 return mSourceManagerRef.makeSourceRegionInfo(srcRegion, this);
}

SourceInfo::ConstIterator SourceInfo::begin() const {
 return sourceContent().cbegin();
}
SourceInfo::ConstIterator SourceInfo::cbegin() const {
 return sourceContent().cbegin();
}
SourceInfo::ConstIterator SourceInfo::end() const {
 return sourceContent().cend();
}
SourceInfo::ConstIterator SourceInfo::cend() const {
 return sourceContent().cend();
}

[[nodiscard]] SourceInfo *SourceManager::addSourceInfo(SourceInfo &&srcInfo) {
 SourceInfo *newSrcInfo {new SourceInfo {std::move(srcInfo)}};
 mSources.push_back(newSrcInfo);
 return newSrcInfo;
}

[[nodiscard]] SourceRegionInfo *SourceManager::addSourceRegionInfo(
  SourceRegionInfo &&srcRegionInfo) {
 SourceRegionInfo *newSrcRegionInfo {
   new SourceRegionInfo {std::move(srcRegionInfo)}};
 mSourceRegions.push_back(newSrcRegionInfo);
 return newSrcRegionInfo;
}

[[nodiscard]] SourceRegionInfo *SourceManager::makeSourceRegionInfo(
  SourceRegion &&srcRegion, SourceInfo *srcInfo) {
 return addSourceRegionInfo({std::move(srcRegion), srcInfo});
}
[[nodiscard]] SourceRegionInfo *SourceManager::makeSourceRegionInfo(
  SourceRegion const &srcRegion, SourceInfo *srcInfo) {
 return addSourceRegionInfo({srcRegion, srcInfo});
}

SourceManager::SourceManager() {}

SourceManager::~SourceManager() {
 for (auto srcInfo : mSources) delete srcInfo;
 for (auto srcRegionInfo : mSourceRegions) delete srcRegionInfo;
}

[[nodiscard]] SourceInfo *SourceManager::addFile(FileInfo *fileInfo) {
 return addSourceInfo({SourceInfo::File {fileInfo}, *this});
}
[[nodiscard]] SourceInfo *SourceManager::addShellInput(std::string &&input) {
 return addSourceInfo({SourceInfo::Shell {std::move(input)}, *this});
}
[[nodiscard]] SourceInfo *SourceManager::addShellInput(
  std::string const &input) {
 return addSourceInfo({SourceInfo::Shell {input}, *this});
}
[[nodiscard]] SourceInfo *SourceManager::addStdInInput(std::string &&input) {
 return addSourceInfo({SourceInfo::StdIn {std::move(input)}, *this});
}
[[nodiscard]] SourceInfo *SourceManager::addStdInInput(
  std::string const &input) {
 return addSourceInfo({SourceInfo::StdIn {input}, *this});
}

} // namespace plush
