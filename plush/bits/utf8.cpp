#include "bits/utf8.h"

namespace plush {

namespace utf8 {

std::ostream &operator<<(std::ostream &stream, char32_t c) {
 char s[5] {};
 encode(s, c);
 stream << s;
 return stream;
}

} // namespace utf8

} // namespace plush
