#include <libcs/ast/SymbolTable.hpp>
#include <iostream>

namespace csharp::ast {

std::size_t SymbolTableVisitor::getScopeLvl() {
    for (auto el = global_table_.rbegin(); el != global_table_.rend(); ++el) {
        if ((el->get_table_typ() == "Scope") && (el->get_obj_name() == "--------")) {
            return (el->get_scope_level() - 1);
        }
        if ((el->get_table_typ() == "Scope") && (el->get_obj_name() != "--------")){
            return (el->get_scope_level());
        }
    }
    return 0;
}

bool SymbolTableVisitor::contains(const std::string &elem, std::size_t &scop_lvl) {
    auto sl = scop_lvl;
    for (auto el = global_table_.rbegin(); el != global_table_.rend(); ++el) {
        if (el->get_obj_name() == elem && el->get_scope_level() == sl) {
            return true;
        }
        if (el->get_scope_level() < sl || ((el->get_table_typ() == "Scope") && (el->get_obj_name() == "--------") && (el->get_scope_level() == sl))) {
            --sl;
        }
    }
    return false;
}

SymbolTable SymbolTableVisitor::exec(Program &program) {
    SymbolTableVisitor table_visitor;
    program.get_csclass()->accept(table_visitor);
    return table_visitor.get_symbol_table();
}

void SymbolTableVisitor::visit(CSClass &node) {
    std::size_t scope_lvl = 0;
    global_table_.push_back(Symbol(TableType::Class, node.get_csclass_name(),"-" , scope_lvl));
    global_table_.push_back(Symbol(TableType::Scope, node.get_csclass_name(),"-" , scope_lvl));
    node.get_expr()->accept(*this);
    global_table_.push_back(Symbol(TableType::Scope, "--------", "--------" , scope_lvl));
}

void SymbolTableVisitor::visit(Expression &node) {
    for (auto *c : node.get_exprs()) {
        c->get_elem()->accept(*this);
    }
}

void SymbolTableVisitor::visit(Func_def &node) {
    std::size_t scope_lvl = getScopeLvl();
    std::string funcname = node.get_func_name();
    if (contains(funcname, scope_lvl)) {
        throw SemanticError(
            "Repeat declaration of class '" + funcname + "'");
    }
    if (node.get_void()) {
        std::string stringvoid = "void";
        global_table_.push_back(Symbol(TableType::Function, funcname, "void", scope_lvl));
    }
    if (!node.get_var().empty()) {
        global_table_.push_back(Symbol(TableType::Function, funcname, node.get_var(), scope_lvl));
    }
    global_table_.push_back(Symbol(TableType::Scope, funcname, "-", scope_lvl+1));
    node.get_pars()->accept(*this);
    node.get_scope()->accept(*this);
    global_table_.push_back(Symbol(TableType::Scope, "--------", "--------", scope_lvl+1));
}

void SymbolTableVisitor::visit(Read_statement &node) {
    node.get_read()->accept(*this);
}

void SymbolTableVisitor::visit(Print_statement &node) {
    node.get_print()->accept(*this);
}

void SymbolTableVisitor::visit(Assign_statement &node) {
    if (node.get_var_def() != nullptr) {
        node.get_var_def()->accept(*this);
    } else {
        std::string id = node.get_id_left();
        std::size_t scope_lvl = getScopeLvl();
        if (!contains(id, scope_lvl)) {
            throw SemanticError(
                "Not declarated variable '" + id + "'");
        }
    }
    if (node.get_func_call() != nullptr) {
        node.get_func_call()->accept(*this);
    }
    if (!node.get_arg_rigth().empty()) {
        for (auto *c : node.get_arg_rigth()) {
            c->accept(*this);
        }
    }
}

void SymbolTableVisitor::visit(Mas_statement &node) {
    if (node.get_mas_change() != nullptr) {
        node.get_mas_change()->accept(*this);
    } else {
        node.get_mas_def()->accept(*this);
    }
    if (node.get_func_call() != nullptr) {
        node.get_func_call()->accept(*this);
    }
    if (!node.get_arg_rigth().empty()) {
        for (auto *c : node.get_arg_rigth()) {
            c->accept(*this);
        }
    }
}

void SymbolTableVisitor::visit(If_statement &node) {
    std::size_t scope_lvl = getScopeLvl();
    std::string id = node.get_if_1_arg();
    if (!contains(id, scope_lvl)) {
        throw SemanticError(
            "Not declarated variable '" + id + "'");
    }
    if (!node.get_if_log_op().empty()) {
        node.get_if_2_arg()->accept(*this);
    }
    global_table_.push_back(Symbol(TableType::Scope, "if", "-", scope_lvl+1));
    node.get_if_scope()->accept(*this);
    global_table_.push_back(Symbol(TableType::Scope, "--------", "--------", scope_lvl+1));
    if (node.get_if_else() != nullptr) {
        node.get_if_else()->accept(*this);
    }
}

void SymbolTableVisitor::visit(Else_statement &node) {
    std::size_t scope_lvl = getScopeLvl();
    global_table_.push_back(Symbol(TableType::Scope, "else", "-", scope_lvl+1));
    node.get_else_scope()->accept(*this);
    global_table_.push_back(Symbol(TableType::Scope, "--------", "--------", scope_lvl+1));
}

void SymbolTableVisitor::visit(For_statement &node) {
    std::size_t scope_lvl = getScopeLvl();
    global_table_.push_back(Symbol(TableType::Scope, "for", "-", scope_lvl+1));
    node.get_assign()->accept(*this);
    node.get_cond()->accept(*this);
    node.get_oper()->accept(*this);
    node.get_scope()->accept(*this);
    global_table_.push_back(Symbol(TableType::Scope, "--------", "--------", scope_lvl+1));
}

void SymbolTableVisitor::visit(For_operation &node) {
    if (node.get_for_assign() == nullptr) {
        std::string id = node.get_for_id_var();
        std::size_t scope_lvl = getScopeLvl();
        if (!contains(id, scope_lvl)) {
            throw SemanticError(
                "Not declarated variable '" + id + "'");
        }
    } else {
        node.get_for_assign()->accept(*this);
    }
}

void SymbolTableVisitor::visit(For_condition &node) {
    std::string id = node.get_for_cond_1_arg();
    std::size_t scope_lvl = getScopeLvl();
        if (!contains(id, scope_lvl)) {
        throw SemanticError(
            "Not declarated variable '" + id + "'");
    }
    node.get_for_cond_2_arg()->accept(*this);
}

void SymbolTableVisitor::visit(Pars &node) {
    if (!node.get_pars_var().empty()) {
        for (auto *c : node.get_pars_var()) {
            c->accept(*this);
        }
    }
    if (!node.get_pars_mas().empty()) {
        for (auto *c : node.get_pars_mas()) {
            c->accept(*this);
        }
    }
}

void SymbolTableVisitor::visit(Scope &node) {
    for (auto *c : node.get_args()) {
        c->get_statement_elem()->accept(*this);
    }
}

void SymbolTableVisitor::visit(Return_statement &node) {
    if (node.get_return_arg() != nullptr) {
        node.get_return_arg()->accept(*this);
    }
}

void SymbolTableVisitor::visit(Var_def &node) {
    std::size_t scope_lvl = getScopeLvl();
    std::string id = node.get_var_id();
    if (contains(id, scope_lvl)) {
        throw SemanticError(
            "Repeat declaration of variable '" + id + "'");
    }
    global_table_.push_back(Symbol(TableType::Variable, id, node.get_var_typ(), scope_lvl));
}

void SymbolTableVisitor::visit(Mas_def &node) {
    std::size_t scope_lvl = getScopeLvl();
    std::string id = node.get_var_id();
    if (contains(id, scope_lvl)) {
        throw SemanticError(
            "Repeat declaration of massive '" + id + "'");
    }
    global_table_.push_back(Symbol(TableType::Array, id, node.get_var_typ(), scope_lvl));
}

void SymbolTableVisitor::visit(Mas_change &node) {
    std::string id = node.get_change_id();
    std::size_t scope_lvl = getScopeLvl();
    if (!contains(id, scope_lvl)) {
        throw SemanticError(
            "Not declarated massive '" + id + "'");
    }
    node.get_at()->accept(*this);
}

void SymbolTableVisitor::visit(Length &node) {
    std::string id = node.get_id_number();
    std::size_t scope_lvl = getScopeLvl();
    if (!contains(id, scope_lvl)) {
        throw SemanticError(
            "Not declarated var '" + id + "'");
    }
}

void SymbolTableVisitor::visit(Func_call &node) {
    std::string funcname = node.get_func_call_name();
    std::size_t scope_lvl = getScopeLvl();
    if (!contains(funcname, scope_lvl)) {
        throw SemanticError(
            "Not declarated of function '" + funcname + "'");
    }
}

void SymbolTableVisitor::visit(Arguments &node) {
    for (auto *c : node.get_args()) {
        c->accept(*this);
    }
}

void SymbolTableVisitor::visit(Arg &node) {
    if (node.get_arg() != nullptr) {
        node.get_arg()->accept(*this);
    } else if (!node.get_arg_id().empty()){
        std::string id = node.get_arg_id();
        std::size_t scope_lvl = getScopeLvl();
        if (!contains(id, scope_lvl)) {
            throw SemanticError(
                "Not declarated arg '" + id + "'");
        }
    } else {
        node.get_arg_mas()->accept(*this);
    }
}

} //namespace csharp::ast
