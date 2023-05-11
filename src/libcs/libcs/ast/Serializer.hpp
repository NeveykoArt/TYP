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
    void visit([[maybe_unused]]Expressions &node) override{}
    void visit([[maybe_unused]]Assign_statement &node) override{}
    void visit([[maybe_unused]]Literal &node) override{}
    void visit([[maybe_unused]]Var_def &node) override{}
    void visit([[maybe_unused]]Func_def &node) override{}
    void visit([[maybe_unused]]Scope &node) override {}
    void visit([[maybe_unused]]Return_statement &node) override {}
    void visit([[maybe_unused]]Statement &node) override {}
    void visit([[maybe_unused]]Func_call &node) override {}
    void visit([[maybe_unused]]Arguments &node) override {}
    void visit([[maybe_unused]]Arg &node) override {}
    void visit([[maybe_unused]]Pars &node) override {}
    void visit([[maybe_unused]]If_statement &node) override {}
    void visit([[maybe_unused]]Else_statement &node) override {}
    void visit([[maybe_unused]]For_statement &node) override {}
    void visit([[maybe_unused]]For_condition &node) override {}
    void visit([[maybe_unused]]For_operation &node) override {}
    void visit([[maybe_unused]]Kw_statement &node) override {}
    void visit([[maybe_unused]]Print_statement &node) override {}
    void visit([[maybe_unused]]Read_statement &node) override {}
};

}  // namespace csharp::ast
