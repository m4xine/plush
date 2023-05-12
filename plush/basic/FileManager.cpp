// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#include "basic/FileManager.h"

namespace plush {

FileInfo::FileInfo(std::filesystem::path &&filePath, std::string &&fileContent,
                   FileManager &fileManagerRef)
  : mFilePath {std::move(filePath)}, mFileContent {std::move(fileContent)},
    mFileManagerRef {fileManagerRef} {}

FileManager::FileManager() {}

FileManager::~FileManager() {
 for (auto fileInfo : mFiles) delete fileInfo;
}

void FileManager::addLookupDir(std::filesystem::path const &dirPath) {
 mLookupDirs.push_back(dirPath);
}

[[nodiscard]] Expect<FileInfo *> FileManager::readFile(
  std::filesystem::path const &inFilePath) {
 std::filesystem::path filePath {inFilePath};

 // If the provided file path does not exist, attempt to find a path that does
 // exist within mLookupDirs.
 if (!std::filesystem::exists(filePath))
  for (auto &lookupDir : mLookupDirs) {
   auto fullPath = lookupDir / filePath;
   if (std::filesystem::exists(fullPath)) {
    filePath = std::move(fullPath);
    break;
   }
  }

 if (!std::filesystem::exists(filePath)) {
  // No file could be found, return an error with the attempted paths.
  std::vector<std::filesystem::path> triedPaths;
  for (auto &lookupDir : mLookupDirs)
   triedPaths.push_back(lookupDir / filePath);

  std::ostringstream oss;
  oss << "Couldn't open file, attempted paths: ";
  for (auto it = triedPaths.begin(); it != triedPaths.end(); ++it)
   oss << *it << ((it != triedPaths.end()) ? ", " : "");
  return BasicError {oss.str()};
 }

 std::ifstream ifs {filePath, std::ios::binary};
 if (!ifs)
  // The file exists but is unable to be read.
  return BasicError {std::strerror(errno)};

 // Read the file at the path and construct a new FileInfo.
 std::ostringstream oss;
 oss << ifs.rdbuf();
 FileInfo *newFile {new FileInfo {std::move(filePath), oss.str(), *this}};

 mFiles.push_back(newFile);
 return newFile;
}

} // namespace plush
