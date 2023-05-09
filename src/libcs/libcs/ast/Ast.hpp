#pragma once

#include <memory>
#include <vector>
#include <fstream>
#include <string>
#include <map>

namespace csharp::ast {

class Visitor;

class Node;

class Program;
class CSClass;
class Expressions;
class Assign_statement;
class Literal;
class Var_def;
class Func_def;
class Scope;
class Return_statement;
class Statement;
class Func_call;
class Args;
class Arg;
class Pars;
class If_statement;
class Else_statement;
class For_statement;
class For_condition;
class For_operation;
class Kw_statement;
class Print_statement;
class Read_statement;

class Node {
public:
    virtual ~Node() = default;

    virtual void accept(Visitor &visitor) = 0;
};

class Statement : public Node {
public:
    virtual ~Statement() = default;

    virtual void accept(Visitor &visitor) = 0;
};

class Program {
private:
    CSClass *csclass_ptr = nullptr;

public:
    Program() = default;
    void set_csclass(CSClass *cscptr) { csclass_ptr = cscptr; }
    CSClass *get_csclass() { return csclass_ptr; }
};

class CSClass final : public Node {
private:
    Expressions *expr_ptr = nullptr;
    std::string class_name;

public:
    CSClass() = default;
    std::string &get_class_name() { return class_name; }
    void set_class_name(std::string cname) { class_name = cname; };

    void set_exprs(Expressions *expr) { expr_ptr = expr; }
    Expressions *get_exprs() { return expr_ptr; };

    void accept(Visitor &visitor) override;
};

class Expressions final : public Node {
private:
    std::vector<ast::Statement *> components;

public:
    explicit Expressions(std::vector<ast::Statement *> components_)
        : components(std::move(components_)) {}

    const std::vector<ast::Statement *> &get_components() const { return components; }

