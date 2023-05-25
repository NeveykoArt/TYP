#pragma once

#include <libcs/ast/Visitor.hpp>
#include <vector>

namespace csharp::ast {

enum class TableType { Function, Variable, Array, Class, Scope };

class Symbol final {
 private:
  TableType table_type_;
  std::string obj_name_;
  std::string var_type_;
  std::size_t scope_level_;

 public:
  Symbol(
      TableType table_typ,
      std::string obj_name,
      std::string var_typ,
      std::size_t scope_level)
      : table_type_(table_typ),
        obj_name_(obj_name),
        var_type_(var_typ),
        scope_level_(scope_level) {}
  std::string get_table_typ() {
    switch (table_type_) {
      case TableType::Function:
        return "function";
      case TableType::Variable:
        return "variable";
      case TableType::Array:
        return "array";
      case TableType::Class:
        return "class";
      case TableType::Scope:
        return "Scope";
      default:
        return "-";
    }
  }
  std::string get_var_typ() const { return var_type_; }
  std::string get_obj_name() const { return obj_name_; }
  std::size_t get_scope_level() const { return scope_level_; }
};

class SemanticError final : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

using SymbolTable = std::vector<Symbol>;

class SymbolTableVisitor final : public Visitor {
 private:
  SymbolTable global_table_;

 public:
  static SymbolTable exec(Program& program);

  void visit(CSClass& node) override;
  void visit(Expression& node) override;
  void visit([[maybe_unused]] Expressions& node) override {}
  void visit(Mas_statement& node) override;
  void visit(Assign_statement& node) override;
  void visit([[maybe_unused]] Literal& node) override {}
  void visit(Mas_def& node) override;
  void visit(Mas_selection& node) override;
  void visit(Mas_change& node) override;
  void visit(Var_def& node) override;
  void visit(Func_def& node) override;
  void visit(Scope& node) override;
  void visit(Return_statement& node) override;
  void visit([[maybe_unused]] Statement& node) override {}
  void visit(Func_call& node) override;
  void visit(Arguments& node) override;
  void visit(Arg& node) override;
  void visit(Pars& node) override;
  void visit(If_statement& node) override;
  void visit(Else_statement& node) override;
  void visit(For_statement& node) override;
  void visit(For_condition& node) override;
  void visit(For_operation& node) override;
  void visit(Length& node) override;
  void visit([[maybe_unused]] Kw_statement& node) override {}
  void visit(Print_statement& node) override;
  void visit(Read_statement& node) override;

  SymbolTable get_symbol_table() { return global_table_; };
  std::size_t getScopeLvl();
  bool contains(const std::string& elem, std::size_t& scop_lvl);
  std::string functype();
  std::string getTypeOf(const std::string& elem);
};

}  // namespace csharp::ast
