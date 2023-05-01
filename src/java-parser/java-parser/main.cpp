#include <libjava/dump_tokens.hpp>

#include <cxxopts.hpp>

#include <fstream>
#include <iostream>
#include <string>

const char* const file_path_opt = "file-path";
const char* const dump_tokens_opt = "dump-tokens";

int main(int argc, char** argv) {
  cxxopts::Options options("java-parser", "ANTLR4 java parser example");

  options.positional_help("<file-path>");

  try {
    // clang-format off
    options.add_options()
    (file_path_opt, "", cxxopts::value<std::string>())
    (dump_tokens_opt, "")
    ("h,help", "Print help");
    // clang-format on

    options.parse_positional({file_path_opt});

    const auto result = options.parse(argc, argv);

    if (result.count("help") > 0 || result.count(file_path_opt) != 1) {
      std::cout << options.help() << "\n";
      return 0;
    }

    std::ifstream input_stream(result[file_path_opt].as<std::string>());

    if (!input_stream.good()) {
      std::cerr << "Unable to read stream\n";
      return 1;
    }

    if (result.count(dump_tokens_opt) > 0) {
      java::dump_tokens(input_stream, std::cout);
    }
    } catch (const std::exception &e) {
      std::cerr << e.what() << "\n";
      return 1;
    }
}
