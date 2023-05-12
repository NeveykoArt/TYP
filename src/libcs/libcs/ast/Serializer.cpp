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
    std::cout << "GDE1\n";
    node.get_expr()->accept(*this);

    nodes_.pop();
}

void Serializer::visit(Expression &node) {
    std::cout << "GDE2\n";
    for (auto *c : node.get_exprs()) {
        c->get_elem()->accept(*this);
    }
}

void Serializer::visit(Func_def &node) {
    std::cout << "GDE3\n";
    auto header = append_child("function");
    nodes_.push(header);

    auto idheader = append_child("id");
    nodes_.push(idheader);

    if (!node.get_kw().empty()) {
        for (auto &c : node.get_kw()) {
            append_text(c.c_str());
        }
    }
    if (node.get_void()) {
        std::string stringvoid = "void";
        append_text(stringvoid.c_str());
    }
    if (!node.get_var().empty()) {
        append_text(node.get_var().c_str());
    }

    append_text(node.get_func_name().c_str());


    node.get_pars()->accept(*this);

    nodes_.pop();

    node.get_scope()->accept(*this);
    node.get_return()->accept(*this);

    nodes_.pop();
}

void Serializer::visit(Read_statement &node) {
    std::cout << "GDE4\n";
    auto header = append_child("readln");
    nodes_.push(header);
    auto idheader = append_child("id");
    nodes_.push(idheader);
    std::string readline = "ReadLine";
    append_text(readline.c_str());
    auto argheader = append_child("arg");
    nodes_.push(argheader);
    append_text(node.get_read().c_str());
    nodes_.pop();
    nodes_.pop();
    nodes_.pop();
}

void Serializer::visit(Print_statement &node) {
    std::cout << "GDE5\n";
    auto header = append_child("println");
    nodes_.push(header);
    auto idheader = append_child("id");
    nodes_.push(idheader);
    std::string writeline = "WriteLine";
    append_text(writeline.c_str());
    auto argheader = append_child("arg");
    nodes_.push(argheader);
    append_text(node.get_print().c_str());
    nodes_.pop();
    nodes_.pop();
    nodes_.pop();
}

void Serializer::visit(Assign_statement &node) {
    std::cout << "GDE6\n";
    auto header = append_child("assign");
    nodes_.push(header);

    auto leftside = append_child("leftside");
    nodes_.push(leftside);
    if (node.get_var_def() != nullptr) {
         node.get_var_def()->accept(*this);
    } else {
        append_text(node.get_id_left().c_str());
    }
    nodes_.pop();

    auto rightside = append_child("rightside");
    nodes_.push(rightside);
    if (node.get_func_call() != nullptr) {
        node.get_func_call()->accept(*this);
    } else {
        auto binop = append_child("binop");
        nodes_.push(binop);

        auto op_typ = append_child("op_typ");
        nodes_.push(op_typ);
        append_text(node.get_bin_op().c_str());
        nodes_.pop();

        for (auto *c : node.get_arg_rigth()) {
            c->accept(*this);
        }
        nodes_.pop();
    }
    nodes_.pop();

    nodes_.pop();
}

void Serializer::visit(If_statement &node) {
    std::cout << "GDE7\n";
    auto header = append_child("if");
    nodes_.push(header);

    auto roundbrackets = append_child("roundbrackets");
    nodes_.push(roundbrackets);
    append_text(node.get_if_1_arg().c_str());
    if (node.get_if_2_arg() != nullptr) {
        append_text(node.get_if_log_op().c_str());
        node.get_if_2_arg()->accept(*this);
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

void Serializer::visit(For_statement &node) {
    std::cout << "GDE8\n";
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

void Serializer::visit(Pars &node) {
    std::cout << "GDE8\n";
    auto header = append_child("pars");
    nodes_.push(header);

    for (auto *c : node.get_pars()) {
        c->accept(*this);
    }

    nodes_.pop();
}

void Serializer::visit(Scope &node) {
    std::cout << "GDE9\n";
    auto header = append_child("scope");
    nodes_.push(header);
    for (auto *c : node.get_args()) {
        c->get_statement_elem()->accept(*this);
    }
    nodes_.pop();
}

void Serializer::visit(Return_statement &node) {
    std::cout << "GDE10\n";
    auto header = append_child("return");
    nodes_.push(header);
    node.get_return_arg()->accept(*this);
    nodes_.pop();
}

void Serializer::visit(Var_def &node) {
    std::cout << "GDE11\n";
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

void Serializer::visit(Func_call &node) {
    std::cout << "GDE12\n";
    auto header = append_child("func_call");
    nodes_.push(header);
    std::cout << "GDE12-----1\n";
    auto idheader = append_child("id");
    std::cout << "GDE12-----11\n";
    nodes_.push(idheader);
    std::cout << "GDE12-----12\n";
    std::cout << nodes_.size() << "\n";
    std::cout << node.get_func_call_name() << "\n";
    std::cout << "---------------------\n";
    append_text(node.get_func_call_name().c_str());
    std::cout << "GDE12-----13\n";
    nodes_.pop();
    std::cout << "GDE12-----2\n";
    auto roundbrackets = append_child("roundbrackets");
    nodes_.push(roundbrackets);
    std::cout << "GDE12-----3\n";
    node.get_args()->accept(*this);

    nodes_.pop();

    nodes_.pop();
}

void Serializer::visit(Arguments &node) {
    std::cout << "GDE13\n";
    for (auto *c : node.get_args()) {
        c->accept(*this);
    }
}

void Serializer::visit(Arg &node) {
    auto header = append_child("arg");
    nodes_.push(header);

    if (node.get_arg() != nullptr) {
        node.get_arg()->accept(*this);
    } else {
        auto idheader = append_child("id");
        nodes_.push(idheader);
        append_text(node.get_arg_id().c_str());
        nodes_.pop();
    }

    nodes_.pop();
}

void Serializer::visit(Literal &node) {
    std::cout << "GDE14\n";
    auto header = append_child("literal");
    nodes_.push(header);

    append_text(node.get_literal().c_str());

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
