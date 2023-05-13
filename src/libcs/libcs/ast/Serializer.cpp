#include <libcs/ast/Serializer.hpp>

#include <string>
#include <iostream>

namespace csharp::ast {

void Serializer::exec(Program &program, std::ostream &out) {
    Serializer xml_serializer;
    xml_serializer.nodes_.push(xml_serializer.doc_.append_child("csharp"));
    program.get_csclass()->accept(xml_serializer);
    xml_serializer.doc_.save(out, "  ");
}

void Serializer::visit(CSClass &node) {
    auto classname = append_child("class");
    nodes_.push(classname);
        auto header = append_child("id");
        nodes_.push(header);
            append_text(node.get_csclass_name().c_str());
        nodes_.pop();
        node.get_expr()->accept(*this);
    nodes_.pop();
}

void Serializer::visit(Expression &node) {
    for (auto *c : node.get_exprs()) {
        c->get_elem()->accept(*this);
    }
}

void Serializer::visit(Func_def &node) {
    auto header = append_child("function");
    nodes_.push(header);
        auto typeheader = append_child("function_type");
        nodes_.push(typeheader);
            std::string space_ = " ";
            if (!node.get_kw().empty()) {
                for (auto &c : node.get_kw()) {
                    append_text(c.c_str());
                    append_text(space_.c_str());
                }
            }
            if (node.get_void()) {
                std::string stringvoid = "void";
                append_text(stringvoid.c_str());
            }
            if (!node.get_var().empty()) {
                append_text(node.get_var().c_str());
            }
        nodes_.pop();
        auto idheader = append_child("id");
        nodes_.push(idheader);
            append_text(node.get_func_name().c_str());
        nodes_.pop();
        node.get_pars()->accept(*this);
        node.get_scope()->accept(*this);
        node.get_return()->accept(*this);
    nodes_.pop();
}

void Serializer::visit(Read_statement &node) {
    auto header = append_child("readln");
    nodes_.push(header);
        auto idheader = append_child("id");
        nodes_.push(idheader);
            std::string readline = "Console.ReadLine";
            append_text(readline.c_str());
            node.get_read()->accept(*this);
        nodes_.pop();
    nodes_.pop();
}

void Serializer::visit(Print_statement &node) {
    auto header = append_child("println");
    nodes_.push(header);
        auto idheader = append_child("id");
        nodes_.push(idheader);
            std::string writeline = "Console.WriteLine";
            append_text(writeline.c_str());
            node.get_print()->accept(*this);
        nodes_.pop();
    nodes_.pop();
}

void Serializer::visit(Assign_statement &node) {
    auto header = append_child("assign");
    nodes_.push(header);
        auto leftside = append_child("leftside");
        nodes_.push(leftside);
        if (node.get_var_def() != nullptr) {
            node.get_var_def()->accept(*this);
        } else {
            auto arg = append_child("arg");
            nodes_.push(arg);
                auto idheader = append_child("id");
                nodes_.push(idheader);
                    append_text(node.get_id_left().c_str());
                nodes_.pop();
            nodes_.pop();
        }
        nodes_.pop();
        if (!node.get_arg_rigth().empty() || node.get_func_call() != nullptr || node.get_mas_selection() != nullptr) {
            auto rightside = append_child("rightside");
            nodes_.push(rightside);
            if (node.get_func_call() != nullptr) {
                node.get_func_call()->accept(*this);
            } else if (node.get_mas_selection() != nullptr) {
                node.get_mas_selection()->accept(*this);
            } else if (!node.get_bin_op().empty()) {
                auto binop = append_child("binary_op");
                nodes_.push(binop);
                auto op_typ = append_child("op_type");
                nodes_.push(op_typ);
                    append_text(node.get_bin_op().c_str());
                nodes_.pop();
                for (auto *c : node.get_arg_rigth()) {
                    c->accept(*this);
                }
                nodes_.pop();
            } else {
                for (auto *c : node.get_arg_rigth()) {
                    c->accept(*this);
                }
            }
            nodes_.pop();
        }
    nodes_.pop();
}

void Serializer::visit(Mas_statement &node) {
    auto header = append_child("mas_assign");
    nodes_.push(header);
        auto leftside = append_child("leftside");
        nodes_.push(leftside);
        if (node.get_mas_change() != nullptr) {
            node.get_mas_change()->accept(*this);
        } else {
            node.get_mas_def()->accept(*this);
        }
        nodes_.pop();
        if (!node.get_arg_rigth().empty() || node.get_func_call() != nullptr || node.get_mas_selection() != nullptr) {
            auto rightside = append_child("rightside");
            nodes_.push(rightside);
            if (node.get_func_call() != nullptr) {
                node.get_func_call()->accept(*this);
            } else if (node.get_mas_selection() != nullptr) {
                node.get_mas_selection()->accept(*this);
            } else if (!node.get_bin_op().empty()) {
                auto binop = append_child("binary_op");
                nodes_.push(binop);
                auto op_typ = append_child("op_type");
                nodes_.push(op_typ);
                    append_text(node.get_bin_op().c_str());
                nodes_.pop();
                for (auto *c : node.get_arg_rigth()) {
                    c->accept(*this);
                }
                nodes_.pop();
            } else {
                for (auto *c : node.get_arg_rigth()) {
                    c->accept(*this);
                }
            }
            nodes_.pop();
        }
    nodes_.pop();
}

void Serializer::visit(If_statement &node) {
    auto header = append_child("if");
    nodes_.push(header);
        auto roundbrackets = append_child("roundbrackets");
        nodes_.push(roundbrackets);
            auto leftside = append_child("leftside");
            nodes_.push(leftside);
                auto arg = append_child("arg");
                nodes_.push(arg);
                    auto id = append_child("id");
                    nodes_.push(id);
                        append_text(node.get_if_1_arg().c_str());
                    nodes_.pop();
                nodes_.pop();
            nodes_.pop();
            if (!node.get_if_log_op().empty()) {
                auto logop = append_child("logic_op");
                nodes_.push(logop);
                    append_text(node.get_if_log_op().c_str());
                nodes_.pop();
                auto rightside = append_child("rightside");
                nodes_.push(rightside);
                    if (node.get_if_2_arg() != nullptr) {
                        node.get_if_2_arg()->accept(*this);
                    }
                nodes_.pop();
            }
        nodes_.pop();
        auto braces = append_child("braces");
        nodes_.push(braces);
            node.get_if_scope()->accept(*this);
        nodes_.pop();
    nodes_.pop();
    if (node.get_if_else() != nullptr) {
        node.get_if_else()->accept(*this);
    }
}

void Serializer::visit(Else_statement &node) {
    auto header = append_child("Else");
    nodes_.push(header);
        auto braces = append_child("braces");
        nodes_.push(braces);
            node.get_else_scope()->accept(*this);
        nodes_.pop();
    nodes_.pop();
}

void Serializer::visit(For_statement &node) {
    auto header = append_child("for");
    nodes_.push(header);
        auto roundbrackets = append_child("roundbrackets");
        nodes_.push(roundbrackets);
            node.get_assign()->accept(*this);
            node.get_cond()->accept(*this);
            node.get_oper()->accept(*this);
        nodes_.pop();
        auto braces = append_child("braces");
        nodes_.push(braces);
            node.get_scope()->accept(*this);
        nodes_.pop();
    nodes_.pop();
}

void Serializer::visit(For_operation &node) {
    auto header = append_child("for_operation");
    nodes_.push(header);
    if (node.get_for_assign() == nullptr) {
        auto leftside = append_child("leftside");
        nodes_.push(leftside);
            auto header = append_child("arg");
            nodes_.push(header);
                append_text(node.get_for_id_var().c_str());
            nodes_.pop();
        nodes_.pop();
        auto logop = append_child("unary_op");
        nodes_.push(logop);
            append_text(node.get_for_unar_op().c_str());
        nodes_.pop();
    } else {
        node.get_for_assign()->accept(*this);
    }
    nodes_.pop();
}

void Serializer::visit(For_condition &node) {
    auto header = append_child("for_condition");
    nodes_.push(header);
        auto leftside = append_child("leftside");
        nodes_.push(leftside);
            auto arg = append_child("arg");
            nodes_.push(arg);
                append_text(node.get_for_cond_1_arg().c_str());
            nodes_.pop();
        nodes_.pop();
        auto logop = append_child("logop");
        nodes_.push(logop);
            append_text(node.get_for_cond_log_op().c_str());
        nodes_.pop();
        auto rightside = append_child("rightside");
        nodes_.push(rightside);
            node.get_for_cond_2_arg()->accept(*this);
        nodes_.pop();
    nodes_.pop();
}

void Serializer::visit(Pars &node) {
    auto header = append_child("pars");
    nodes_.push(header);
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
    nodes_.pop();
}

void Serializer::visit(Scope &node) {
    auto header = append_child("scope");
    nodes_.push(header);
    for (auto *c : node.get_args()) {
        c->get_statement_elem()->accept(*this);
    }
    nodes_.pop();
}

void Serializer::visit(Return_statement &node) {
    auto header = append_child("return");
    nodes_.push(header);
    if (node.get_return_arg() != nullptr) {
        node.get_return_arg()->accept(*this);
    }
    nodes_.pop();
}

void Serializer::visit(Var_def &node) {
    auto header = append_child("var_def");
    nodes_.push(header);
        auto typheader = append_child("type");
        nodes_.push(typheader);
            append_text(node.get_var_typ().c_str());
        nodes_.pop();
        auto idheader = append_child("id");
        nodes_.push(idheader);
            append_text(node.get_var_id().c_str());
        nodes_.pop();
    nodes_.pop();
}

void Serializer::visit(Mas_def &node) {
    auto header = append_child("var_def");
    nodes_.push(header);
        auto typheader = append_child("type");
        nodes_.push(typheader);
            append_text(node.get_var_typ().c_str());
            std::string br = "[]";
            append_text(br.c_str());
        nodes_.pop();
        auto idheader = append_child("id");
        nodes_.push(idheader);
            append_text(node.get_var_id().c_str());
        nodes_.pop();
    nodes_.pop();
}

void Serializer::visit(Mas_selection &node) {
    auto header = append_child("new");
    nodes_.push(header);
        auto typheader = append_child("type");
        nodes_.push(typheader);
            append_text(node.get_var_typ().c_str());
            if (node.get_length()!=nullptr) {
                auto sbraks = append_child("sq_brakets");
                nodes_.push(sbraks);
                    append_text(node.get_length()->get_id_number().c_str());
                nodes_.pop();
            } else {
                std::string br = "[]";
                append_text(br.c_str());
            }
        nodes_.pop();
        if (!node.get_literal().empty()) {
            auto braces = append_child("braces");
            nodes_.push(braces);
                for (auto *c : node.get_literal()) {
                    c->accept(*this);
                }
            nodes_.pop();
        }
    nodes_.pop();
}

void Serializer::visit(Mas_change &node) {
    auto header = append_child("arg");
    nodes_.push(header);
        auto typheader = append_child("id");
        nodes_.push(typheader);
            append_text(node.get_change_id().c_str());
        nodes_.pop();
        auto idheader = append_child("at");
        nodes_.push(idheader);
            if (node.get_at()!=nullptr) {
                auto sbraks = append_child("sq_brakets");
                nodes_.push(sbraks);
                    append_text(node.get_at()->get_id_number().c_str());
                nodes_.pop();
            } else {
                std::string br = "[]";
                append_text(br.c_str());
            }
        nodes_.pop();
    nodes_.pop();
}

void Serializer::visit(Func_call &node) {
    auto header = append_child("func_call");
    nodes_.push(header);
        auto idheader = append_child("id");
        nodes_.push(idheader);
            append_text(node.get_func_call_name().c_str());
        nodes_.pop();
        auto roundbrackets = append_child("roundbrackets");
        nodes_.push(roundbrackets);
            node.get_args()->accept(*this);
        nodes_.pop();
    nodes_.pop();
}

void Serializer::visit(Arguments &node) {
    for (auto *c : node.get_args()) {
        c->accept(*this);
    }
}

void Serializer::visit(Arg &node) {
    if (node.get_arg() != nullptr) {
        auto header = append_child("arg");
        nodes_.push(header);
            node.get_arg()->accept(*this);
         nodes_.pop();
    } else if (!node.get_arg_id().empty()){
        auto header = append_child("arg");
        nodes_.push(header);
            auto idheader = append_child("id");
            nodes_.push(idheader);
                append_text(node.get_arg_id().c_str());
            nodes_.pop();
        nodes_.pop();
    } else {
        node.get_arg_mas()->accept(*this);
    }
}

void Serializer::visit(Literal &node) {
    auto header = append_child("literal");
    nodes_.push(header);
    if (!node.get_literal().empty()) {
        append_text(node.get_literal().c_str());
    } else {
        if (node.get_bool_flag()) {
            std::string t = "true";
            append_text(t.c_str());
        } else {
            std::string f = "false";
            append_text(f.c_str());
        }
    }
    nodes_.pop();
}

pugi::xml_node Serializer::append_child(const char *name) {
    return nodes_.top().append_child(name);
}

void Serializer::append_text(const char *text) {
    auto text_node = nodes_.top().append_child(pugi::node_pcdata);
    text_node.set_value(text);
}

}  // namespace csharp::ast
