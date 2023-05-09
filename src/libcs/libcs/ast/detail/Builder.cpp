#include <libcs/ast/detail/Builder.hpp>

namespace csharp::ast::detail {

std::any Builder::visitProgram(CSharpParser::ProgramContext *context)
{
    auto *csclass = dynamic_cast<CSClass *>(
        std::any_cast<Node *>(context->class_()->accept(this)));
    program_.set_csclass(csclass);

    return csclass;
}

std::any Builder::visitClass(CSharpParser::ClassContext *context)
{
    auto classname = context->ID()->getText();
    program_.get_csclass()->set_class_name(classname);

    auto *expressions = dynamic_cast<Expressions *>(
          std::any_cast<Node *>(context->expressions(0)->accept(this)));
    program_.get_csclass()->set_exprs(expressions);

    return expressions;
}

std::any Builder::visitExpressions(CSharpParser::ExpressionsContext *context)
{
  if (context->func_def() != nullptr)
  {
      return antlrcpp::Any(context->func_def());
  }
  if (context->assign_statement() != nullptr)
  {
      return antlrcpp::Any(context->assign_statement());
  }
  if (context->if_statement() != nullptr)
  {
      return antlrcpp::Any(context->if_statement());
  }
  if (context->for_statement() != nullptr)
  {
      return antlrcpp::Any(context->for_statement());
  }
  if (context->print_statement() != nullptr)
  {
      return antlrcpp::Any(context->print_statement());
  }
  if (context->read_statement() != nullptr)
  {
      return antlrcpp::Any(context->read_statement());
  }
  return antlrcpp::Any(context);
}

std::any Builder::visitStatement(CSharpParser::StatementContext *context)
{
  if (context->func_call() != nullptr)
  {
      return antlrcpp::Any(context->func_call());
  }
  if (context->assign_statement() != nullptr)
  {
      return antlrcpp::Any(context->assign_statement());
  }
  if (context->if_statement() != nullptr)
  {
      return antlrcpp::Any(context->if_statement());
  }
  if (context->kw_statement() != nullptr)
  {
      return antlrcpp::Any(context->kw_statement());
  }
  if (context->for_statement() != nullptr)
  {
      return antlrcpp::Any(context->for_statement());
  }
  if (context->print_statement() != nullptr)
  {
      return antlrcpp::Any(context->print_statement());
  }
  if (context->read_statement() != nullptr)
  {
      return antlrcpp::Any(context->read_statement());
  }
  return antlrcpp::Any(context);
}

std::any Builder::visitAssign_statement(CSharpParser::Assign_statementContext *context)
{
  return antlrcpp::Any(context);
}

std::any Builder::visitVar_def(CSharpParser::Var_defContext *context)
{
  return antlrcpp::Any(context);
}

std::any Builder::visitScope(CSharpParser::ScopeContext *context)
{
  return antlrcpp::Any(context);
}

std::any Builder::visitFunc_def(CSharpParser::Func_defContext *context)
{
  return antlrcpp::Any(context);
}

std::any Builder::visitFunc_call(CSharpParser::Func_callContext *context)
{
  return antlrcpp::Any(context);
}

std::any Builder::visitArgs(CSharpParser::ArgsContext *context)
{
  if (!context->arg().empty())
  {
      return antlrcpp::Any(context->arg());
  }
  return antlrcpp::Any(context);
}

std::any Builder::visitPars(CSharpParser::ParsContext *context)
{
  return antlrcpp::Any(context);
}

std::any Builder::visitIf_statement(CSharpParser::If_statementContext *context)
{
  return antlrcpp::Any(context);
}

std::any Builder::visitElse_statement(CSharpParser::Else_statementContext *context)
{
  return antlrcpp::Any(context);
}

std::any Builder::visitLiteral(CSharpParser::LiteralContext *context)
{
  if (context->NUMBER() != nullptr)
  {
      return antlrcpp::Any(context->NUMBER());
  }
  if (context->FLOAT_NUMBER() != nullptr)
  {
      return antlrcpp::Any(context->FLOAT_NUMBER());
  }
  if (context->TEXT() != nullptr)
  {
      return antlrcpp::Any(context->TEXT());
  }
  if (context->CHARv() != nullptr)
  {
      return antlrcpp::Any(context->CHARv());
  }
  return antlrcpp::Any(context);
}

std::any Builder::visitReturn_statement(CSharpParser::Return_statementContext *context)
{
  if (context->ID() != nullptr)
  {
      return antlrcpp::Any(context->ID());
  }
  if (context->literal() != nullptr)
  {
      if (context->literal()->NUMBER() != nullptr)
      {
          return antlrcpp::Any(context->literal()->NUMBER());
      }
      if (context->literal()->FLOAT_NUMBER() != nullptr)
      {
          return antlrcpp::Any(context->literal()->FLOAT_NUMBER());
      }
      if (context->literal()->TEXT() != nullptr)
      {
          return antlrcpp::Any(context->literal()->TEXT());
      }
      if (context->literal()->CHARv() != nullptr)
      {
          return antlrcpp::Any(context->literal()->CHARv());
      }
  }
  return antlrcpp::Any(context);
}

std::any Builder::visitArg(CSharpParser::ArgContext *context)
{
  if (context->ID() != nullptr)
  {
      return antlrcpp::Any(context->ID());
  }
  if (context->literal() != nullptr)
  {
      return visitLiteral(context->literal());
  }
    return antlrcpp::Any(context);
}

std::any Builder::visitFor_statement(CSharpParser::For_statementContext *context)
{
  return antlrcpp::Any(context);
}

std::any Builder::visitPrint_statement(CSharpParser::Print_statementContext *context)
{
  return antlrcpp::Any(context);
}

std::any Builder::visitFor_condition(CSharpParser::For_conditionContext *context)
{
  return antlrcpp::Any(context);
}

std::any Builder::visitFor_operation(CSharpParser::For_operationContext *context)
{
  return antlrcpp::Any(context);
}

std::any Builder::visitKw_statement(CSharpParser::Kw_statementContext *context)
{
  return antlrcpp::Any(context);
}

std::any Builder::visitRead_statement(CSharpParser::Read_statementContext *context)
{
  return antlrcpp::Any(context);
}

}  // namespace csharp::ast::detail
