#pragma once

#include <memory>
#include <vector>
#include <fstream>
#include <string>
#include <map>

namespace csharp::ast {

class Visitor;

class Program;
class CSClass;
class Expression;
class Expressions;
class Kw_statement;
class Var_def;
class Print_statement;
class Read_statement;
class Func_call;
class Arguments;
class Arg;
class Literal;
class Statement;
class Scope;
class If_statement;
class Else_statement;
class Return_statement;
class Pars;
class Func_def;
class Assign_statement;
class For_statement;
class For_condition;
class For_operation;
class Length;
class Mas_statement;
class Mas_def;
class Mas_selection;
class Mas_change;

class Node {
public:
    virtual ~Node() = default;

    virtual void accept(Visitor &visitor) = 0;
};



class Program final {
private:
    std::vector<std::unique_ptr<Node>> members_;
    CSClass *csclass_ptr_ = nullptr;

public:
    template <class T, class... Args> T *create_node(Args &&...args) {
        static_assert(std::is_base_of_v<Node, T>);
        members_.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        return dynamic_cast<T *>(members_.back().get());
    }
    void set_csclass(CSClass *csclass_ptr) { csclass_ptr_ = csclass_ptr; }
    CSClass *get_csclass() { return csclass_ptr_; }
};



class CSClass final : public Node {
private:
    std::string csclass_name_;
    Expression* expression_ = nullptr;

public:
    explicit CSClass(std::string id, Expression* expression)
        : csclass_name_(std::move(id)), expression_(expression) {}
    Expression* get_expr() const { return expression_; }
    std::string get_csclass_name() { return csclass_name_; }

    void accept(Visitor& visitor) override;
};



class Expression final : public Node {
private:
    std::vector<Expressions *> expressions_;

public:
    explicit Expression(std::vector<Expressions *> expressions) : expressions_(std::move(expressions)) {}
    std::vector<Expressions *> get_exprs() const { return expressions_; }

    void accept(Visitor& visitor) override;
};



class Expressions final : public Node {
private:
    Node* elem_ = nullptr;

public:
    explicit Expressions(Node* elem) : elem_(elem) {}
    Node* get_elem() const { return elem_; }

    void accept(Visitor& visitor) override;
};



class Kw_statement final : public Node {
private:
    std::string key_word_;

public:
    explicit Kw_statement(std::string key_word)
        : key_word_(std::move(key_word)) {}
    std::string get_key_word() { return key_word_; }

    void accept(Visitor& visitor) override;
};



class Var_def final : public Node {
private:
    std::string var_type_;
    std::string var_id_;

public:
    explicit Var_def(std::string var_type_tmp, std::string var_id)
        : var_type_(std::move(var_type_tmp)), var_id_(std::move(var_id)) {}
    std::string get_var_typ() { return var_type_; }
    std::string get_var_id() { return var_id_; }

    void accept(Visitor& visitor) override;
};



class Mas_selection final : public Node {
private:
    std::string var_type_;
    Length* length_ = nullptr;
    std::vector<Literal *> literal_;

public:
    explicit Mas_selection(std::string var_type_tmp, Length* length, std::vector<Literal *> literal)
        : var_type_(std::move(var_type_tmp)), length_(std::move(length)), literal_(std::move(literal)) {}

    std::string get_var_typ() { return var_type_; }
    Length* get_length() { return length_; }
    std::vector<Literal *> get_literal() { return literal_; }

    void accept(Visitor& visitor) override;
};



class Length final : public Node {
private:
    std::string id_number_;

public:
    explicit Length(std::string id_number)
        : id_number_(std::move(id_number)) {}

    std::string get_id_number() { return id_number_; }

    void accept(Visitor& visitor) override;
};



class Mas_change final : public Node {
private:
    std::string change_id_;
    Length* at_ = nullptr;

public:
    explicit Mas_change(std::string change_id, Length* at)
        : change_id_(std::move(change_id)), at_(std::move(at)) {}
    std::string get_change_id() { return change_id_; }
    Length* get_at() { return at_; }

    void accept(Visitor& visitor) override;
};



class Mas_def final : public Node {
private:
    std::string var_type_;
    std::string var_id_;

public:
    explicit Mas_def(std::string var_type_tmp, std::string var_id)
        : var_type_(std::move(var_type_tmp)), var_id_(std::move(var_id)) {}
    std::string get_var_typ() { return var_type_; }
    std::string get_var_id() { return var_id_; }

