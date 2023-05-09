#include <gtest/gtest.h>

#include <libcs/dump_tokens.hpp>

#include <sstream>
#include <string>

namespace csharp::test {

TEST(LexerSuite, ValidNumbers) {
  // Given
  std::stringstream in("0 123456789");

  // When
  std::stringstream out;
  dump_tokens(in, out);

  // Then
  EXPECT_EQ(
      out.str(),
      "Loc=<1:0>\tNUMBER '0'\nLoc=<1:2>\tNUMBER '123456789'\n");
}

TEST(LexerSuite, StringTokens) {
  // Given
  std::stringstream in("abc");

  // When
  std::stringstream out;
  dump_tokens(in, out);

  // Then
  EXPECT_EQ(
      out.str(),
      "Loc=<1:0>\tID 'abc'\n");
}

}  // namespace csharp::test
