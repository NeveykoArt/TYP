#include <libcs/ast/detail/Builder.hpp>
#include <iostream>
#include <algorithm>

namespace csharp::ast::detail {

static std::string normalize_register(const std::string &str) {
    auto text = str;
    std::transform(
        text.begin(), text.end(), text.begin(), [](unsigned char chr) {
            return std::tolower(chr);
        });
    return text;
}

std::any Builder::visitProgram(CSharpParser::ProgramContext *context)  {
    std::cout << "visitProgram\n";
    auto* csclass = dynamic_cast<CSClass *>(std::any_cast<Node*>(context->class_()->accept(this)));
    program_.set_csclass(csclass);
    std::cout << "visitProgram-----\n";
    return csclass;
}

std::any Builder::visitClass(CSharpParser::ClassContext *context)  {
    std::cout << "visitClass\n";
    auto* value = dynamic_cast<Expression *>(std::any_cast<Node*>(visit(context->expression())));
    auto class_name = context->ID()->getText();
    std::cout << "visitClass----\n";
    return static_cast<Node*>(program_.create_node<CSClass>(class_name, value));
}

std::any Builder::visitExpression(CSharpParser::ExpressionContext *context) {
    std::cout << "visitExpression\n";
    std::vector<Expressions *> exprs;
    if (!context->expressions().empty()) {
        for (auto* el : context->expressions()) {
            exprs.push_back(dynamic_cast<Expressions *>(std::any_cast<Node*>(visit(el))));
        }
        std::cout << "visitExpression-----\n";
        return static_cast<Node*>(program_.create_node<Expression>(exprs));
    }
    std::cout << "visitExpression----\n";
    return 0;
}

std::any Builder::visitExpressions(CSharpParser::ExpressionsContext *context)  {
    std::cout << "visitExpressions\n";
    if (context->func_def() != nullptr) {
        auto* f_d = dynamic_cast<Func_def*>(
        std::any_cast<Node*>(visit(context->func_def())));
        std::cout << "visitExpressions----------\n";
        return static_cast<Node*>(program_.create_node<Expressions>(f_d, 1));
    }
    if (context->assign_statement() != nullptr) {
        auto* assig = dynamic_cast<Assign_statement*>(
        std::any_cast<Node*>(visit(context->assign_statement())));
        std::cout << "visitExpressions----------\n";
        return static_cast<Node*>(program_.create_node<Expressions>(assig, 4));
    }
    if (context->if_statement() != nullptr) {
        auto* i_s =
        dynamic_cast<If_statement*>(std::any_cast<Node*>(visit(context->if_statement())));
        std::cout << "visitExpressions----------\n";
        return static_cast<Node*>(program_.create_node<Expressions>(i_s, 5));
    }
    if (context->for_statement() != nullptr) {
        auto* f_s = dynamic_cast<For_statement*>(
        std::any_cast<Node*>(visit(context->for_statement())));
        std::cout << "visitExpressions----------\n";
        return static_cast<Node*>(program_.create_node<Expressions>(f_s, 6));
    }
    if (context->read_statement() != nullptr) {
        auto* r_s =
        dynamic_cast<Read_statement*>(std::any_cast<Node*>(visit(context->read_statement())));
        std::cout << "visitExpressions----------\n";
        return static_cast<Node*>(program_.create_node<Expressions>(r_s, 2));
    }
    if (context->print_statement() != nullptr) {
        auto* p_s =
        dynamic_cast<Print_statement*>(std::any_cast<Node*>(visit(context->print_statement())));
        std::cout << "visitExpressions----------\n";
        return static_cast<Node*>(program_.create_node<Expressions>(p_s, 3));
    }
    std::cout << "visitExpressions----------------\n";
    return 0;
}

std::any Builder::visitAssign_statement(CSharpParser::Assign_statementContext *context)  {
    std::cout << "visitAssign_statement\n";
    if (context->var_def() != nullptr) {
        auto* v_d =
        dynamic_cast<Var_def*>(std::any_cast<Node*>(visit(context->var_def())));
        if (context->func_call() != nullptr) {
            auto* f_c =
            dynamic_cast<Func_call*>(std::any_cast<Node*>(visit(context->func_call())));
            std::cout << "visitAssign_statement----\n";
            return static_cast<Node*>(program_.create_node<Assign_statement>(v_d, f_c));
        } else {
            if (!context->BINARY_OP()->getText().empty()) {
                std::vector<Arg *> exprs;
                for (auto* el : context->arg()) {
                    exprs.push_back(dynamic_cast<Arg *>(std::any_cast<Node*>(visit(el))));
                }
                std::cout << "visitAssign_statement----\n";
                return static_cast<Node*>(program_.create_node<Assign_statement>(v_d, exprs, context->BINARY_OP()->getText()));
            } else {
                std::vector<Arg *> exprs;
                for (auto* el : context->arg()) {
                    exprs.push_back(dynamic_cast<Arg *>(std::any_cast<Node*>(visit(el))));
                }
                std::cout << "visitAssign_statement----\n";
                return static_cast<Node*>(program_.create_node<Assign_statement>(v_d, exprs));
            }
        }
    } else {
        auto id_left = context->ID()->getText();
        if (context->func_call() != nullptr) {
            auto* f_c =
            dynamic_cast<Func_call*>(std::any_cast<Node*>(visit(context->func_call())));
            std::cout << "visitAssign_statement----\n";
            return static_cast<Node*>(program_.create_node<Assign_statement>(id_left, f_c));
        } else {
            if (!context->BINARY_OP()->getText().empty()) {
                std::vector<Arg *> exprs;
                for (auto* el : context->arg()) {
                    exprs.push_back(dynamic_cast<Arg *>(std::any_cast<Node*>(visit(el))));
                }
                auto binop = context->BINARY_OP()->getText();
                std::cout << "visitAssign_statement----\n";
                return static_cast<Node*>(program_.create_node<Assign_statement>(id_left, exprs, binop));
            } else {
                std::vector<Arg *> exprs;
                for (auto* el : context->arg()) {
                    exprs.push_back(dynamic_cast<Arg *>(std::any_cast<Node*>(visit(el))));
                }
                std::cout << "visitAssign_statement----\n";
                return static_cast<Node*>(program_.create_node<Assign_statement>(id_left, exprs));
            }
        }
    }
    std::cout << "visitAssign_statement------------------\n";
    return 0;
}

std::any Builder::visitLiteral(CSharpParser::LiteralContext *context)  {
    std::cout << "visitLiteral\n";
    return static_cast<Node*>(program_.create_node<Literal>(normalize_register(context->getText())));
}

std::any Builder::visitVar_def(CSharpParser::Var_defContext *context)  {
    std::cout << "visitVar_def\n";
    if (!context->VAR()->getText().empty()) {
        auto var_line_var = context->VAR()->getText();
        auto var_line_id = context->ID()->getText();
        std::string var_line_braks = "[] ";
        if (context->SLP() != nullptr) {
            std::cout << "visitVar_def----\n";
            return static_cast<Node*>(program_.create_node<Var_def>(var_line_var + var_line_braks + var_line_id));
        }
        std::cout << "visitVar_def----\n";
        return static_cast<Node*>(program_.create_node<Var_def>(var_line_var + " " + var_line_id));
    }
    std::cout << "visitVar_def----------------\n";
    return 0;
}

std::any Builder::visitFunc_def(CSharpParser::Func_defContext *context)  {
    std::cout << "visitFunc_def\n";
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
    std::cout << "visitFunc_def----------------\n";
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
    auto* assign = dynamic_cast<Assign_statement*>(
            std::any_cast<Node*>(visit(context->assign_statement())));
    auto* cond = dynamic_cast<For_condition*>(
            std::any_cast<Node*>(visit(context->for_condition())));
    auto* oper = dynamic_cast<For_operation*>(
            std::any_cast<Node*>(visit(context->for_operation())));
    auto* scope = dynamic_cast<Scope*>(
            std::any_cast<Node*>(visit(context->scope())));
    return static_cast<Node*>(program_.create_node<For_statement>(assign, cond, oper, scope));
}

std::any Builder::visitFor_condition(CSharpParser::For_conditionContext *context)  {
    auto arg1 = context->ID()->getText();
    auto logop = context->LOGIC_OP()->getText();
    auto* arg2 = dynamic_cast<Arg*>(
        std::any_cast<Node*>(visit(context->arg())));
    return static_cast<Node*>(program_.create_node<For_condition>(arg1, logop, arg2));
}

std::any Builder::visitFor_operation(CSharpParser::For_operationContext *context)  {
    if (context->assign_statement() != nullptr) {
        auto* value = dynamic_cast<Assign_statement*>(
        std::any_cast<Node*>(visit(context->assign_statement())));
        return static_cast<Node*>(program_.create_node<For_operation>(value));
    } else {
        auto id_var = context->ID()->getText();
        auto unarop = context->UNARYMATHEXP()->getText();
        return static_cast<Node*>(program_.create_node<For_operation>(id_var, unarop));
    }
    return 0;
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
