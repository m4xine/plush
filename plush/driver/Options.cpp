#include "driver/Options.h"

namespace plush::driver {

Expect<Options> Options::parseArgs(int argc, char **argv) {
 Options opt;

 for (int i = 1; i < argc; ++i) {
  std::string_view arg {argv[i]};
  if (arg == "--debug")
   opt.debugEnabled = true;
  else
   opt.filePaths.push_back(arg);
 }

 return opt;
}

} // namespace plush::driver
