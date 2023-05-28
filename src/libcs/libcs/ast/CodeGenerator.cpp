#include <libcs/ast/CodeGenerator.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace csharp::ast {

std::size_t index = 0;
std::size_t block_index = 0;
std::map<std::string, std::pair<std::string, std::string>> name_index_params;
std::size_t print_counter = 0;
std::size_t scanf_counter = 0;
std::size_t for_flag = 0;
std::map<std::string, std::string> global_strs;
std::map<std::string, std::pair<std::string, std::string>> variables_tab;
std::size_t str_index = 8;
std::vector<std::pair<std::string, std::pair<std::string, std::string>>> stack;
std::vector<std::string> operation_stack;
std::size_t common_index = 0;
std::size_t memset_flag = 0;
std::size_t new_flag = 0;
std::size_t slevel = 0;

static std::string new_index() {
  std::string tmp;
  tmp = "%v" + std::to_string(index);
  ++index;
  return tmp;
}

static std::string get_llvm_typ(const std::string& p) {
  std::string ptype;
  if (p == "int") {
    ptype = "i32";
  } else if (p == "char") {
    ptype = "i8";
  } else if (p == "float") {
    ptype = "float";
  } else if (p == "string") {
    ptype = "i8*";
  } else if (p == "void") {
    ptype = "void";
  }
  return ptype;
}

union {
  long long i;
  double d;
} value;

static std::string double2hexstr(double x) {
  value.d = x;
  char buf[17];
  snprintf(buf, sizeof(buf), "%016llx", value.i);
  buf[16] = 0;
  return std::string(buf);
}

static std::size_t get_type_size(const std::string& type) {
  if (type == "int" || type == "i32") {
    return 4;
  }
  if (type == "float" || type == "double") {
    return 4;
  }
  if (type == "char" || type == "i8") {
    return 1;
  }
  return 0;
}

static std::string get_llvm_op(const std::string& op, const std::string& type) {
  std::string ret_op;
  if (op == "+") {
    if (type == "int" || type == "i32") {
      ret_op = "add";
    }
    if (type == "float" || type == "double") {
      ret_op = "fadd";
    }
  } else if (op == "-") {
    if (type == "int" || type == "i32") {
      ret_op = "sub";
    }
    if (type == "float" || type == "double") {
      ret_op = "fsub";
    }
  } else if (op == "/") {
    if (type == "int" || type == "i32") {
      ret_op = "sdiv";
    }
    if (type == "float" || type == "double") {
      ret_op = "fdiv";
    }
  } else if (op == "%") {
    if (type == "float" || type == "double") {
      std::cerr << "ERROR: invalid operands to binary expression ('double' and "
                   "'double')\n";
    }
    ret_op = "srem";
  } else if (op == "*") {
    if (type == "int" || type == "i32") {
      ret_op = "mul";
    }
    if (type == "float" || type == "double") {
      ret_op = "fmul";
    }
  } else if (op == "++") {
    if (type == "int" || type == "i32") {
      ret_op = "add";
    }
    if (type == "float" || type == "double") {
      ret_op = "fadd";
    }
  } else if (op == "--") {
    if (type == "int" || type == "i32") {
      ret_op = "sub";
    }
    if (type == "float" || type == "double") {
      ret_op = "fsub";
    }
  }
  return ret_op;
}

static std::string get_log_op_int(const std::string& op) {
  std::string lo;
  if (op == ">") {
    lo = "sgt";
  } else if (op == ">=") {
    lo = "sge";
  } else if (op == "<=") {
    lo = "sle";
  } else if (op == "<") {
    lo = "slt";
  } else if (op == "==") {
    lo = "eq";
  } else if (op == "!=") {
    lo = "ne";
  }
  return lo;
}

static std::string get_log_op_double(const std::string& op) {
  std::string lo;
  if (op == ">") {
    lo = "ogt";
  } else if (op == ">=") {
    lo = "oge";
  } else if (op == "<=") {
    lo = "ole";
  } else if (op == "<") {
    lo = "olt";
  } else if (op == "==") {
    lo = "oeq";
  } else if (op == "!=") {
    lo = "une";
  }
  return lo;
}

void CodeGenerator::generate_file(std::ostream& out) {
  out << "target datalayout = "
         "\"e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-"
         "S128\"\n";
  out << "target triple = \"x86_64-redhat-linux-gnu\"\n\n";
  out << "@.str = private unnamed_addr constant [3 x i8] c\"%d\\00\", align "
         "1\n";
  out << "@.str.2 = private unnamed_addr constant [3 x i8] c\"%f\\00\", align "
         "1\n\n";

  out << "@.str.4 = private unnamed_addr constant [4 x i8] c\"%d\\0A\\00\", "
         "align 1\n";
  out << "@.str.5 = private unnamed_addr constant [4 x i8] c\"%c\\0A\\00\", "
         "align 1\n";
  out << "@.str.6 = private unnamed_addr constant [4 x i8] c\"%f\\0A\\00\", "
         "align 1\n";
  out << "@.str.7 = private unnamed_addr constant [4 x i8] c\"%s\\0A\\00\", "
         "align 1\n\n";

  out << string_stream_.str();

  if (!global_strs.empty()) {
    for (auto& item : global_strs) {
      out << item.first << " = dso_local constant [" << item.second.size() + 2
          << " x i8] c\"" << item.second << "\\0A\\00\", align 1\n";
    }
    out << "\n";
  }
  if (print_counter > 0) {
    out << "declare dso_local i32 @printf(i8*, ...) #1\n\n";
  }
  if (scanf_counter > 0) {
    out << "declare dso_local i32 @__isoc99_scanf(i8*, ...) #1\n\n";
  }
  if (memset_flag > 0) {
    out << "declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, "
           "i64, i1 immarg) #2\n";
  }
  if (new_flag > 0) {
    out << "declare dso_local nonnull i8* @_Znam(i64)\n";
  }
  out << "attributes #0 = { noinline nounwind optnone uwtable "
         "\"frame-pointer\"=\"all\" \"min-legal-vector-width\"=\"0\" "
         "\"no-trapping-math\"=\"true\" \"stack-protector-buffer-size\"=\"8\" "
         "\"target-cpu\"=\"x86-64\" "
         "\"target-features\"=\"+cx8,+fxsr,+mmx,+sse,+sse2,+x87\" "
         "\"tune-cpu\"=\"generic\" }\n";
  out << "attributes #1 = { \"frame-pointer\"=\"all\" "
         "\"no-trapping-math\"=\"true\" \"stack-protector-buffer-size\"=\"8\" "
         "\"target-cpu\"=\"x86-64\" "
         "\"target-features\"=\"+cx8,+fxsr,+mmx,+sse,+sse2,+x87\" "
         "\"tune-cpu\"=\"generic\" }\n";
  out << "attributes #2 = { argmemonly nofree nounwind willreturn }\n";
  if (for_flag > 0) {
    out << "!4 = distinct !{!4, !5}\n";
    out << "!5 = !{!\"llvm.loop.mustprogress\"}";
  }
}

