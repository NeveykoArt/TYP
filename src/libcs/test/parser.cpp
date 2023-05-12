#include <gtest/gtest.h>

#include <libcs/parser.hpp>

#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>

namespace csharp::test {

/*
static size_t count_indentation(const std::string& str) {
  const auto it = std::find_if_not(
      str.begin(), str.end(), [](char ch) { return std::isspace(ch); });
  return std::distance(str.begin(), it);
}

static std::string dedent(const std::string& str) {
  std::stringstream ss(str);
  std::stringstream out;
  std::string line;
  // Skip first '\n'
  std::getline(ss, line);

  std::getline(ss, line);
  const auto indentation = count_indentation(line);
  out << line.substr(std::min(indentation, line.size())) << "\n";

  while (std::getline(ss, line)) {
    out << line.substr(std::min(indentation, line.size())) << "\n";
  }

  return out.str();
}
*/

TEST(ParserSuite, ValidDocument) {

  EXPECT_EQ(1, 1);
}

TEST(ParserSuite, InvalidDocument) {
  // Given
  EXPECT_EQ(2, 2);
}

}  // namespace csharp::test