    void accept(Visitor& visitor) override;
};



class Mas_statement final : public Node {
private:
    Mas_change* mas_change_ = nullptr;
    Mas_def* mas_def_ = nullptr;
    std::vector<Arg*> arg_rigth_;
    std::string bin_op_;
    Func_call* func_call_ = nullptr;
    Mas_selection* mas_selection_ = nullptr;

public:
    explicit Mas_statement(Mas_change* mas_change, std::vector<Arg*> arg_rigth)
    : mas_change_(std::move(mas_change)), arg_rigth_(std::move(arg_rigth)) {}
    explicit Mas_statement(Mas_change* mas_change, std::vector<Arg*> arg_rigth, std::string bin_op)
    : mas_change_(std::move(mas_change)), arg_rigth_(std::move(arg_rigth)), bin_op_(std::move(bin_op)) {}
    explicit Mas_statement(Mas_change* mas_change, Func_call* func_call)
    : mas_change_(std::move(mas_change)), func_call_(func_call) {}
    explicit Mas_statement(Mas_change* mas_change, Mas_selection* mas_selection)
    : mas_change_(std::move(mas_change)), mas_selection_(mas_selection) {}

    explicit Mas_statement(Mas_def* mas_def, std::vector<Arg*> arg_rigth)
    : mas_def_(mas_def), arg_rigth_(std::move(arg_rigth)) {}
    explicit Mas_statement(Mas_def* mas_def, std::vector<Arg*> arg_rigth, std::string bin_op)
    : mas_def_(mas_def), arg_rigth_(std::move(arg_rigth)), bin_op_(std::move(bin_op)) {}
    explicit Mas_statement(Mas_def* mas_def, Func_call* func_call)
    : mas_def_(mas_def), func_call_(func_call) {}
    explicit Mas_statement(Mas_def* mas_def, Mas_selection* mas_selection)
    : mas_def_(mas_def), mas_selection_(mas_selection) {}

    Mas_change* get_mas_change() const { return mas_change_; }
    Mas_def* get_mas_def() const { return mas_def_; }
    std::vector<Arg*> get_arg_rigth() { return arg_rigth_; }
    std::string get_bin_op() { return bin_op_; }
    Func_call* get_func_call() const { return func_call_; }
    Mas_selection* get_mas_selection() const { return mas_selection_; }

    void accept(Visitor& visitor) override;
};



class Pars final : public Node {
private:
    std::vector<Var_def *> var_defs_;
    std::vector<Mas_def *> mas_defs_;

public:
    explicit Pars(std::vector<Var_def *> defs) : var_defs_(std::move(defs)) {}
    explicit Pars(std::vector<Mas_def *> defs) : mas_defs_(std::move(defs)) {}
    explicit Pars(std::vector<Var_def *> var_defs, std::vector<Mas_def *> mas_defs)
    : var_defs_(std::move(var_defs)), mas_defs_(std::move(mas_defs)) {}
    std::vector<Var_def *> get_pars_var() const { return var_defs_; }
    std::vector<Mas_def *> get_pars_mas() const { return mas_defs_; }

    void accept(Visitor& visitor) override;
};



class Statement final : public Node {
private:
    Node* elem_ = nullptr;

public:
    explicit Statement(Node* elem) : elem_(elem) {}
    Node* get_statement_elem() const { return elem_; }

    void accept(Visitor& visitor) override;
};



class Scope final : public Node {
private:
    std::vector<Statement *> statements_;

public:
    explicit Scope(std::vector<Statement *> statements) : statements_(std::move(statements)) {}
    std::vector<Statement *> get_args() const { return statements_; }

    void accept(Visitor& visitor) override;
};



class Arguments final : public Node {
private:
    std::vector<Arg *> args_;

public:
    explicit Arguments(std::vector<Arg *> args) : args_(std::move(args)) {}
    std::vector<Arg *> get_args() const { return args_; }

    void accept(Visitor& visitor) override;
};



class Arg final : public Node {
private:
    std::string arg_id_;
    Literal* arg_ = nullptr;
    Mas_change* arg_mas_ = nullptr;

public:
    explicit Arg(std::string arg_id) : arg_id_(std::move(arg_id)) {}
    explicit Arg(Literal* arg) : arg_(arg) {}
    explicit Arg(Mas_change* arg_mas) : arg_mas_(arg_mas) {}
    Literal* get_arg() const { return arg_; }
    Mas_change* get_arg_mas() const { return arg_mas_; }
    std::string get_arg_id() { return arg_id_; }

