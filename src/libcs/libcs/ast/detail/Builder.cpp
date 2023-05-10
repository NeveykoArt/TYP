#include <libcs/ast/detail/Builder.hpp>
#include <iostream>

namespace csharp::ast::detail {

std::any Builder::visitProgram(CSharpParser::ProgramContext *context)  {
    auto* csclass = std::any_cast<Node*>(visitChildren(context));
    program_.set_csclass(csclass);
    return csclass;
}

std::any Builder::visitClass(CSharpParser::ClassContext *context)  {
    auto* value = dynamic_cast<Expression *>(std::any_cast<Node*>(visit(context->expression())));
    auto class_name = context->ID()->getText();
    return static_cast<Node*>(program_.create_node<CSClass>(class_name, value));
}

std::any Builder::visitExpression(CSharpParser::ExpressionContext *context) {
    std::vector<Expressions *> exprs;
    for (auto* el : context->expressions()) {
        exprs.push_back(dynamic_cast<Expressions *>(std::any_cast<Node*>(visit(el))));
    }
    return static_cast<Node*>(program_.create_node<Expression>(exprs));
}

std::any Builder::visitExpressions(CSharpParser::ExpressionsContext *context)  {
    if (context->func_def() != nullptr) {
        auto* f_d = dynamic_cast<Func_def*>(
        std::any_cast<Node*>(visit(context->func_def())));
        return static_cast<Node*>(program_.create_node<Expressions>(f_d));
    }
    if (context->assign_statement() != nullptr) {
        auto* assig = dynamic_cast<Assign_statement*>(
        std::any_cast<Node*>(visit(context->assign_statement())));
        return static_cast<Node*>(program_.create_node<Expressions>(assig));
    }
    if (context->if_statement() != nullptr) {
        auto* i_s =
        dynamic_cast<If_statement*>(std::any_cast<Node*>(visit(context->if_statement())));
        return static_cast<Node*>(program_.create_node<Expressions>(i_s));
    }
    if (context->for_statement() != nullptr) {
        auto* f_s = dynamic_cast<For_statement*>(
        std::any_cast<Node*>(visit(context->for_statement())));
        return static_cast<Node*>(program_.create_node<Expressions>(f_s));
    }
    if (context->read_statement() != nullptr) {
        auto* r_s =
        dynamic_cast<Read_statement*>(std::any_cast<Node*>(visit(context->read_statement())));
        return static_cast<Node*>(program_.create_node<Expressions>(r_s));
    }
    if (context->print_statement() != nullptr) {
        auto* p_s =
        dynamic_cast<Print_statement*>(std::any_cast<Node*>(visit(context->print_statement())));
        return static_cast<Node*>(program_.create_node<Expressions>(p_s));
    }
    return 0;
}

std::any Builder::visitAssign_statement(CSharpParser::Assign_statementContext *context)  {
    if (contex->var_def() != nullptr) {
        auto* v_d =
        dynamic_cast<Var_def*>(std::any_cast<Node*>(visit(context->var_def())));

    } else {
        auto id_left = context->ID()->getText();
        if (context->func_call() != nullptr) {
            auto* f_c =
            dynamic_cast<Func_call*>(std::any_cast<Node*>(visit(context->func_call())));

        } else {
            if (!context->BINARY_OP()->getText().empty()) {
                std::vector<Kw_statement *> exprs;
                for (auto* el : context->KEYWORD()) {
                    exprs.push_back(dynamic_cast<Kw_statement *>(std::any_cast<Node*>(visit(el))));
                }
                return static_cast<Node*>(program_.create_node<Func_def>(exprs, void_tmp, func_name, pars, scope, return_tmp));
            } else {

            }
        }
    }
    return 0;
}

std::any Builder::visitLiteral(CSharpParser::LiteralContext *context)  {
    std::string literal;
    if (!context->TEXT()->getText().empty()) {
        literal = context->TEXT()->getText();
    }
    if (!context->NUMBER()->getText().empty()) {
        literal = context->NUMBER()->getText();
    }
    if (!context->CHARv()->getText().empty()) {
        literal = context->CHARv()->getText();
    }
    if (!context->FLOAT_NUMBER()->getText().empty()) {
        literal = context->FLOAT_NUMBER()->getText();
    }
    return static_cast<Node*>(program_.create_node<Literal>(literal));
}

std::any Builder::visitVar_def(CSharpParser::Var_defContext *context)  {
    if (!context->VAR()->getText().empty()) {
        auto var_line_var = context->VAR()->getText();
        auto var_line_id = context->ID()->getText();
        std::string var_line_braks = "[] ";
        if (!context->SLP()->getText().empty()) {
            return static_cast<Node*>(program_.create_node<Var_def>(var_line_var + var_line_braks + var_line_id));
        }
        return static_cast<Node*>(program_.create_node<Var_def>(var_line_var + var_line_id));
    }
    return 0;
}

std::any Builder::visitFunc_def(CSharpParser::Func_defContext *context)  {
    auto* pars = dynamic_cast<Pars *>(std::any_cast<Node*>(visit(context->pars())));
    auto* scope = dynamic_cast<Scope *>(std::any_cast<Node*>(visit(context->scope())));
    auto* return_tmp = dynamic_cast<Return_statement *>(std::any_cast<Node*>(visit(context->return_statement())));
    auto func_name = context->ID()->getText();
    if (!context->VOID()->getText().empty()) {
        bool void_tmp = true;
        if (!context->KEYWORD().empty()) {
            std::vector<Kw_statement *> exprs;
            for (auto* el : context->KEYWORD()) {
                exprs.push_back(dynamic_cast<Kw_statement *>(std::any_cast<Node*>(visit(el))));
            }
            return static_cast<Node*>(program_.create_node<Func_def>(exprs, void_tmp, func_name, pars, scope, return_tmp));
        }
        return static_cast<Node*>(program_.create_node<Func_def>(void_tmp, func_name, pars, scope, return_tmp));
    }

    if (!context->VAR()->getText().empty()) {
        auto var = context->VAR()->getText();
        return static_cast<Node*>(program_.create_node<Func_def>(var, func_name, pars, scope, return_tmp));
    }

    return 0;
}

std::any Builder::visitScope(CSharpParser::ScopeContext *context)  {
    std::vector<Statement *> states;
    for (auto* el : context->statement()) {
        states.push_back(dynamic_cast<Statement *>(std::any_cast<Node*>(visit(el))));
    }
    return static_cast<Node*>(program_.create_node<Scope>(states));
}

std::any Builder::visitReturn_statement(CSharpParser::Return_statementContext *context)  {
    auto* arg = dynamic_cast<Arg*>(
        std::any_cast<Node*>(visit(context->arg())));
    return static_cast<Node*>(program_.create_node<Return_statement>(arg));
}

std::any Builder::visitStatement(CSharpParser::StatementContext *context)  {
    if (context->func_call() != nullptr) {
        auto* f_c = dynamic_cast<Func_call*>(
        std::any_cast<Node*>(visit(context->func_call())));
        return static_cast<Node*>(program_.create_node<Statement>(f_c));
    }
    if (context->assign_statement() != nullptr) {
        auto* assig = dynamic_cast<Assign_statement*>(
        std::any_cast<Node*>(visit(context->assign_statement())));
        return static_cast<Node*>(program_.create_node<Statement>(assig));
    }
    if (context->if_statement() != nullptr) {
        auto* i_s =
        dynamic_cast<If_statement*>(std::any_cast<Node*>(visit(context->if_statement())));
        return static_cast<Node*>(program_.create_node<Statement>(i_s));
    }
    if (context->for_statement() != nullptr) {
        auto* f_s = dynamic_cast<For_statement*>(
        std::any_cast<Node*>(visit(context->for_statement())));
        return static_cast<Node*>(program_.create_node<Statement>(f_s));
    }
    if (context->kw_statement() != nullptr) {
        auto* k_s = dynamic_cast<Kw_statement*>(
        std::any_cast<Node*>(visit(context->kw_statement())));
        return static_cast<Node*>(program_.create_node<Statement>(k_s));
    }
    if (context->read_statement() != nullptr) {
        auto* r_s =
        dynamic_cast<Read_statement*>(std::any_cast<Node*>(visit(context->read_statement())));
        return static_cast<Node*>(program_.create_node<Statement>(r_s));
    }
    if (context->print_statement() != nullptr) {
        auto* p_s =
        dynamic_cast<Print_statement*>(std::any_cast<Node*>(visit(context->print_statement())));
        return static_cast<Node*>(program_.create_node<Statement>(p_s));
    }
    return 0;
}

std::any Builder::visitFunc_call(CSharpParser::Func_callContext *context)  {
    auto* value = dynamic_cast<Arguments *>(std::any_cast<Node*>(visit(context->args())));
    std::string func_name;
    if (!context->ID()->getText().empty()) {
        func_name = context->ID()->getText();
    }
    if (!context->WRITELN()->getText().empty()) {
        func_name = context->WRITELN()->getText();
    }
    if (!context->READLN()->getText().empty()) {
        func_name = context->READLN()->getText();
    }
    return static_cast<Node*>(program_.create_node<Func_call>(func_name, value));
}

std::any Builder::visitArgs(CSharpParser::ArgsContext *context)  {
    std::vector<Arg *> args;
    for (auto* el : context->arg()) {
        args.push_back(dynamic_cast<Arg *>(std::any_cast<Node*>(visit(el))));
    }
    return static_cast<Node*>(program_.create_node<Arguments>(args));
}

std::any Builder::visitArg(CSharpParser::ArgContext *context)  {
    if (context->literal() != nullptr) {
        auto* l = dynamic_cast<Literal*>(
        std::any_cast<Node*>(visit(context->literal())));
        return static_cast<Node*>(program_.create_node<Arg>(l));
    }
    auto arg_id = context->ID()->getText();
    return static_cast<Node*>(program_.create_node<Arg>(arg_id));
}

std::any Builder::visitPars(CSharpParser::ParsContext *context)  {
    std::vector<Var_def *> exprs;
    for (auto* el : context->var_def()) {
        exprs.push_back(dynamic_cast<Var_def *>(std::any_cast<Node*>(visit(el))));
    }
    return static_cast<Node*>(program_.create_node<Pars>(exprs));
}

std::any Builder::visitIf_statement(CSharpParser::If_statementContext *context)  {
    auto arg1 = context->ID()->getText();
    auto* scope = dynamic_cast<Scope *>(std::any_cast<Node*>(visit(context->scope())));

    if (context->arg() != nullptr) {
        auto* arg2 = dynamic_cast<Arg*>(
        std::any_cast<Node*>(visit(context->arg())));
        auto logop = context->LOGIC_OP()->getText();
        if (context->else_statement() != nullptr) {
            auto* else_st = dynamic_cast<Else_statement*>(
            std::any_cast<Node*>(visit(context->else_statement())));
            return static_cast<Node*>(program_.create_node<If_statement>(arg1, logop, arg2, scope, else_st));
        }
        return static_cast<Node*>(program_.create_node<If_statement>(arg1, logop, arg2, scope));
    }

    if (context->else_statement() != nullptr) {
        auto* else_st = dynamic_cast<Else_statement*>(
        std::any_cast<Node*>(visit(context->else_statement())));
        return static_cast<Node*>(program_.create_node<If_statement>(arg1, scope, else_st));
    }
    return static_cast<Node*>(program_.create_node<If_statement>(arg1, scope));
}

std::any Builder::visitElse_statement(CSharpParser::Else_statementContext *context)  {
    auto* arg = dynamic_cast<Scope*>(
        std::any_cast<Node*>(visit(context->scope())));
    return static_cast<Node*>(program_.create_node<Else_statement>(arg));
}

std::any Builder::visitFor_statement(CSharpParser::For_statementContext *context)  {
return visitChildren(context);
}

std::any Builder::visitFor_condition(CSharpParser::For_conditionContext *context)  {
    auto arg1 = context->ID()->getText();
    auto logop = context->LOGIC_OP()->getText();
    auto* arg2 = dynamic_cast<Arg*>(
        std::any_cast<Node*>(visit(context->arg())));
    return static_cast<Node*>(program_.create_node<For_condition>(arg1, logop, arg2));
}

std::any Builder::visitFor_operation(CSharpParser::For_operationContext *context)  {
return visitChildren(context);
}

std::any Builder::visitKw_statement(CSharpParser::Kw_statementContext *context)  {
    auto key_word = context->KEYWORD()->getText();
    return static_cast<Node*>(program_.create_node<Kw_statement>(key_word));
}

std::any Builder::visitPrint_statement(CSharpParser::Print_statementContext *context)  {
    auto id = context->ID()->getText();
    return static_cast<Node*>(program_.create_node<Print_statement>(id));
}

std::any Builder::visitRead_statement(CSharpParser::Read_statementContext *context)  {
    auto id = context->ID()->getText();
    return static_cast<Node*>(program_.create_node<Read_statement>(id));
}

}  // namespace csharp::ast::detail
