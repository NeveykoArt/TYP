#pragma once

#include <libcs/ast/SymbolTable.hpp>
#include <libcs/ast/Visitor.hpp>

#include <ostream>
#include <sstream>

namespace csharp::ast {

class CodeGenerator final : public Visitor {
private:
    SymbolTable symbol_table_;
    std::stringstream string_stream_;

    void generate_file(std::ostream &out);

public:
    CodeGenerator(SymbolTable &symbol_table)
    : symbol_table_(symbol_table) {}

    static void exec(Program &program, SymbolTable &symbol_table, std::ostream &out);

    void visit(CSClass &node) override;
    void visit(Expression &node) override;
    void visit([[maybe_unused]]Expressions &node) override {}
    void visit([[maybe_unused]]Mas_statement &node) override {}
    void visit([[maybe_unused]]Assign_statement &node) override {}
    void visit(Literal &node) override;
    void visit([[maybe_unused]]Mas_def &node) override {}
    void visit([[maybe_unused]]Mas_selection &node) override {}
    void visit([[maybe_unused]]Mas_change &node) override {}
    void visit([[maybe_unused]]Var_def &node) override {}
    void visit(Func_def &node) override;
    void visit(Scope &node) override;
    void visit(Return_statement &node) override;
    void visit([[maybe_unused]]Statement &node) override {}
    void visit(Func_call &node) override;
    void visit([[maybe_unused]]Arguments &node) override {}
    void visit(Arg &node) override;
    void visit(Pars &node) override;
    void visit([[maybe_unused]]If_statement &node) override {}
    void visit([[maybe_unused]]Else_statement &node) override {}
    void visit([[maybe_unused]]For_statement &node) override {}
    void visit([[maybe_unused]]For_condition &node) override {}
    void visit([[maybe_unused]]For_operation &node) override {}
    void visit([[maybe_unused]]Length &node) override {}
    void visit([[maybe_unused]]Kw_statement &node) override {}
    void visit(Print_statement &node) override;
    void visit(Read_statement &node) override;
};

} // namespace csharp::ast
