#pragma once

#include <libcs/ast/Visitor.hpp>

#include <pugixml.hpp>

#include <ostream>
#include <stack>
#include <any>
#include <fstream>
#include <iostream>

namespace csharp::ast {

class Serializer final : public Visitor {
private:
    pugi::xml_node append_child(const char *name);
    void append_text(const char *text);

    pugi::xml_document doc_;
    std::stack<pugi::xml_node> nodes_;

public:
    static void exec(Program &program, std::ostream &out);
    void visit(CSClass &node) override;
    void visit(Expression &node) override;
    void visit([[maybe_unused]]Expressions &node) override {}
    void visit(Mas_statement &node) override;
    void visit(Assign_statement &node) override;
    void visit(Literal &node) override;
    void visit(Mas_def &node) override;
    void visit(Mas_selection &node) override;
    void visit(Mas_change &node) override;
    void visit(Var_def &node) override;
    void visit(Func_def &node) override;
    void visit(Scope &node) override;
    void visit(Return_statement &node) override;
    void visit([[maybe_unused]]Statement &node) override {}
    void visit(Func_call &node) override;
    void visit(Arguments &node) override;
    void visit(Arg &node) override;
    void visit(Pars &node) override;
    void visit(If_statement &node) override;
    void visit(Else_statement &node) override;
    void visit(For_statement &node) override;
    void visit(For_condition &node) override;
    void visit(For_operation &node) override;
    void visit([[maybe_unused]]Kw_statement &node) override {}
    void visit(Print_statement &node) override;
    void visit(Read_statement &node) override;
};

}  // namespace csharp::ast
