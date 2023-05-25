#include <libcs/dump_tokens.hpp>
#include <libcs/parser.hpp>

#include <antlr4-runtime.h>
#include <cxxopts.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

const char* const file_path_opt = "file-path";
const char* const dump_tokens_opt = "dump-tokens";
const char* const dump_ast_opt = "dump-ast";
const char* const dump_table_opt = "dump-table";
const char* const dump_asm_opt = "dump-asm";

int main(int argc, char** argv) {
  cxxopts::Options options("csharp-parser", "ANTLR4 csharp parser example");

  options.positional_help("<file-path>");

  try {
    // clang-format off
    options.add_options()
        (file_path_opt, "", cxxopts::value<std::string>())
        (dump_tokens_opt, "")
        (dump_ast_opt, "")
        (dump_table_opt, "")
        (dump_asm_opt, "")
        ("h,help", "Print help");
    // clang-format on

    options.parse_positional({file_path_opt});

    const auto result = options.parse(argc, argv);

    if (result.count("help") > 0 || result.count(file_path_opt) != 1) {
      std::cout << options.help() << "\n";
      return 0;
    }

    std::string in_st = result[file_path_opt].as<std::string>();
    std::ifstream input_stream(in_st);

    if (!input_stream.good()) {
      std::cerr << "Unable to read stream\n";
      return 1;
    }

    if (result.count(dump_tokens_opt) > 0) {
      csharp::dump_tokens(input_stream, std::cout);
    } else {
      auto parser_result = csharp::parse(input_stream);
      if (!parser_result.errors_.empty()) {
        csharp::dump_errors(parser_result.errors_, std::cerr);
        return 0;
      }
      auto symbol_table =
          csharp::ast::SymbolTableVisitor::exec(parser_result.program_);
      if (result.count(dump_ast_opt) > 0) {
        csharp::dump_ast(parser_result.program_, std::cout);
      } else if (result.count(dump_table_opt) > 0) {
        csharp::dump_table(symbol_table);
      }
      std::regex target(".cs");
      std::string str{(std::filesystem::current_path() / in_st).c_str()};
      const auto filename = std::regex_replace(str, target, std::string{});
      std::ofstream output_stream(filename + ".ll");
      csharp::generate_code(
          parser_result.program_, symbol_table, output_stream);
      output_stream.close();
      if (result.count("--dump-asm") == 0) {
        csharp::generate_exec(filename + ".ll", filename);
      }
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