    void accept(Visitor& visitor) override;
};



class Literal final : public Node {
private:
    std::string literal_;
    bool bool_flag_ = false;

public:
    explicit Literal(std::string literal)
        : literal_(std::move(literal)) {}
    explicit Literal(std::string literal, bool bool_flag)
        : literal_(std::move(literal)), bool_flag_(std::move(bool_flag)) {}
    std::string get_literal() { return literal_; }
    bool get_bool_flag() { return bool_flag_; }

    void accept(Visitor& visitor) override;
};



class Func_call final : public Node {
private:
    std::string func_name_;
    Arguments* args_ = nullptr;

public:
    explicit Func_call(std::string func_name, Arguments* args)
        : func_name_(std::move(func_name)), args_(args) {}
    Arguments* get_args() const { return args_; }
    std::string get_func_call_name() { return func_name_; }
    bool isempty() { return func_name_.empty(); }

    void accept(Visitor& visitor) override;
};



class Return_statement final : public Node {
private:
    Arg* arg_ = nullptr;

public:
    explicit Return_statement(Arg* arg)
        : arg_(arg) {}
    Arg* get_return_arg() const { return arg_; }

    void accept(Visitor& visitor) override;
};



class Func_def final : public Node {
private:
    std::vector<std::string> kw_;
    std::string var_;
    bool void_ = false;
    std::string func_name_;
    Pars* pars_ = nullptr;
    Scope* scope_ = nullptr;
    Return_statement* return_ = nullptr;

public:
    explicit Func_def(bool void_tmp, std::string func_name, Pars* pars, Scope* scope, Return_statement* return_tmp)
    : void_(std::move(void_tmp)), func_name_(std::move(func_name)), pars_(pars), scope_(scope), return_(return_tmp) {}
    explicit Func_def(std::string var, std::string func_name, Pars* pars, Scope* scope, Return_statement* return_tmp)
    : var_(std::move(var)), func_name_(std::move(func_name)), pars_(pars), scope_(scope), return_(return_tmp) {}
    explicit Func_def(std::vector<std::string> kw, bool void_tmp, std::string func_name, Pars* pars, Scope* scope, Return_statement* return_tmp)
    : kw_(std::move(kw)), void_(std::move(void_tmp)), func_name_(std::move(func_name)), pars_(pars), scope_(scope), return_(return_tmp) {}
    explicit Func_def(std::vector<std::string> kw, std::string var, std::string func_name, Pars* pars, Scope* scope, Return_statement* return_tmp)
    : kw_(std::move(kw)), var_(std::move(var)), func_name_(std::move(func_name)), pars_(pars), scope_(scope), return_(return_tmp) {}

    std::vector<std::string> get_kw() const { return kw_; }
    std::string get_var() { return var_; }
    bool get_void() { return void_; }
    std::string get_func_name() { return func_name_; }
    Pars* get_pars() const { return pars_; }
    Scope* get_scope() const { return scope_; }
    Return_statement* get_return() const { return return_; }

    void accept(Visitor& visitor) override;
};



class Assign_statement final : public Node {
private:
    std::string id_left_;
    Var_def* var_def_ = nullptr;
    std::vector<Arg*> arg_rigth_;
    std::string bin_op_;
    Func_call* func_call_ = nullptr;
    Mas_selection* mas_selection_ = nullptr;

public:
    explicit Assign_statement(std::string id_left, std::vector<Arg*> arg_rigth)
    : id_left_(std::move(id_left)), arg_rigth_(std::move(arg_rigth)) {}
    explicit Assign_statement(std::string id_left, std::vector<Arg*> arg_rigth, std::string bin_op)
    : id_left_(std::move(id_left)), arg_rigth_(std::move(arg_rigth)), bin_op_(std::move(bin_op)) {}
    explicit Assign_statement(std::string id_left, Func_call* func_call)
    : id_left_(std::move(id_left)), func_call_(func_call) {}
    explicit Assign_statement(std::string id_left, Mas_selection* mas_selection)
    : id_left_(std::move(id_left)), mas_selection_(mas_selection) {}

    explicit Assign_statement(Var_def* var_def, std::vector<Arg*> arg_rigth)
    : var_def_(var_def), arg_rigth_(std::move(arg_rigth)) {}
    explicit Assign_statement(Var_def* var_def, std::vector<Arg*> arg_rigth, std::string bin_op)
    : var_def_(var_def), arg_rigth_(std::move(arg_rigth)), bin_op_(std::move(bin_op)) {}
    explicit Assign_statement(Var_def* var_def, Func_call* func_call)
    : var_def_(var_def), func_call_(func_call) {}
    explicit Assign_statement(Var_def* var_def, Mas_selection* mas_selection)
    : var_def_(var_def), mas_selection_(mas_selection) {}

