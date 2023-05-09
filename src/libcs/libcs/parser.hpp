#pragma once

#include <libcs/ast/Ast.hpp>

#include <iosfwd>

namespace csharp {

struct Error {
  size_t line_;
  size_t column_;
  std::string message_;
};

using Errors = std::vector<Error>;

struct ParseResult {
  ast::Program program_;
  Errors errors_;

  static ParseResult program(ast::Program program) {
    ParseResult result;
    result.program_ = std::move(program);
    return result;
  }

  static ParseResult errors(Errors errors) {
    ParseResult result;
    result.errors_ = std::move(errors);
    return result;
  }
};

ParseResult parse(std::ifstream &input_stream);

void dump_ast(ast::Program &program, std::ostream& stream);
void dump_errors(const Errors &errors, std::ostream& out);

}  // namespace csharp
