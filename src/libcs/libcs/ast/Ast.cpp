#include <libcs/ast/Ast.hpp>
#include <libcs/ast/Visitor.hpp>

namespace csharp::ast {
    void CSClass::accept(Visitor &visitor) { visitor.visit(*this); }
    void Expressions::accept(Visitor &visitor) { visitor.visit(*this); }
    /*
    void Assign_statement::accept(Visitor &visitor) { visitor.visit(*this); }
    void Literal::accept(Visitor &visitor) { visitor.visit(*this); }
    void Var_def::accept(Visitor &visitor) { visitor.visit(*this); }
    void Func_def::accept(Visitor &visitor) { visitor.visit(*this); }
    void Scope::accept(Visitor &visitor) { visitor.visit(*this); }
    void Return_statement::accept(Visitor &visitor) { visitor.visit(*this); }
    void Statement::accept(Visitor &visitor) { visitor.visit(*this); }
    void Func_call::accept(Visitor &visitor) { visitor.visit(*this); }
    void Args::accept(Visitor &visitor) { visitor.visit(*this); }
    void Arg::accept(Visitor &visitor) { visitor.visit(*this); }
    void Pars::accept(Visitor &visitor) { visitor.visit(*this); }
    void If_statement::accept(Visitor &visitor) { visitor.visit(*this); }
    void Else_statement::accept(Visitor &visitor) { visitor.visit(*this); }
    void For_statement::accept(Visitor &visitor) { visitor.visit(*this); }
    void For_condition::accept(Visitor &visitor) { visitor.visit(*this); }
    void For_operation::accept(Visitor &visitor) { visitor.visit(*this); }
    void Kw_statement::accept(Visitor &visitor) { visitor.visit(*this); }
    void Print_statement::accept(Visitor &visitor) { visitor.visit(*this); }
    void Read_statement::accept(Visitor &visitor) { visitor.visit(*this); }
    */
}  // namespace csharp::ast