    std::string get_id_left() { return id_left_; }
    Var_def* get_var_def() const { return var_def_; }
    std::vector<Arg*> get_arg_rigth() const { return arg_rigth_; }
    std::string get_bin_op() { return bin_op_; }
    Func_call* get_func_call() const { return func_call_; }
    Mas_selection* get_mas_selection() { return mas_selection_; }

    void accept(Visitor& visitor) override;
};



class For_statement final : public Node {
private:
    Assign_statement* assign_ = nullptr;
    For_condition* cond_ = nullptr;
    For_operation* oper_ = nullptr;
    Scope* scope_ = nullptr;

public:
    explicit For_statement(Assign_statement* assign, For_condition* cond, For_operation* oper, Scope* scope)
    : assign_(assign), cond_(cond), oper_(oper), scope_(scope) {}

    Assign_statement* get_assign() const { return assign_; }
    For_condition* get_cond() const { return cond_; }
    For_operation* get_oper() const { return oper_; }
    Scope* get_scope() const { return scope_; }

    void accept(Visitor& visitor) override;
};



class For_operation final : public Node {
private:
    std::string id_var_;
    std::string unar_op_;
    Assign_statement* assign_ = nullptr;

public:
    explicit For_operation(std::string id_var, std::string unar_op) : id_var_(std::move(id_var)), unar_op_(std::move(unar_op)) {}
    explicit For_operation(Assign_statement* assign) : assign_(assign) {}

    std::string get_for_id_var() { return id_var_; }
    std::string get_for_unar_op() { return unar_op_; }
    Assign_statement* get_for_assign() const { return assign_; }

    void accept(Visitor& visitor) override;
};



class If_statement final : public Node {
private:
    std::string arg1_;
    std::string logic_op_;
    Arg* arg2_ = nullptr;
    Scope* scope_ = nullptr;
    Else_statement* else_ = nullptr;

public:
    explicit If_statement(std::string arg1, Scope* scope) : arg1_(std::move(arg1)), scope_(scope) {}
    explicit If_statement(std::string arg1, std::string logic_op, Arg* arg2, Scope* scope) : arg1_(std::move(arg1)), logic_op_(std::move(logic_op)), arg2_(arg2), scope_(scope) {}
    explicit If_statement(std::string arg1, Scope* scope, Else_statement* else_tmp) : arg1_(std::move(arg1)), scope_(scope), else_(else_tmp) {}
    explicit If_statement(std::string arg1, std::string logic_op, Arg* arg2, Scope* scope, Else_statement* else_tmp)
    : arg1_(std::move(arg1)), logic_op_(std::move(logic_op)), arg2_(arg2), scope_(scope), else_(else_tmp) {}

    std::string get_if_1_arg() { return arg1_; }
    std::string get_if_log_op() { return logic_op_; }
    Arg* get_if_2_arg() const { return arg2_; }
    Scope* get_if_scope() const { return scope_; }
    Else_statement* get_if_else() const { return else_; }

    void accept(Visitor& visitor) override;
};



class Else_statement final : public Node {
private:
    Scope* scope_ = nullptr;

public:
    explicit Else_statement(Scope* scope)
        : scope_(scope) {}
    Scope* get_else_scope() const { return scope_; }

    void accept(Visitor& visitor) override;
};



class For_condition final : public Node {
private:
    std::string arg1_;
    std::string logic_op_;
    Arg* arg2_ = nullptr;

public:
    explicit For_condition(std::string arg1, std::string logic_op, Arg* arg2)
        : arg1_(std::move(arg1)), logic_op_(std::move(logic_op)), arg2_(arg2) {}
    std::string get_for_cond_1_arg() { return arg1_; }
    std::string get_for_cond_log_op() { return logic_op_; }
    Arg* get_for_cond_2_arg() const { return arg2_; }

    void accept(Visitor& visitor) override;

};



class Read_statement final : public Node {
private:
    Arg* arg_ = nullptr;

public:
    explicit Read_statement(Arg* arg)
        : arg_(arg) {}
    Arg* get_read() { return arg_; }

    void accept(Visitor& visitor) override;
};



class Print_statement final : public Node {
private:
    Arg* arg_ = nullptr;

public:
    explicit Print_statement(Arg* arg)
        : arg_(arg) {}
    Arg* get_print() { return arg_; }

    void accept(Visitor& visitor) override;
};

}  // namespace csharp::ast
