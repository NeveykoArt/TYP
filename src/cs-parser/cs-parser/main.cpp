#include <libcs/dump_tokens.hpp>
#include <libcs/parser.hpp>

#include <antlr4-runtime.h>

#include <cxxopts.hpp>
#include <string>
#include <iostream>
#include <fstream>

const char* const file_path_opt = "file-path";
const char* const dump_tokens_opt = "dump-tokens";
const char* const dump_ast_opt = "dump-ast";

int main(int argc, char **argv) {
  cxxopts::Options options("csharp-parser", "ANTLR4 csharp parser example");

  options.positional_help("<file-path>");

  try {
    // clang-format off
    options.add_options()
        (file_path_opt, "", cxxopts::value<std::string>())
        (dump_tokens_opt, "")
        (dump_ast_opt, "")
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
      csharp::dump_tokens(input_stream, std::cout);
    } else if (result.count(dump_ast_opt) > 0) {
      std::cout<< "rfoireoipf\n";
      auto parser_result = csharp::parse(input_stream);
      if (!parser_result.errors_.empty()) {
        csharp::dump_errors(parser_result.errors_, std::cerr);
      } else {
        csharp::dump_ast(parser_result.program_, std::cout);
      }
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
