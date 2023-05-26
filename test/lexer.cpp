#include "driver/interpret.h"

using namespace plush;

int main(int argc, char **argv) {
 return !driver::interpret({
   /* debugEnabled */ true,
   /* filePaths */ {"test/plush_sources/lexer.psh"},
 });
}
