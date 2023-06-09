#include <libcs/parser.hpp>

#include <libcs/ast/Serializer.hpp>
#include <libcs/ast/detail/Builder.hpp>

#include <CSharpLexer.h>
#include <CSharpParser.h>

#include <fmt/format.h>
#include <iomanip>
#include <iostream>

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

ParseResult parse(std::ifstream& input_stream) {
  antlr4::ANTLRInputStream stream(input_stream);
  CSharpLexer lexer(&stream);
  antlr4::CommonTokenStream tokens(&lexer);
  CSharpParser parser(&tokens);

  StreamErrorListener error_listener;
  parser.removeErrorListeners();
  parser.addErrorListener(&error_listener);

  auto* program_parse_tree = parser.program();

  const auto& errors = error_listener.errors();
  if (!errors.empty()) {
    return ParseResult::errors(errors);
  }

  ast::Program program;
  ast::detail::Builder builder(program);
  builder.visit(program_parse_tree);

  return ParseResult::program(std::move(program));
}

void dump_ast(ast::Program& program, std::ostream& stream) {
  ast::Serializer::exec(program, stream);
}

void dump_table(ast::SymbolTable& symb_tab) {
  std::cout << "\t\tSymbolTable\n";
  std::cout << "------------------------------------------------\n";
  for (auto& el : symb_tab) {
    std::cout << std::setw(15) << std::left << el.get_table_typ() << "|"
              << std::setw(20) << el.get_obj_name() << "|" << std::setw(10)
              << el.get_var_typ() << "|" << std::setw(15)
              << el.get_scope_level() << "\n";
  }
}

void dump_errors(const Errors& errors, std::ostream& out) {
  for (const auto& error : errors) {
    out << fmt::format(
        "{}:{} {}\n", error.line_, error.column_, error.message_);
  }
}

void generate_code(
    ast::Program& program,
    ast::SymbolTable& symbol_table,
    std::ostream& out) {
  ast::CodeGenerator::exec(program, symbol_table, out);
}

void generate_exec(std::string_view input_file, std::string_view output_file) {
  std::stringstream ss;
  ss << "clang++ " << input_file << " -o " << output_file;
  system(ss.str().c_str());
}

}  // namespace csharp