void CodeGenerator::exec(
    Program& program,
    SymbolTable& symbol_table,
    std::ostream& out) {
  CodeGenerator code_generator(symbol_table);
  program.get_csclass()->accept(code_generator);
  code_generator.generate_file(out);
}

void CodeGenerator::visit(CSClass& node) {
  node.get_expr()->accept(*this);
}

void CodeGenerator::visit(Expression& node) {
  for (auto* c : node.get_exprs()) {
    c->get_elem()->accept(*this);
  }
}

void CodeGenerator::visit(Func_def& node) {
  std::string typ_tmp;
  if (node.get_void()) {
    typ_tmp = "void";
  } else {
    typ_tmp = node.get_var();
  }
  string_stream_ << "define " << get_llvm_typ(typ_tmp) << " @"
                 << node.get_func_name();
  // PARAMS
  string_stream_ << "(";
  if (!node.get_pars()->get_pars_var().empty()) {
    node.get_pars()->accept(*this);
  }
  string_stream_ << ") #0 {\n";

  string_stream_ << "block" << block_index << ":\n";
  block_index++;

  if (!node.get_pars()->get_pars_var().empty()) {
    for (auto& item : name_index_params) {
      auto ind = new_index();
      auto old_ind = variables_tab[item.first].first;
      variables_tab[item.first].first = ind;
      if (variables_tab[item.first].second == "i8*") {
        string_stream_ << ind << " = alloca i8*, align 8\n";
        string_stream_ << "store i8* " << old_ind << ", i8** " << ind
                       << ", align 8\n";
      }
      if (variables_tab[item.first].second == "i32") {
        string_stream_ << ind << " = alloca i32, align 4\n";
        string_stream_ << "store i32 " << old_ind << ", i32* " << ind
                       << ", align 4\n";
      }
      if (variables_tab[item.first].second == "i8") {
        string_stream_ << ind << " = alloca i8, align 1\n";
        string_stream_ << "store i8 " << old_ind << ", i8* " << ind
                       << ", align 1\n";
      }
      if (variables_tab[item.first].second == "double") {
        string_stream_ << ind << " = alloca double, align 8\n";
        string_stream_ << "store float " << old_ind << ", float* " << ind
                       << ", align 4\n";
      }
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

void CodeGenerator::visit(Pars& node) {
  if (!node.get_pars_var().empty()) {
    std::size_t i = 0;
    for (auto* c : node.get_pars_var()) {
      auto ptype = get_llvm_typ(c->get_var_typ());
      variables_tab[c->get_var_id()] = std::make_pair(new_index(), ptype);
      string_stream_ << ptype << " " << variables_tab[c->get_var_id()].first;
      name_index_params[c->get_var_id()] = std::make_pair(new_index(), ptype);
      stack.emplace_back(std::make_pair(
          c->get_var_id(),
          std::make_pair(
              name_index_params[c->get_var_id()].first,
              name_index_params[c->get_var_id()].second)));
      if (i < node.get_pars_var().size() - 1) {
        string_stream_ << ", ";
      }
      i++;
    }
  }
}

void CodeGenerator::visit(Scope& node) {
  slevel++;
  for (auto& c : node.get_args()) {
    c->get_statement_elem()->accept(*this);
  }
  slevel--;
}

std::string CodeGenerator::gettypof(const std::string& elem) {
  for (auto& el : symbol_table_) {
    if (el.get_obj_name() == elem) {
      return el.get_var_typ();
    }
  }
  return "";
}

void CodeGenerator::visit(Func_call& node) {
  if (node.get_args() != nullptr) {
    for (auto& c : node.get_args()->get_args()) {
      if (c->get_arg() != nullptr) {
        auto tmpind = new_index();
        if (c->get_arg()->get_var_typ() == "int") {
          string_stream_ << tmpind << " = alloca i32, align 4\n";
          stack.emplace_back(
              std::make_pair(tmpind, std::make_pair("-", "i32")));
        }
        if (c->get_arg()->get_var_typ() == "float") {
          string_stream_ << tmpind << " = alloca float, align 4\n";
          stack.emplace_back(
              std::make_pair(tmpind, std::make_pair("-", "float")));
        }
      }
      c->accept(*this);
    }
  }
  std::string tmpind;
  auto count = node.get_args()->get_args().size();

  std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
      tmpstack;

  while (count != 0) {
    tmpind = new_index();

    tmpstack.emplace_back(stack.back());
    stack.pop_back();

    if (tmpstack.back().second.second == "int" ||
        tmpstack.back().second.second == "i32") {
      string_stream_ << tmpind << " = load i32, i32* " << tmpstack.back().first
                     << ", align 4\n";
      tmpstack.back().first = tmpind;
    }
    if (tmpstack.back().second.second == "float") {
      string_stream_ << tmpind << " = load float, float* "
                     << tmpstack.back().first << ", align 4\n";
      tmpstack.back().first = tmpind;
    }
    --count;
  }
  count = node.get_args()->get_args().size();
  while (count != 0) {
    stack.emplace_back(tmpstack.back());
    tmpstack.pop_back();
    --count;
  }
  count = node.get_args()->get_args().size();
  tmpind = new_index();
  if (get_llvm_typ(gettypof(node.get_func_call_name())) != "void") {
    string_stream_ << tmpind << " = call " << get_llvm_typ(gettypof(node.get_func_call_name())) << " @" << node.get_func_call_name()
                  << "(";
  } else {
    string_stream_ << "call " << get_llvm_typ(gettypof(node.get_func_call_name())) << " @" << node.get_func_call_name()
                  << "(";
  }
  while (count != 0) {
    string_stream_ << stack.back().second.second << " " << stack.back().first;
    stack.pop_back();
    if (count > 1) {
      string_stream_ << ", ";
    }
    --count;
  }
  string_stream_ << ")\n";
  if (get_llvm_typ(gettypof(node.get_func_call_name())) != "void") {
    if (stack.back().second.second == "int" ||
        stack.back().second.second == "i32") {
      string_stream_ << "store i32 " << tmpind << ", i32* " << stack.back().first
                    << ", align 4\n";
    }
    if (stack.back().second.second == "float") {
      string_stream_ << "store float " << tmpind << ", float* "
                    << stack.back().first << ", align 4\n";
    }
  }
}

void CodeGenerator::visit(If_statement& node) {
  std::string tmpind = new_index();
  std::string ret_typ;
  if (node.get_if_log_op().empty()) {
    auto ind = variables_tab[node.get_if_1_arg()].first;
    ret_typ = variables_tab[node.get_if_1_arg()].second;
    string_stream_ << tmpind << " = load " << ret_typ << ", " << ret_typ << "* "
                   << ind << ", align 4\n";
    stack.emplace_back(std::make_pair(tmpind, std::make_pair(ind, ret_typ)));

    tmpind = new_index();
    if (stack.back().second.second == "int" ||
        stack.back().second.second == "i32") {
      string_stream_ << tmpind << " = icmp ne i32 " << stack.back().first
                     << ", 0\n";
    }
    if (stack.back().second.second == "float") {
      string_stream_ << tmpind << " = fcmp une float " << stack.back().first
                     << ", 0.000000e+00\n";
    }
    stack.back().first = tmpind;
  } else {
    auto ind = variables_tab[node.get_if_1_arg()].first;
    ret_typ = variables_tab[node.get_if_1_arg()].second;
    string_stream_ << tmpind << " = load " << ret_typ << ", " << ret_typ << "* "
                   << ind << ", align 4\n";
    stack.emplace_back(std::make_pair(tmpind, std::make_pair(ind, ret_typ)));
    if (node.get_if_2_arg()->get_arg() != nullptr) {
      tmpind = new_index();
      if (node.get_if_2_arg()->get_arg()->get_var_typ() == "int") {
        string_stream_ << tmpind << " = alloca i32, align 4\n";
        stack.emplace_back(std::make_pair(tmpind, std::make_pair("-", "i32")));
      }
      if (node.get_if_2_arg()->get_arg()->get_var_typ() == "float") {
        string_stream_ << tmpind << " = alloca float, align 4\n";
        stack.emplace_back(
            std::make_pair(tmpind, std::make_pair("-", "float")));
      }
    }
    node.get_if_2_arg()->accept(*this);
    tmpind = new_index();
    string_stream_ << tmpind << " = load " << stack.back().second.second << ", "
                   << stack.back().second.second << "* " << stack.back().first
                   << ", align 4\n";
    stack.pop_back();
    stack.emplace_back(std::make_pair(
        tmpind,
        std::make_pair(stack.back().first, stack.back().second.second)));
    tmpind = new_index();

    auto first_ = stack.back();
    stack.pop_back();
    auto second_ = stack.back();

    if (stack.back().second.second == "int" ||
        stack.back().second.second == "i32") {
      string_stream_ << tmpind << " = icmp "
                     << get_log_op_int(node.get_if_log_op()) << " i32 "
                     << second_.first << ", " << first_.first << "\n";
    }
    if (stack.back().second.second == "float") {
      string_stream_ << tmpind << " = fcmp "
                     << get_log_op_double(node.get_if_log_op()) << " float "
                     << second_.first << ", " << first_.first << "\n";
    }
    stack.pop_back();
  }
  string_stream_ << "br i1 " << tmpind << ", label %block" << block_index << "." << slevel << ", label %block"
                 << block_index + 1 << "." << slevel
                 << "\n";
  string_stream_ << "block" << block_index << "." << slevel << ":\n";
  block_index = block_index + 1;
  auto tmp = block_index;
  block_index = 1;
  node.get_if_scope()->accept(*this);
  block_index = tmp;
  if (node.get_if_else() != nullptr) {
    string_stream_
        << "br label %block"
        << block_index + 1 << "." << slevel
        << "\n";
    common_index = block_index + 1;
  } else {
    string_stream_ << "br label %block"
                   << block_index << "." << slevel
                   << "\n";
  }
  string_stream_ << "block" << block_index << "." << slevel
                 << ":\n";
  if (node.get_if_else() != nullptr) {
    node.get_if_else()->accept(*this);
  }
  block_index++;
}

void CodeGenerator::visit(Else_statement& node) {
  node.get_else_scope()->accept(*this);
  string_stream_ << "br label %block" << common_index << "." << slevel << "\n";
  string_stream_ << "block" << common_index << "." << slevel << ":\n";
  block_index = common_index + 1;
}

void CodeGenerator::visit(For_statement& node) {
  for_flag++;
  node.get_assign()->accept(*this);
  string_stream_ << "br label %for" << block_index << "." << slevel << "\n";
  string_stream_ << "for" << block_index << "." << slevel << ":\n";
  auto the_first_index = block_index;
  ++block_index;
  node.get_cond()->accept(*this);
  //auto tmp1 = index;
  string_stream_ << "br i1 %v" << index - 1 << ", label %for" << block_index << "." << slevel
                 << ", label %for"
                 << block_index + 2 << "." << slevel << "\n";
  auto tmp_index = block_index + 1;
  string_stream_ << "for" << block_index << "." << slevel << ":\n";
  ++block_index;
  auto tmp = block_index;
  block_index = 1;
  node.get_scope()->accept(*this);
  block_index = tmp;
  string_stream_ << "br label %for" << block_index << "." << slevel << "\n";
  string_stream_ << "for" << block_index << "." << slevel << ":\n";
  ++block_index;
  node.get_oper()->accept(*this);
  string_stream_ << "br label %for" << the_first_index << "." << slevel << ", !llvm.loop !4\n";
  string_stream_ << "for" << tmp_index + 1 << "." << slevel << ":\n";
  block_index = tmp_index + 2;
}

void CodeGenerator::visit(For_condition& node) {
  std::string tmpind = new_index();
  std::string ret_typ;

  auto ind = variables_tab[node.get_for_cond_1_arg()].first;
  ret_typ = variables_tab[node.get_for_cond_1_arg()].second;
  string_stream_ << tmpind << " = load " << ret_typ << ", " << ret_typ << "* "
                 << ind << ", align 4\n";
  stack.emplace_back(std::make_pair(tmpind, std::make_pair(ind, ret_typ)));
  //variables_tab[node.get_for_cond_1_arg()].first = tmpind;
  if (node.get_for_cond_2_arg()->get_arg() != nullptr) {
    tmpind = new_index();
    if (node.get_for_cond_2_arg()->get_arg()->get_var_typ() == "int") {
      string_stream_ << tmpind << " = alloca i32, align 4\n";
      stack.emplace_back(std::make_pair(tmpind, std::make_pair("-", "i32")));
    }
    if (node.get_for_cond_2_arg()->get_arg()->get_var_typ() == "float") {
      string_stream_ << tmpind << " = alloca float, align 4\n";
      stack.emplace_back(std::make_pair(tmpind, std::make_pair("-", "float")));
    }
  }
  node.get_for_cond_2_arg()->accept(*this);
  tmpind = new_index();
  string_stream_ << tmpind << " = load " << stack.back().second.second << ", "
                 << stack.back().second.second << "* " << stack.back().first
                 << ", align 4\n";
  stack.pop_back();
  stack.emplace_back(std::make_pair(
      tmpind, std::make_pair(stack.back().first, stack.back().second.second)));
  tmpind = new_index();

  auto first_ = stack.back();
  stack.pop_back();
  auto second_ = stack.back();

  if (stack.back().second.second == "int" ||
      stack.back().second.second == "i32") {
    string_stream_ << tmpind << " = icmp "
                   << get_log_op_int(node.get_for_cond_log_op()) << " i32 "
                   << second_.first << ", " << first_.first << "\n";
  }
  if (stack.back().second.second == "float") {
    string_stream_ << tmpind << " = fcmp "
                   << get_log_op_double(node.get_for_cond_log_op()) << " float "
                   << second_.first << ", " << first_.first << "\n";
  }
  stack.pop_back();
}

void CodeGenerator::visit(For_operation& node) {
  std::string tmpind = new_index();
  std::string tmpind2 = new_index();
  if (node.get_for_assign() != nullptr) {
    node.get_for_assign()->accept(*this);
    return;
  } else {
    auto ret_typ = variables_tab[node.get_for_id_var()].second;
    auto ind = variables_tab[node.get_for_id_var()].first;
    if (ret_typ == "i32" || ret_typ == "int") {
      string_stream_ << tmpind << " = load i32, i32* " << ind << ", align 4\n";
      string_stream_ << tmpind2 << " = "
                     << get_llvm_op(node.get_for_unar_op(), "i32")
                     << " nsw i32 " << tmpind << ", 1\n";
      string_stream_ << "store i32 " << tmpind2 << ", i32* " << ind
                     << ", align 4\n";
    }
    if (ret_typ == "float") {
      string_stream_ << tmpind << " = load float, float* " << ind
                     << ", align 4\n";
      string_stream_ << tmpind2 << " = "
                     << get_llvm_op(node.get_for_unar_op(), "float")
                     << " float " << tmpind << ", 1.000000e+00\n";
      string_stream_ << "store float " << tmpind2 << ", float* " << ind
                     << ", align 4\n";
    }
  }
}

void CodeGenerator::visit(Return_statement& node) {
  std::string tmp_return = new_index();
  std::string ret_typ;
  if (node.get_return_arg()->get_arg() != nullptr) {
    ret_typ = node.get_return_arg()->get_arg()->get_var_typ();
    ret_typ = get_llvm_typ(ret_typ);
    string_stream_ << "ret " << ret_typ << " "
                   << node.get_return_arg()->get_arg()->get_literal() << '\n';
  } else if (!node.get_return_arg()->get_arg_id().empty()) {
    ret_typ = variables_tab[node.get_return_arg()->get_arg_id()].second;
    auto ind = variables_tab[node.get_return_arg()->get_arg_id()].first;
    string_stream_ << tmp_return << " = load " << ret_typ << ", " << ret_typ
                   << "* " << ind << "\n";
    string_stream_ << "ret " << ret_typ << " " << tmp_return << '\n';
  } else if (node.get_func_call() != nullptr) {
    node.get_func_call()->accept(*this);
    string_stream_ << "ret " << stack.back().second.second << " "
                   << stack.back().first << '\n';
  }
}

void CodeGenerator::assig() {
  auto tmpind = new_index();
  auto first_ = stack.back();
  stack.pop_back();
  auto second_ = stack.back();
  if (stack.back().second.second == "int" ||
      stack.back().second.second == "i32") {
    string_stream_ << tmpind << " = load i32, i32* " << first_.first
                   << ", align 4\n";
    string_stream_ << "store i32 " << tmpind << ", i32* " << second_.first
                   << ", align 4\n";
  }
  if (stack.back().second.second == "char" ||
      stack.back().second.second == "i8") {
    string_stream_ << tmpind << " = load i8, i8* " << first_.first
                   << ", align 1\n";
    string_stream_ << "store i8 " << tmpind << ", i8* " << second_.first
                   << ", align 1\n";
  }
  if (stack.back().second.second == "float") {
    string_stream_ << tmpind << " = load float, float* " << first_.first
                   << ", align 4\n";
    string_stream_ << "store float " << tmpind << ", float* " << second_.first
                   << ", align 8\n";
  }
  if (stack.back().second.second == "string" ||
      stack.back().second.second == "i8*") {
    string_stream_ << tmpind << " = load i8*, i8** " << first_.first
                   << ", align 8\n";
    string_stream_ << "store i8* " << tmpind << ", i8** " << second_.first
                   << ", align 8\n";
  }
}

void CodeGenerator::calculation() {
  auto tmpind1 = new_index();
  auto tmpind2 = new_index();
  auto tmpind3 = new_index();
  auto first_ = stack.back();
  stack.pop_back();
  auto second_ = stack.back();
  stack.pop_back();
  auto answ_ = stack.back();
  if (stack.back().second.second == "int" ||
      stack.back().second.second == "i32") {
    string_stream_ << tmpind1 << " = load i32, i32* " << first_.first
                   << ", align 4\n";
    string_stream_ << tmpind2 << " = load i32, i32* " << second_.first
                   << ", align 4\n";
    string_stream_ << tmpind3 << " = "
                   << get_llvm_op(operation_stack.back(), "i32") << " i32 "
                   << tmpind2 << ", " << tmpind1 << "\n";
    string_stream_ << "store i32 " << tmpind3 << ", i32* " << answ_.first
                   << ", align 4\n";
  }
  if (stack.back().second.second == "float") {
    auto tmpind4 = new_index();
    auto tmpind5 = new_index();
    auto tmpind6 = new_index();
    string_stream_ << tmpind1 << " = load float, float* " << first_.first
                   << ", align 4\n";
    string_stream_ << tmpind2 << " = fpext float " << tmpind1 << " to double\n";
    string_stream_ << tmpind3 << " = load float, float* " << second_.first
                   << ", align 4\n";
    string_stream_ << tmpind4 << " = fpext float " << tmpind3 << " to double\n";
    string_stream_ << tmpind5 << " = "
                   << get_llvm_op(operation_stack.back(), "float") << " double "
                   << tmpind2 << ", " << tmpind4 << "\n";
    string_stream_ << tmpind6 << " = fptrunc double " << tmpind5
                   << " to float\n";
    string_stream_ << "store float " << tmpind6 << ", float* " << answ_.first
                   << ", align 4\n";
  }
  operation_stack.pop_back();
}

void CodeGenerator::visit(Assign_statement& node) {
  if (node.get_var_def() != nullptr) {
    node.get_var_def()->accept(*this);
  }
  if (!node.get_arg_rigth().empty()) {
    if (node.get_arg_rigth().size() == 1) {
      for (auto& c : node.get_arg_rigth()) {
        c->accept(*this);
        if (c->get_arg() == nullptr) {
          assig();
          return;
        }
      }
    } else {
      operation_stack.emplace_back(node.get_bin_op());
      for (auto& c : node.get_arg_rigth()) {
        if (c->get_arg() != nullptr) {
          auto tmpind = new_index();
          if (c->get_arg()->get_var_typ() == "int") {
            string_stream_ << tmpind << " = alloca i32, align 4\n";
            stack.emplace_back(
                std::make_pair(tmpind, std::make_pair("-", "i32")));
          }
          if (c->get_arg()->get_var_typ() == "float") {
            string_stream_ << tmpind << " = alloca float, align 4\n";
            stack.emplace_back(
                std::make_pair(tmpind, std::make_pair("-", "float")));
          }
        }
        c->accept(*this);
      }
      calculation();
      return;
    }
  }
  if (node.get_func_call() != nullptr) {
    node.get_func_call()->accept(*this);
  }
  if (node.get_mas_selection() != nullptr) {
    node.get_mas_selection()->accept(*this);
  }
}
void CodeGenerator::visit(Mas_statement& node) {
  if (node.get_mas_def() != nullptr) {
    node.get_mas_def()->accept(*this);
  }
  if (node.get_mas_change() != nullptr) {
    node.get_mas_change()->accept(*this);
  }
  if (!node.get_arg_rigth().empty()) {
    if (node.get_arg_rigth().size() == 1) {
      for (auto& c : node.get_arg_rigth()) {
        c->accept(*this);
        if (c->get_arg() == nullptr) {
          assig();
          return;
        }
      }
    } else {
      operation_stack.emplace_back(node.get_bin_op());
      for (auto& c : node.get_arg_rigth()) {
        if (c->get_arg() != nullptr) {
          auto tmpind = new_index();
          if (c->get_arg()->get_var_typ() == "int") {
            string_stream_ << tmpind << " = alloca i32, align 4\n";
            stack.emplace_back(
                std::make_pair(tmpind, std::make_pair("-", "i32")));
          }
          if (c->get_arg()->get_var_typ() == "float") {
            string_stream_ << tmpind << " = alloca float, align 4\n";
            stack.emplace_back(
                std::make_pair(tmpind, std::make_pair("-", "float")));
          }
        }
        c->accept(*this);
      }
      calculation();
      return;
    }
  }
  if (node.get_func_call() != nullptr) {
    node.get_func_call()->accept(*this);
  }
  if (node.get_mas_selection() != nullptr) {
    node.get_mas_selection()->accept(*this);
  }
}

void CodeGenerator::visit(Mas_change& node) {
  auto tmpind = new_index();
  auto tmpind2 = new_index();
  auto tmpind3 = new_index();
  auto tmpind4 = new_index();
  if (variables_tab[node.get_change_id()].second == "i32") {
    string_stream_ << tmpind << " = load i32*, i32** "
                   << variables_tab[node.get_change_id()].first
                   << ", align 8\n";
    if (variables_tab.find(node.get_at()->get_id_number()) !=
        variables_tab.end()) {
      string_stream_ << tmpind2 << " = load i32, i32* "
                     << variables_tab[node.get_at()->get_id_number()].first
                     << ", align 4\n";
      string_stream_ << tmpind3 << " = sext i32 " << tmpind2 << " to i64\n";
      string_stream_ << tmpind4 << " = getelementptr inbounds i32, i32* "
                     << tmpind << ", i64 " << tmpind3 << "\n";
      stack.emplace_back(
          std::make_pair(tmpind4, std::make_pair("@.str.4", "i32")));
    } else {
      string_stream_ << tmpind2 << " = getelementptr inbounds i32, i32* "
                     << tmpind << ", i64 " << node.get_at()->get_id_number()
                     << "\n";
      stack.emplace_back(
          std::make_pair(tmpind2, std::make_pair("@.str.4", "i32")));
    }
  }
  if (variables_tab[node.get_change_id()].second == "i8") {
    string_stream_ << tmpind << " = load i8*, i8** "
                   << variables_tab[node.get_change_id()].first
                   << ", align 8\n";
    if (variables_tab.find(node.get_at()->get_id_number()) !=
        variables_tab.end()) {
      string_stream_ << tmpind2 << " = load i32, i32* "
                     << variables_tab[node.get_at()->get_id_number()].first
                     << ", align 4\n";
      string_stream_ << tmpind3 << " = sext i32 " << tmpind2 << " to i64\n";
      string_stream_ << tmpind4 << " = getelementptr inbounds i8, i8* "
                     << tmpind << ", i64 " << tmpind3 << "\n";
      stack.emplace_back(
          std::make_pair(tmpind4, std::make_pair("@.str.5", "i8")));
    } else {
      string_stream_ << tmpind2 << " = getelementptr inbounds i8, i8* "
                     << tmpind << ", i64 " << node.get_at()->get_id_number()
                     << "\n";
      stack.emplace_back(
          std::make_pair(tmpind2, std::make_pair("@.str.5", "i8")));
    }
  }
  if (variables_tab[node.get_change_id()].second == "float") {
    string_stream_ << tmpind << " = load float*, float** "
                   << variables_tab[node.get_change_id()].first
                   << ", align 8\n";
    if (variables_tab.find(node.get_at()->get_id_number()) !=
        variables_tab.end()) {
      string_stream_ << tmpind2 << " = load i32, i32* "
                     << variables_tab[node.get_at()->get_id_number()].first
                     << ", align 4\n";
      string_stream_ << tmpind3 << " = sext i32 " << tmpind2 << " to i64\n";
      string_stream_ << tmpind4 << " = getelementptr inbounds float, float* "
                     << tmpind << ", i64 " << tmpind3 << "\n";
      stack.emplace_back(
          std::make_pair(tmpind4, std::make_pair("@.str.6", "float")));
    } else {
      string_stream_ << tmpind2 << " = getelementptr inbounds float, float* "
                     << tmpind << ", i64 " << node.get_at()->get_id_number()
                     << "\n";
      stack.emplace_back(
          std::make_pair(tmpind2, std::make_pair("@.str.6", "float")));
    }
  }
}

void CodeGenerator::visit(Mas_selection& node) {
  auto tmpind = new_index();
  string_stream_ << tmpind << " = call noalias nonnull i8* @_Znam(i64 "
                 << stoi(node.get_length()->get_id_number()) *
                        get_type_size(node.get_var_typ())
                 << ")\n";
  new_flag++;

  std::string tmpind2;
  std::string tmpind3;
  if (stack.back().second.second != "char" &&
      stack.back().second.second != "i8") {
    tmpind2 = new_index();
    tmpind3 = tmpind2;
    if (stack.back().second.second == "int" ||
        stack.back().second.second == "i32") {
      string_stream_ << tmpind2 << " = bitcast i8* " << tmpind << " to i32*\n";
    }
    if (stack.back().second.second == "float") {
      string_stream_ << tmpind2 << " = bitcast i8* " << tmpind
                     << " to float*\n";
    }
  } else {
    tmpind2 = tmpind;
    tmpind3 = tmpind;
  }

  if (!node.get_literal().empty()) {
    for (auto& c : node.get_literal()) {
      tmpind = new_index();
      if (c->get_var_typ() != "string") {
        std::string tmp_str = c->get_literal();
        if (c->get_var_typ() == "int") {
          string_stream_ << "store i32 " << tmp_str << ", i32* " << tmpind2
                         << ", align 4\n";
          string_stream_ << tmpind << " = getelementptr inbounds i32, i32* "
                         << tmpind2 << ", i64 1\n";
        }
        if (c->get_var_typ() == "char") {
          tmp_str.erase(
              std::remove(tmp_str.begin(), tmp_str.end(), '\''), tmp_str.end());
          char t = tmp_str[0];
          string_stream_ << "store i8 " << static_cast<int>(t) << ", i8* "
                         << tmpind2 << ", align 1\n";
          string_stream_ << tmpind << " = getelementptr inbounds i8, i8* "
                         << tmpind2 << ", i64 1\n";
        }
        if (c->get_var_typ() == "float") {
          double tmp = atof(tmp_str.c_str());
          string_stream_ << "store float 0x" << double2hexstr(tmp)
                         << ", float* " << tmpind2 << ", align 4\n";
          string_stream_ << tmpind << " = getelementptr inbounds float, float* "
                         << tmpind2 << ", i64 1\n";
        }
        tmpind2 = tmpind;
      }
    }
    tmpind = new_index();
    if (stack.back().second.second == "int" ||
        stack.back().second.second == "i32") {
      string_stream_ << tmpind << " = bitcast i32* " << tmpind2 << " to i8*\n";
      string_stream_ << "call void @llvm.memset.p0i8.i64(i8* align 4 " << tmpind
                     << ", i8 0, i64 4, i1 false)\n";
      memset_flag++;
    }
  }
  string_stream_ << "store " << stack.back().second.second << "* " << tmpind3
                 << ", " << stack.back().second.second << "** "
                 << stack.back().first << ", align 8\n";
}

void CodeGenerator::visit(Mas_def& node) {
  auto tmpind = new_index();
  if (node.get_var_typ() == "int") {
    string_stream_ << tmpind << " = alloca i32*, align 8\n";
    variables_tab[node.get_var_id()].first = tmpind;
    variables_tab[node.get_var_id()].second = "i32";
    stack.emplace_back(
        std::make_pair(tmpind, std::make_pair(node.get_var_id(), "i32")));
  }
  if (node.get_var_typ() == "char") {
    string_stream_ << tmpind << " = alloca i8*, align 8\n";
    variables_tab[node.get_var_id()].first = tmpind;
    variables_tab[node.get_var_id()].second = "i8";
    stack.emplace_back(
        std::make_pair(tmpind, std::make_pair(node.get_var_id(), "i8")));
  }
  if (node.get_var_typ() == "float") {
    string_stream_ << tmpind << " = alloca float*, align 8\n";
    variables_tab[node.get_var_id()].first = tmpind;
    variables_tab[node.get_var_id()].second = "float";
    stack.emplace_back(
        std::make_pair(tmpind, std::make_pair(node.get_var_id(), "float")));
  }
}

void CodeGenerator::visit(Var_def& node) {
  auto tmpind = new_index();
  if (node.get_var_typ() == "int") {
    string_stream_ << tmpind << " = alloca i32, align 4\n";
    variables_tab[node.get_var_id()].first = tmpind;
    variables_tab[node.get_var_id()].second = "i32";
    stack.emplace_back(
        std::make_pair(tmpind, std::make_pair(node.get_var_id(), "i32")));
  }
  if (node.get_var_typ() == "char") {
    string_stream_ << tmpind << " = alloca i8, align 1\n";
    variables_tab[node.get_var_id()].first = tmpind;
    variables_tab[node.get_var_id()].second = "i8";
    stack.emplace_back(
        std::make_pair(tmpind, std::make_pair(node.get_var_id(), "i8")));
  }
  if (node.get_var_typ() == "float") {
    string_stream_ << tmpind << " = alloca float, align 4\n";
    variables_tab[node.get_var_id()].first = tmpind;
    variables_tab[node.get_var_id()].second = "float";
    stack.emplace_back(
        std::make_pair(tmpind, std::make_pair(node.get_var_id(), "float")));
  }
  if (node.get_var_typ() == "string") {
    string_stream_ << tmpind << " = alloca i8*, align 8\n";
    variables_tab[node.get_var_id()].first = tmpind;
    variables_tab[node.get_var_id()].second = "i8*";
    stack.emplace_back(
        std::make_pair(tmpind, std::make_pair(node.get_var_id(), "i8*")));
  }
}

void CodeGenerator::visit(Print_statement& node) {
  print_counter++;
  if (node.get_print()->get_arg() == nullptr) {
    node.get_print()->accept(*this);
    auto tmpind = new_index();
    if (stack.back().second.second == "i32") {
      string_stream_ << tmpind << " = load i32, i32* " << stack.back().first
                     << ", align 4\n";
      stack.back().first = tmpind;
    }
    if (stack.back().second.second == "i8") {
      string_stream_ << tmpind << " = load i8, i8* " << stack.back().first
                     << ", align 1\n";
      auto tmp = new_index();
      string_stream_ << tmp << " = sext i8 " << tmpind << " to i32\n";
      stack.back().first = tmp;
      stack.back().second.second = "i32";
    }
    if (stack.back().second.second == "float") {
      string_stream_ << tmpind << " = load float, float* " << stack.back().first
                     << ", align 4\n";
      auto tmp = new_index();
      string_stream_ << tmp << " = fpext float " << tmpind << " to double\n";
      stack.back().first = tmp;
      stack.back().second.second = "double";
    }
    if (stack.back().second.second == "i8*") {
      string_stream_ << tmpind << " = load i8*, i8** " << stack.back().first
                     << ", align 8\n";
      stack.back().first = tmpind;
    }
    string_stream_ << new_index()
                   << " = call i32 (i8*, ...) @printf(i8* getelementptr "
                      "inbounds ([4 x i8], [4 x i8]* "
                   << stack.back().second.first << ", i64 0, i64 0), "
                   << stack.back().second.second << " " << stack.back().first
                   << ")\n";
    stack.pop_back();
  } else if (node.get_print()->get_arg() != nullptr) {
    std::string tmp_str = node.get_print()->get_arg()->get_literal();
    if (node.get_print()->get_arg()->get_var_typ() == "char") {
      tmp_str.erase(
          std::remove(tmp_str.begin(), tmp_str.end(), '\''), tmp_str.end());
    }
    if (node.get_print()->get_arg()->get_var_typ() == "string") {
      tmp_str.erase(
          std::remove(tmp_str.begin(), tmp_str.end(), '\"'), tmp_str.end());
    }
    global_strs["@.str." + std::to_string(str_index)] = tmp_str;
    stack.emplace_back(std::make_pair(
        "-", std::make_pair("@.str." + std::to_string(str_index), tmp_str)));
    ++str_index;
    string_stream_
        << new_index()
        << " = call i32 (i8*, ...) @printf(i8* getelementptr inbounds (["
        << stack.back().second.second.size() + 2 << " x i8], ["
        << stack.back().second.second.size() + 2 << " x i8]* "
        << stack.back().second.first << ", i64 0, i64 0))\n";
    stack.pop_back();
  }
}

void CodeGenerator::visit(Read_statement& node) {
  scanf_counter++;

  if (node.get_read()->get_arg() != nullptr) {
    return;
  }
  node.get_read()->accept(*this);
  stack.pop_back();
  if (!node.get_read()->get_arg_id().empty()) {
    if (variables_tab[node.get_read()->get_arg_id()].second == "i32") {
      stack.emplace_back(std::make_pair(
          variables_tab[node.get_read()->get_arg_id()].first,
          std::make_pair("@.str", "i32")));
    }
    if (variables_tab[node.get_read()->get_arg_id()].second == "float") {
      stack.emplace_back(std::make_pair(
          variables_tab[node.get_read()->get_arg_id()].first,
          std::make_pair("@.str.2", "float")));
    }
  }

  string_stream_ << new_index()
                 << " = call i32 (i8*, ...) @__isoc99_scanf(i8* getelementptr "
                    "inbounds ([3 x i8], [3 x i8]* "
                 << stack.back().second.first << ", i64 0, i64 0), "
                 << stack.back().second.second << "* " << stack.back().first
                 << ")\n";
  stack.pop_back();
}

void CodeGenerator::visit(Arg& node) {
  if (node.get_arg() != nullptr) {
    node.get_arg()->accept(*this);
  }
  if (node.get_arg_mas() != nullptr) {
    node.get_arg_mas()->accept(*this);
  }
  if (!node.get_arg_id().empty()) {
    if (variables_tab[node.get_arg_id()].second == "i8*") {
      stack.emplace_back(std::make_pair(
          variables_tab[node.get_arg_id()].first,
          std::make_pair("@.str.7", "i8*")));
    }
    if (variables_tab[node.get_arg_id()].second == "i32") {
      stack.emplace_back(std::make_pair(
          variables_tab[node.get_arg_id()].first,
          std::make_pair("@.str.4", "i32")));
    }
    if (variables_tab[node.get_arg_id()].second == "i8") {
      stack.emplace_back(std::make_pair(
          variables_tab[node.get_arg_id()].first,
          std::make_pair("@.str.5", "i8")));
    }
    if (variables_tab[node.get_arg_id()].second == "float") {
      stack.emplace_back(std::make_pair(
          variables_tab[node.get_arg_id()].first,
          std::make_pair("@.str.6", "float")));
    }
  }
}

void CodeGenerator::visit(Literal& node) {
  std::string tmp_str = node.get_literal();
  if (node.get_var_typ() == "int") {
    auto ind = stack.back().first;
    string_stream_ << "store i32 " << tmp_str << ", i32* " << ind
                   << ", align 4\n";
    stack.back().first = ind;
  }
  if (node.get_var_typ() == "char") {
    auto ind = stack.back().first;
    tmp_str.erase(
        std::remove(tmp_str.begin(), tmp_str.end(), '\''), tmp_str.end());
    char t = tmp_str[0];
    string_stream_ << "store i8 " << static_cast<int>(t) << ", i8* " << ind
                   << ", align 1\n";
  }
  if (node.get_var_typ() == "float") {
    auto ind = stack.back().first;
    double tmp = atof(tmp_str.c_str());
    string_stream_ << "store float 0x" << double2hexstr(tmp) << ", float* "
                   << ind << ", align 4\n";
    stack.back().first = ind;
  }
  if (node.get_var_typ() == "string") {
    auto ind = stack.back().first;
    tmp_str.erase(
        std::remove(tmp_str.begin(), tmp_str.end(), '\"'), tmp_str.end());
    string_stream_ << "store i8* getelementptr inbounds (["
                   << std::to_string(tmp_str.size() + 2) << " x i8], ["
                   << std::to_string(tmp_str.size() + 2) << " x i8]* "
                   << "@.str." << std::to_string(str_index)
                   << ", i64 0, i64 0), i8** " << ind << ", align 8\n";
    global_strs["@.str." + std::to_string(str_index)] = tmp_str;
    ++str_index;
  }
}

}  // namespace csharp::ast
