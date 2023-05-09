#pragma once

#include <libcs/ast/Ast.hpp>
#include <libcs/parser.hpp>

#include <CSharpVisitor.h>

#include <any>

namespace csharp::ast::detail {

class Builder final : public CSharpVisitor {
 public:
  explicit Builder(ast::Program &program) : program_(program) {}

  std::any visitExpressions(CSharpParser::ExpressionsContext *context) override;
  std::any visitProgram(CSharpParser::ProgramContext *context) override;
  std::any visitClass(CSharpParser::ClassContext *context) override;
  std::any visitVar_def(CSharpParser::Var_defContext *context) override;
  std::any visitFunc_def(CSharpParser::Func_defContext *context) override;
  std::any visitAssign_statement(CSharpParser::Assign_statementContext *context) override;
  std::any visitStatement(CSharpParser::StatementContext *context) override;
  std::any visitScope(CSharpParser::ScopeContext *context) override;
  std::any visitFunc_call(CSharpParser::Func_callContext *context) override;
  std::any visitArgs(CSharpParser::ArgsContext *context) override;
  std::any visitLiteral(CSharpParser::LiteralContext *context) override;
  std::any visitReturn_statement(CSharpParser::Return_statementContext *context) override;
  std::any visitArg(CSharpParser::ArgContext *context) override;
  std::any visitIf_statement(CSharpParser::If_statementContext *context) override;
  std::any visitFor_statement(CSharpParser::For_statementContext *context) override;
  std::any visitFor_condition(CSharpParser::For_conditionContext *context) override;
  std::any visitFor_operation(CSharpParser::For_operationContext *context) override;
  std::any visitKw_statement(CSharpParser::Kw_statementContext *context) override;
  std::any visitPars(CSharpParser::ParsContext *context) override;
  std::any visitPrint_statement(CSharpParser::Print_statementContext *context) override;
  std::any visitElse_statement(CSharpParser::Else_statementContext *context) override;
  std::any visitRead_statement(CSharpParser::Read_statementContext *context) override;

private:
  ast::Program &program_;
};

}  // namespace csharp::ast::detail
