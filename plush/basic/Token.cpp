#include "basic/IdTable.h"
#include "basic/Token.h"

namespace plush {

Doc Token::doc() const {
 using namespace token;
 using namespace doc;

 if (is<Id>())
  return text("Id") + colon + hpad() +
         text(std::string {get<Id>().id()->stringRep()});
 else if (is<Punctuator>())
  return text("Punctuator") + colon + hpad() +
         text(std::string {get<Punctuator>().stringRep()});
 else if (is<BinOp>())
  return text("BinOp") + colon + hpad() +
         text(std::string {get<BinOp>().stringRep()});
 else if (is<String>())
  return text("String") + colon + hpad() +
         stringLit(std::string {get<String>().string()});

 assert(!"Unhandled kind");
}

} // namespace plush
