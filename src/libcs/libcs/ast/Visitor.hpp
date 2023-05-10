#pragma once

#include <libcs/ast/Ast.hpp>

namespace csharp::ast {

class Visitor {
 public:
    virtual void visit(CSClass &node) = 0;
    virtual void visit(Expression &node) = 0;
    virtual void visit(Expressions &node) = 0;
    virtual void visit(Assign_statement &node) = 0;
    virtual void visit(Literal &node) = 0;
    virtual void visit(Var_def &node) = 0;
    virtual void visit(Func_def &node) = 0;
    virtual void visit(Scope &node) = 0;
    virtual void visit(Return_statement &node) = 0;
    virtual void visit(Statement &node) = 0;
    virtual void visit(Func_call &node) = 0;
    virtual void visit(Args &node) = 0;
    virtual void visit(Arg &node) = 0;
    virtual void visit(Pars &node) = 0;
    virtual void visit(If_statement &node) = 0;
    virtual void visit(Else_statement &node) = 0;
    virtual void visit(For_statement &node) = 0;
    virtual void visit(For_condition &node) = 0;
    virtual void visit(For_operation &node) = 0;
    virtual void visit(Kw_statement &node) = 0;
    virtual void visit(Print_statement &node) = 0;
    virtual void visit(Read_statement &node) = 0;
};

}  // namespace csharp::ast
