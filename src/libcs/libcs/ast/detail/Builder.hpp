#pragma once

#include <libcs/ast/Ast.hpp>
#include <libcs/parser.hpp>

#include <CSharpBaseVisitor.h>

#include <any>

namespace csharp::ast::detail {

class Builder final : public CSharpBaseVisitor {
 public:
  explicit Builder(Program &program) : program_(program) {}

  antlrcpp::Any visitProgram(CSharpParser::ProgramContext *context) override;
  antlrcpp::Any visitClass(CSharpParser::ClassContext *context) override;
  antlrcpp::Any visitExpression(CSharpParser::ExpressionContext *context) override;
  antlrcpp::Any visitExpressions(CSharpParser::ExpressionsContext *context) override;
  antlrcpp::Any visitMas_statement(CSharpParser::Mas_statementContext *context) override;
  antlrcpp::Any visitAssign_statement(CSharpParser::Assign_statementContext *context) override;
  antlrcpp::Any visitLiteral(CSharpParser::LiteralContext *context) override;
  antlrcpp::Any visitMas_change(CSharpParser::Mas_changeContext *context) override;
  antlrcpp::Any visitMas_selection(CSharpParser::Mas_selectionContext *context) override;
  antlrcpp::Any visitMas_def(CSharpParser::Mas_defContext *context) override;
  antlrcpp::Any visitVar_def(CSharpParser::Var_defContext *context) override;
  antlrcpp::Any visitFunc_def(CSharpParser::Func_defContext *context) override;
  antlrcpp::Any visitScope(CSharpParser::ScopeContext *context) override;
  antlrcpp::Any visitReturn_statement(CSharpParser::Return_statementContext *context) override;
  antlrcpp::Any visitStatement(CSharpParser::StatementContext *context) override;
  antlrcpp::Any visitFunc_call(CSharpParser::Func_callContext *context) override;
  antlrcpp::Any visitArgs(CSharpParser::ArgsContext *context) override;
  antlrcpp::Any visitArg(CSharpParser::ArgContext *context) override;
  antlrcpp::Any visitPars(CSharpParser::ParsContext *context) override;
  antlrcpp::Any visitIf_statement(CSharpParser::If_statementContext *context) override;
  antlrcpp::Any visitElse_statement(CSharpParser::Else_statementContext *context) override;
  antlrcpp::Any visitFor_statement(CSharpParser::For_statementContext *context) override;
  antlrcpp::Any visitFor_condition(CSharpParser::For_conditionContext *context) override;
  antlrcpp::Any visitFor_operation(CSharpParser::For_operationContext *context) override;
  antlrcpp::Any visitKw_statement(CSharpParser::Kw_statementContext *context) override;
  antlrcpp::Any visitPrint_statement(CSharpParser::Print_statementContext *context) override;
  antlrcpp::Any visitRead_statement(CSharpParser::Read_statementContext *context) override;

private:
  ast::Program &program_;
};

}  // namespace csharp::ast::detail
