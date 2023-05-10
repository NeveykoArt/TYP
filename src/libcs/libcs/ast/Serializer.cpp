#include <libcs/ast/Serializer.hpp>

#include <string>
#include <iostream>

namespace csharp::ast {

void Serializer::exec(Program &program, std::ostream &out) {
    Serializer xml_serializer;
    xml_serializer.nodes_.push(xml_serializer.doc_.append_child("csharp"));
    if (program.get_csclass() == nullptr) std::cout << "AAAAAAA\n";
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
    nodes_.pop();

    /*
    auto expressions = append_child("function");
    nodes_.push(classname);
    node.get_exprs()->accept(*this);
    nodes_.pop();
    */
}

pugi::xml_node Serializer::append_child(const char *name) {
    return nodes_.top().append_child(name);
}

void Serializer::append_text(const char *text) {
    auto text_node = nodes_.top().append_child(pugi::node_pcdata);
    text_node.set_value(text);
}

}  // namespace csharp::ast
