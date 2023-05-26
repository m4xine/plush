// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#include "basic/IdTable.h"
#include "basic/SourceManager.h"
#include "driver/interpret.h"
#include "lexer/lex.h"

namespace plush::driver {

Expect<> interpret(Options const &options) {
 DiagnosticsManager diagMgr;
 FileManager        fileMgr;
 FileInfo          *fileInfo {nullptr};

 for (auto &filePath : options.filePaths) {
  auto eFileInfo = fileMgr.readFile(filePath);
  if (!eFileInfo)
   // Filepath couldn't be opened.
   return eFileInfo.takeError<BasicError>();
  else if (fileInfo)
   // Driver was provided too many files.
   return BasicError {"Invalid file inputs: " + eFileInfo->filePath().string()};
  else
   fileInfo = *eFileInfo;
 }

 if (!fileInfo) {
  // TODO(m4xine): Accept stdin instead of just files.
  return BasicError {"Expected file input"};
 }

 IdTable       idTable;
 SourceManager srcMgr;
 SourceInfo   *srcInfo {srcMgr.addFile(fileInfo)};

 auto tokBuf {lex(srcInfo, idTable, diagMgr)};
 if (diagMgr.dump()) return BasicError {"Too many errors"};

 if (options.debugEnabled) {
  std::cout << "Displaying " << tokBuf.tokens().size() << " tokens:\n";

  doc::List list;
  for (auto &tok : tokBuf) {
   using namespace doc;
   list += hpad(4) + tok.doc();
  }

  DocStyle docStyle {};
  docStyle.listSeperator = "\n";
  list.display(docStyle);

  std::cout << "Displayed " << tokBuf.tokens().size() << " tokens.\n";
 }

 return unit;
}

} // namespace plush::driver
