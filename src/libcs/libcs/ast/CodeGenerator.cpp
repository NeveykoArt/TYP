#include <libcs/ast/CodeGenerator.hpp>

#include <algorithm>
#include <string_view>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <map>

namespace csharp::ast {

std::size_t print_counter = 0;
std::size_t scanf_counter = 0;
std::string current_function = "Global";
std::size_t index = 0;
std::map<std::string, std::pair<std::string, std::string>> name_index;
std::vector<std::string> name_index_globals;
std::map<std::string, std::pair<std::string, std::string>> name_index_params;
std::map<std::string, std::pair<std::string, std::string>> name_index_scope;
int amount_new = 0;
std::vector<std::string> name_index_scope_new;
std::map<std::string, std::string> global_strs;
bool is_inner_scope = false;

std::vector<std::pair<std::string, std::string>> stack;

static std::string new_index()
{
    std::string tmp = "";
    tmp = "%v" + std::to_string(index);
    ++index;
    return tmp;
}

static std::string get_llvm_typ(std::string p) {
    std::string ptype = "";
    if (p == "int")
    {
        ptype = "i32";
    }
    else if (p == "char")
    {
        ptype = "i8";
    }
    else if (p == "float")
    {
        ptype = "double";
    }
    else if (p == "void")
    {
        ptype = "void";
    }
    return ptype;
}
/*
static std::string new_global_index()
{
    std::string tmp = "@g" + std::to_string(index);
    index++;
    return tmp;
}

static std::string get_llvm_op(std::string op, std::string type) {
    std::string ret_op = "";
    if (op == "+")
    {
        if (type == "int" || type == "i32")
        {
            ret_op = "add";
        }
        if (type == "float" || type == "double")
        {
            ret_op = "fadd";
        }
    }
    else if (op == "-")
    {
        if (type == "int" || type == "i32")
        {
            ret_op = "sub";
        }
        if (type == "float" || type == "double")
        {
            ret_op = "fsub";
        }
    }
    else if (op == "/")
    {
        if (type == "int" || type == "i32")
        {
            ret_op = "sdiv";
        }
        if (type == "float" || type == "double")
        {
            ret_op = "fdiv";
        }
    }
    else if (op == "%")
    {
        if (type == "float" || type == "double")
        {
            std::cerr << "ERROR: invalid operands to binary expression ('double' and 'double')\n";
        }
        ret_op = "srem";
    }
    else if (op == "*")
    {
        if (type == "int" || type == "i32")
        {
            ret_op = "mul";
        }
        if (type == "float" || type == "double")
        {
            ret_op = "fmul";
        }
    }
    return ret_op;
}

static std::string get_log_op_int(std::string op) {
    std::string lo = "";
    if (op == ">")
    {
        lo = "sgt";
    }
    else if (op == ">=")
    {
        lo = "sge";
    }
    else if (op == "<=")
    {
        lo = "sle";
    }
    else if (op == "<")
    {
        lo = "slt";
    }
    else if (op == "==")
    {
        lo = "eq";
    }
    else if (op == "!=")
    {
        lo = "ne";
    }
    return lo;
}

static std::string get_log_op_double(std::string op) {
    std::string lo = "";
    if (op == ">")
    {
        lo = "ogt";
    }
    else if (op == ">=")
    {
        lo = "oge";
    }
    else if (op == "<=")
    {
        lo = "ole";
    }
    else if (op == "<")
    {
        lo = "olt";
    }
    else if (op == "==")
    {
        lo = "oeq";
    }
    else if (op == "!=")
    {
        lo = "une";
    }
    return lo;
}
*/
void CodeGenerator::generate_file(std::ostream &out) {
    out << "target datalayout = \"e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128\"\n";
    out << "target triple = \"x86_64-redhat-linux-gnu\"\n\n";
    if (print_counter > 0) {
        out << "declare dso_local i32 @printf(i8*, ...) #1\n";
    }
    if (scanf_counter > 0) {
        out << "declare dso_local i32 @__isoc99_scanf(i8*, ...) #1\n";
    }
    if (!global_strs.empty()) {
        for (auto &item : global_strs) {
            out << item.first << " = private unnamed_addr constant [" << item.second.size()+1 << " x i8] c\"" << item.second <<"\\00\", align 1\n";
        }
    }
    out << string_stream_.str();

    out << "attributes #0 = { noinline nounwind optnone uwtable \"frame-pointer\"=\"all\" \"min-legal-vector-width\"=\"0\" \"no-trapping-math\"=\"true\" \"stack-protector-buffer-size\"=\"8\" \"target-cpu\"=\"x86-64\" \"target-features\"=\"+cx8,+fxsr,+mmx,+sse,+sse2,+x87\" \"tune-cpu\"=\"generic\" }\n";
    out << "attributes #1 = { \"frame-pointer\"=\"all\" \"no-trapping-math\"=\"true\" \"stack-protector-buffer-size\"=\"8\" \"target-cpu\"=\"x86-64\" \"target-features\"=\"+cx8,+fxsr,+mmx,+sse,+sse2,+x87\" \"tune-cpu\"=\"generic\" }\n";
    out << "attributes #2 = { argmemonly nofree nounwind willreturn }\n";
}

void CodeGenerator::exec(
    Program &program, SymbolTable &symbol_table, std::ostream &out) {
    CodeGenerator code_generator(symbol_table);
    program.get_csclass()->accept(code_generator);
    code_generator.generate_file(out);
}

void CodeGenerator::visit(CSClass &node) {
    node.get_expr()->accept(*this);
}

void CodeGenerator::visit(Expression &node) {
    for (auto *c : node.get_exprs()) {
        c->get_elem()->accept(*this);
    }
}

void CodeGenerator::visit(Func_def &node) {
    std::string typ_tmp;
    if (node.get_void()) {
        typ_tmp = "void";
    } else {
        typ_tmp = node.get_var();
    }
    string_stream_ << "define " << get_llvm_typ(typ_tmp) << " @" << node.get_func_name();
    // PARAMS
    string_stream_ << "(";
    if (!node.get_pars()->get_pars_var().empty()) {
        node.get_pars()->accept(*this);
    }
    string_stream_ << ") #0 {\n";

    if (!node.get_pars()->get_pars_var().empty()) {
        for (auto &item : name_index_params) {
            auto ind = new_index();
            auto old_ind = name_index[item.first].first;
            name_index[item.first].first = ind;
            string_stream_ << ind << " = alloca " << name_index[item.first].second << "\n";
            string_stream_ << "store " << name_index[item.first].second << " " << old_ind << ", " << name_index[item.first].second << "* " << ind << "\n";
        }
    }
    // SCOPE
    node.get_scope()->accept(*this);
    // RETURN
    if (typ_tmp != "void") {
        node.get_return()->accept(*this);
    } else {
        string_stream_ << "ret void\n";
    }

    string_stream_ << "}\n\n";
}

void CodeGenerator::visit(Pars &node) {
    if (!node.get_pars_var().empty()) {
        std::size_t i = 0;
        for (auto *c : node.get_pars_var()) {
            auto ptype = get_llvm_typ(c->get_var_typ());

            name_index[c->get_var_id()] = std::make_pair(new_index(), ptype);
            string_stream_ << ptype << " " << name_index[c->get_var_id()].first;
            name_index_params[c->get_var_id()] = std::make_pair(new_index(), ptype);
            if (i < node.get_pars_var().size() - 1) {
                string_stream_ << ", ";
            }
            i++;
        }
    }
}

void CodeGenerator::visit(Scope &node) {
    std::cout << "AAAAA1\n";
    for (auto &c : node.get_args()) {
        std::cout << "AAAAA3\n";
        c->get_statement_elem()->accept(*this);
    }
}

void CodeGenerator::visit(Func_call &node) {
    std::cout << "AAAAA7!!!!!!!\n";
    std::cout << node.get_func_call_name() << "\n";
}

void CodeGenerator::visit(Return_statement &node) {
    std::string tmp_return = new_index();
    std::string ret_typ;
    if (node.get_return_arg()->get_arg() != nullptr) {
        ret_typ = node.get_return_arg()->get_arg()->get_var_typ();
        ret_typ = get_llvm_typ(ret_typ);
        string_stream_ << "ret " << ret_typ << " " << node.get_return_arg()->get_arg()->get_literal() << '\n';
    } else if (node.get_return_arg()->get_arg_id() != "") {
        ret_typ = name_index[node.get_return_arg()->get_arg_id()].second;
        auto ind = name_index[node.get_return_arg()->get_arg_id()].first;
        string_stream_ << tmp_return << " = load " << ret_typ << ", " << ret_typ << "* " << ind << "\n";
        string_stream_ << "ret " << ret_typ << " " << tmp_return << '\n';
    }
}

void CodeGenerator::visit(Print_statement &node) {
    std::cout << "AAAAA2\n";
    print_counter++;
    node.get_print()->accept(*this);

    string_stream_ << new_index()
            << " = call i32 (i8*, ...) @printf(i8* getelementptr ("
            << stack.back().second << ", " << stack.back().second + "* "
            << stack.back().first << ", i64 0, i64 0)\n";
    stack.pop_back();
}

void CodeGenerator::visit(Read_statement &node) {
    scanf_counter++;
    node.get_read()->accept(*this);
    string_stream_ << new_index()
            << " = call i32 (i8*, ...) @scanf(i8* getelementptr ("
            << stack.back().second << "* " << stack.back().first << ")\n";
    stack.pop_back();
}

void CodeGenerator::visit(Arg &node) {
    if (node.get_arg() != nullptr) {
        node.get_arg()->accept(*this);
    }
    if (node.get_arg_mas() != nullptr) {
        node.get_arg_mas()->accept(*this);
    }
    if (!node.get_arg_id().empty()) {
        stack.push_back(std::make_pair(name_index[node.get_arg_id()].first, name_index[node.get_arg_id()].second));
    }
}

void CodeGenerator::visit(Literal &node) {
    std::string idx = std::to_string(index++);
    std::string tmp_str = node.get_literal();
    tmp_str.erase(std::remove(tmp_str.begin(), tmp_str.end(), '\"'), tmp_str.end());
    global_strs["@.str." + idx] = tmp_str;
    idx = new_index();
    string_stream_ << idx <<
    stack.push_back(std::make_pair(new_index(), "i8"));
}

} // namespace csharp::ast
