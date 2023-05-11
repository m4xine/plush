// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_BASIC_FILEMANAGER_H
#define PLUSH_BASIC_FILEMANAGER_H

#include <filesystem>
#include <string>
#include <vector>

#include "bits/Expect.h"

namespace plush {

class FileManager;

// Representation of a file.
class FileInfo final {
 friend class FileManager;

 std::filesystem::path mFilePath;
 std::string           mFileContent;
 // Reference to the parent FileManager.
 [[maybe_unused]] FileManager &mFileManagerRef;

 FileInfo(std::filesystem::path &&filePath, std::string &&fileContent,
          FileManager &fileManagerRef);

public:
 constexpr std::filesystem::path const &filePath() const { return mFilePath; }
 constexpr std::string const &fileContent() const { return mFileContent; }
};

// Manages reading files from the filesystem relative to the execution path or
// from added lookup directories.
class FileManager final {
 // Directories to look through when reading files.
 std::vector<std::filesystem::path> mLookupDirs;
 // Every opened file.
 std::vector<FileInfo *> mFiles;

public:
 FileManager();
 FileManager(FileManager &&);
 // Forbid copying and/or moving to avoid invalidating FileInfo pointers when
 // destructing.
 FileManager(FileManager const &)            = delete;
 FileManager &operator=(FileManager &&)      = delete;
 FileManager &operator=(FileManager const &) = delete;
 ~FileManager();

 // Add a directory to look through when reading a file.
 void addLookupDir(std::filesystem::path const &dirPath);
 // Attempts to read a file at the provided path or with one of the added lookup
 // directories.
 [[nodiscard]] Expect<FileInfo *> readFile(
   std::filesystem::path const &inFilePath);
};

} // namespace plush

#endif // PLUSH_BASIC_FILEMANAGER_H
