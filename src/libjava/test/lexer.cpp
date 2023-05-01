#include <gtest/gtest.h>

#include <libjava/dump_tokens.hpp>

#include <sstream>
#include <string>

namespace java::test {

TEST(LexerSuite, ValidNumbers) {
  // Given
  std::stringstream in("0 123456789");

  // When
  std::stringstream out;
  dump_tokens(in, out);

  // Then
  EXPECT_EQ(
      out.str(),
      "Loc=<1:0>\tDECIMAL_LITERAL '0'\n"
      "Loc=<1:2>\tDECIMAL_LITERAL '123456789'\n");
}

TEST(LexerSuite, InvalidTokens) {
  // Given
  std::stringstream in("abc");

  // When
  std::stringstream out;
  dump_tokens(in, out);

  // Then
  EXPECT_EQ(
      out.str(),
      "Loc=<1:0>\tIDENTIFIER 'abc'\n");
}

}  // namespace java::test
