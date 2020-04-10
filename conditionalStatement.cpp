#include "conditionalStatement.h"
#include "expression.h"
#include "statement.h"

namespace CODEGEN {
IR::IfStatement *conditionalStatement::gen_if_stat(bool if_in_func) {
    IR::Expression *cond = nullptr;
    IR::Statement *if_true = nullptr, *if_false = nullptr;

    cond = expression::gen_expr(new IR::Type_Boolean());
    if (not cond) {
        BUG("cond in IfStatement should not be nullptr!");
    }
    if_true = statement::gen_rnd(if_in_func);
    if (not if_true) {
        BUG("if_true in IfStatement should not be nullptr!");
    }
    if_false = statement::gen_rnd(if_in_func);
    if (not if_false) {
        BUG("if_false in IfStatement should not be nullptr!");
    }
    return  new IR::IfStatement(cond, if_true, if_false);;
}
} // namespace CODEGEN