    void accept(Visitor &visitor) override;
};

/*
class Node
{
protected:
    static inline std::size_t m_depth = 0;
    static inline bool dpsn = false;
    std::size_t line = 0;
    std::size_t char_pos = 0;
    bool x = false;

public:
    virtual ~Node() = default;
    static void increase_depth() { m_depth += 2; }
    static void decrease_depth()
    {
        if (m_depth >= 2)
        {
            m_depth -= 2;
        }
    }
    void set_x(bool b) { x = b; }
    bool get_x() { return x; }
    void set_line(std::size_t l) { line = l; }
    std::size_t get_line() { return line; }
    void set_char_pos(std::size_t cp) { char_pos = cp; }
    std::size_t get_char_pos() { return char_pos; }
    static void set_depth(std::size_t d) { m_depth = d; }
    static void set_dpsn(bool d) { dpsn = d; }
    static bool get_dpsn() { return dpsn; }
    static std::size_t get_depth() { return m_depth; }
    virtual void accept(Visitor &visitor) = 0;
};

class Args : public Node
{
private:
    std::string arg;
    bool is_lit = false;

public:
    Args() = default;
    bool is_literal() { return is_lit; }
    void set_literal(bool l) { is_lit = l; }
    std::string get_arg() { return arg; }
    void set_arg(std::string a) { arg = a; }
    void accept(Visitor &visitor) override;
    ~Args() = default;
};

class Scope : public Node
{
private:
    std::vector<Node *> m_statements;
    std::string scope_name;
    std::map<std::string, std::string> variables;

public:
    Scope() = default;
    void append_var(std::string variable, std::string type) { variables[variable] = type; }
    std::map<std::string, std::string> get_var_map() { return variables; }
    void append_statement(Node *node);
    Node *get_statement(std::size_t i);
    std::string get_scope_name() { return scope_name; }
    void set_scope_name(std::string sn) { scope_name = sn; }
    std::vector<Node *> get_statements();
    void remove_statement(Node *r)
    {
        for (auto it = m_statements.begin(); it != m_statements.end(); it++)
        {
            if (*it == r)
            {
                m_statements.erase(it);
                return;
            }
        }
    }

    void accept(Visitor &visitor) override;
    ~Scope()
    {
        for (std::size_t i = 0; i < m_statements.size(); i++)
        {
            delete m_statements[i];
        }
    }
};

class Variable : public Node
{
private:
    std::string fragment_data = "";
    std::string m_var_name = "";
    bool is_lit = false;
    std::string m_var_type = "";
    std::string expr_type = "";
    std::string ctx_type = "";
    std::string func_name = "";

public:
    Variable() = default;
    std::string get_frag() { return fragment_data; }
    void set_frag(std::string f) { fragment_data = f; }
    std::string get_ctx_type() { return ctx_type; }
    void set_ctx_type(std::string s) { ctx_type = s; }
    std::string get_expr_type() { return expr_type; }
    void set_expr_type(std::string s) { expr_type = s; }
    std::string get_var_name() { return m_var_name; }
    void set_var_name(std::string s) { m_var_name = s; }
    std::string get_var_type() { return m_var_type; }
    void set_var_type(std::string s) { m_var_type = s; }
    std::string get_func_name() { return func_name; }
    void set_func_name(std::string s) { func_name = s; }
    void set_literal(bool l) { is_lit = l; }
    bool is_literal() { return is_lit; }
    void accept(Visitor &visitor) override;
    ~Variable() = default;
};

class FuncCall : public Node
{
private:
    std::string m_func_name;
    std::vector<Args *> args;
    std::vector<std::pair<std::string, std::string>> vector_args;

public:
    FuncCall() = default;

    std::string &func_name();
    std::vector<Args *> get_args() { return args; }
    Args *get_arg(std::size_t i) { return args[i]; }
    void set_args(Args *a, size_t i) { args[i] = a; }
    void append_arg(Args *a) { args.push_back(a); }
    void append_args_to_vector(std::string name, std::string type) { vector_args.push_back(std::make_pair(name, type)); }
    std::vector<std::pair<std::string, std::string>> get_args_from_vector() { return vector_args; }
    void accept(Visitor &visitor) override;
    ~FuncCall()
    {
        for (std::size_t i = 0; i < args.size(); i++)
        {
            delete args[i];
        }
    }
};

class Assign : public Node
{
private:
    Variable *lvalue = nullptr;
    Variable *rvalue1 = nullptr;
    Variable *rvalue2 = nullptr;
    FuncCall *funccall = nullptr;
    std::string oper;

public:
    Assign() = default;
    void set_lvalue(Variable *a) { lvalue = a; }
    void set_rvalue1(Variable *a) { rvalue1 = a; }
    void set_rvalue2(Variable *a) { rvalue2 = a; }
    void set_funccall(FuncCall *f) { funccall = f; }
    void set_oper(std::string o) { oper = o; }

    Variable *get_lvalue() { return lvalue; }
    Variable *get_rvalue1() { return rvalue1; }
    Variable *get_rvalue2() { return rvalue2; }
    FuncCall *get_funccall() { return funccall; }
    std::string get_oper() { return oper; }

    void accept(Visitor &visitor) override;

    ~Assign()
    {
        delete lvalue;
        delete rvalue1;
        delete rvalue2;
        delete funccall;
    }
};

class Program : public Node
{
    CSClass *class_ptr;

public:
    Program() = default;
    CSClass *get_class() { return class_ptr; }
    void accept(Visitor &visitor) override;
};

class CSClass : public Node
{
private:
    std::vector<Node *> m_children;

public:
    CSClass() = default;

    void append_child(Node *node);
    const Node *get_child(std::size_t i) const;

    std::vector<Node *> get_children();
    void accept(Visitor &visitor) override;
    void remove_child(Node *r)
    {
        for (auto it = m_children.begin(); it != m_children.end(); it++)
        {
            if (*it == r)
            {
                m_children.erase(it);
                return;
            }
        }
    }
    ~CSClass()
    {
        for (std::size_t i = 0; i < m_children.size(); i++)
        {
            delete m_children[i];
        }
    }
};

class Read : public Node
{
    std::string type = "";
    std::string name = "";

public:
    Read() = default;
    std::string get_type() { return type; }
    std::string get_name() { return name; }
    void set_type(std::string t) { type = t; }
    void set_name(std::string n) { name = n; }
    void accept(Visitor &visitor) override;
    ~Read() = default;
};

class ForCond : public Node
{
    std::string first = "";
    std::string second = "";
    std::string op = "";
    bool is_lit = false;

public:
    ForCond() = default;
    void set_first(std::string f) { first = f; }
    void set_second(std::string s) { second = s; }
    void set_op(std::string o) { op = o; }
    void set_literal(bool l) { is_lit = l; }

    std::string get_first() { return first; }
    std::string get_second() { return second; }
    std::string get_op() { return op; }
    bool is_literal() { return is_lit; }
    void accept(Visitor &visitor) override;
    ~ForCond() = default;
};

class ForOp : public Node
{
    Assign *for_op_assign = nullptr;
    std::string id = "";
    std::string unary_op = "";

public:
    ForOp() = default;
    void set_assign(Assign *a) { for_op_assign = a; }
    void set_id(std::string i) { id = i; }
    void set_unary_op(std::string uo) { unary_op = uo; }

    Assign *get_assign() { return for_op_assign; }
    std::string get_id() { return id; }
    std::string get_unary_op() { return unary_op; }
    void accept(Visitor &visitor) override;

    ~ForOp() { delete for_op_assign; }
};

class For : public Node
{
    Assign *assign;
    ForCond *condition;
    ForOp *op;
    Scope *scope;

public:
    For() = default;
    void set_assing(Assign *a) { assign = a; }
    void set_cond(ForCond *c) { condition = c; }
    void set_op(ForOp *o) { op = o; }
    void set_scope(Scope *s) { scope = s; }

    Assign *get_assing() { return assign; }
    ForCond *get_cond() { return condition; }
    ForOp *get_op() { return op; }
    Scope *get_scope() { return scope; }
    void accept(Visitor &visitor) override;
    ~For()
    {
        delete assign;
        delete condition;
        delete op;
        delete scope;
    }
};

class Return : public Node
{
private:
    std::string return_value = "";
    std::string type = "";
    bool is_lit = false;

public:
    Return() = default;
    bool is_literal() { return is_lit; }
    std::string get_return_value() { return return_value; }
    void set_return_value(std::string rv) { return_value = rv; }
    std::string get_return_type() { return type; }
    void set_return_type(std::string t) { type = t; }
    void set_literal(bool l) { is_lit = l; }
    ~Return() = default;

    void accept(Visitor &visitor) override;
};

class Function : public Node
{
    using Pars = std::vector<std::pair<std::string, std::string>>;

private:
    std::string m_func_name;
    std::string m_return_type;
    Pars pars;
    std::vector<Variable *> params;
    Return *m_return = nullptr;
    Scope *m_scope = nullptr;

public:
    Function() = default;
    std::string &return_type();
    std::string &func_name();
    void set_scope(Scope *sc)
    {
        m_scope = sc;
    }
    Scope *get_scope()
    {
        return m_scope;
    }
    void set_return(Return *r)
    {
        m_return = r;
    }
    Return *get_return()
    {
        return m_return;
    }

    void accept(Visitor &visitor) override;
    ~Function()
    {
        delete m_scope;
        delete m_return;
    }

    std::vector<Variable *> get_params() { return params; }

    void append_param(Variable *p) { params.push_back(p); }
    void append_param_to_vector(std::string name, std::string type) { pars.push_back(std::make_pair(name, type)); }
    Pars get_param_from_vector() { return pars; }
};

class Else : public Node
{
    Scope *m_scope = nullptr;

public:
    Else() = default;
    void set_scope(Scope *sc) { m_scope = sc; }
    Scope *get_scope() { return m_scope; }

    void accept(Visitor &visitor) override;
    ~Else()
    {
        delete m_scope;
    }
};

class If : public Node
{
    std::string first = "";
    std::string first_type = "";
    std::string second = "";
    std::string second_type = "";
    bool is_lit = false;
    std::string op = "";
    Scope *m_scope = nullptr;
    Else *else_statement = nullptr;

public:
    If() = default;
    void set_else(Else *es) { else_statement = es; }
    void set_first(std::string s) { first = s; }
    void set_first_type(std::string s) { first_type = s; }
    void set_second(std::string s) { second = s; }
    void set_second_type(std::string s) { second_type = s; }
    void set_op(std::string s) { op = s; }

    void set_literal(bool l) { is_lit = l; }
    bool is_literal() { return is_lit; }

    Else *get_else() { return else_statement; }
    std::string get_first() { return first; }
    std::string get_first_type() { return first_type; }
    std::string get_second() { return second; }
    std::string get_second_type() { return second_type; }
    std::string get_op() { return op; }

    void set_scope(Scope *sc) { m_scope = sc; }
    Scope *get_scope() { return m_scope; }

    void accept(Visitor &visitor) override;

    ~If() { delete m_scope; }
};

class Kw : public Node
{
    std::string kw = "";

public:
    Kw() = default;
    std::string get_kw() { return kw; }
    void set_kw(std::string k) { kw = k; }
    void accept(Visitor &visitor) override;
    ~Kw() = default;
};

class Print : public Node
{
    std::string type = "";
    std::string name = "";

public:
    Print() = default;
    std::string get_type() { return type; }
    std::string get_name() { return name; }
    void set_type(std::string t) { type = t; }
    void set_name(std::string n) { name = n; }
    void accept(Visitor &visitor) override;
    ~Print() = default;
};*/

}  // namespace csharp::ast
