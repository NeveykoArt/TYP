#include <libcs/parser.hpp>

#include <libcs/ast/Serializer.hpp>
#include <libcs/ast/detail/Builder.hpp>

#include <CSharpLexer.h>
#include <CSharpParser.h>

#include <iostream>
#include <fmt/format.h>

namespace csharp {

namespace {

class StreamErrorListener : public antlr4::BaseErrorListener {
 public:
  void syntaxError(
      antlr4::Recognizer* /*recognizer*/,
      antlr4::Token* /*offendingSymbol*/,
      size_t line,
      size_t column,
      const std::string& message,
      std::exception_ptr /*e*/) override {
    errors_.emplace_back(Error{line, column, message});
  }

  const Errors& errors() const { return errors_; }

 private:
  Errors errors_;
};

}  // namespace

ParseResult parse(std::ifstream &input_stream) {
  std::cout << "pizdaP1000456546";
  antlr4::ANTLRInputStream stream(input_stream);
  CSharpLexer lexer(&stream);
  antlr4::CommonTokenStream tokens(&lexer);
  CSharpParser parser(&tokens);

  std::cout << "pizdaP1000";

  StreamErrorListener error_listener;
  parser.removeErrorListeners();
  parser.addErrorListener(&error_listener);

  auto *program_parse_tree = parser.program();

  const auto& errors = error_listener.errors();
  if (!errors.empty()) {
    return ParseResult::errors(errors);
  }

  std::cout << "pizdaP1";
  ast::Program program;
  std::cout << "pizdaP2";
  ast::detail::Builder builder(program);
  std::cout << "pizdaP3";
  builder.visit(program_parse_tree);

  return ParseResult::program(std::move(program));
}

void dump_ast(ast::Program &program, std::ostream &stream) {
  ast::Serializer::exec(program, stream);
}

void dump_errors(const Errors& errors, std::ostream& out) {
  for (const auto& error : errors) {
    out << fmt::format(
        "{}:{} {}\n", error.line_, error.column_, error.message_);
  }
}

}  // namespace csharp
