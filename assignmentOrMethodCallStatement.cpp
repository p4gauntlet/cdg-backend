#include "assignmentOrMethodCallStatement.h"
#include "expression_2.h"

namespace CODEGEN {
IR::AssignmentStatement *assignmentOrMethodCallStatement::gen_assign(
    bool if_in_parser = false) {
    IR::AssignmentStatement *assignstat = nullptr;
    IR::Expression *left = nullptr, *right = nullptr;
    const IR::Type *l_tp, *r_tp;

    std::vector<int> percent = { 75, 25 };

    switch (randind(percent)) {
        case 0:
            left  = expression::get_bit_operand(&l_tp, false);
            if (left != nullptr) {
                right = expression2::gen_expr(l_tp->to<IR::Type_Bits>());
                // if (!if_in_parser) {
                //     r_tp = expression::mp_expr_2_type[right];
                //     int l_size = l_tp->to<IR::Type_Bits>()->size;
                //     int r_size = r_tp->to<IR::Type_Bits>()->size;
                //     if (l_size != r_size) {
                //         auto right_cast = new IR::Cast(
                //             new IR::Type_Bits(l_size, false), right);
                //         assignstat = new IR::AssignmentStatement(left,
                //                                                  right_cast);
                //     } else
                assignstat = new IR::AssignmentStatement(left, right);
                // }
            }
            break;
        case 1:
            // compund means it is not a simple operator, i.e., bit<128> a is simple, compound may be struct, header
            if (expression::get_two_compound_operands(&left, &right,
                                                      &l_tp,
                                                      &r_tp) == true) {
                assignstat = new IR::AssignmentStatement(left, right);
            }
            break;
    }


    return assignstat;
}


// // TODO: methodcall statement is a big headache
// static IR::MethodCallStatement *gen_methodcall_stat() {
//     IR::MethodCallStatement *mcs = nullptr;

//     if (P4Scope::decl_ins_ctrls.size() == 0) {
//         return nullptr;
//     }

//     size_t ind = rand() % P4Scope::decl_ins_ctrls.size();
//     size_t cnt = 0;
//     cstring name;
//     IR::P4Control *p4_ctrl;
//     for (auto& i : P4Scope::decl_ins_ctrls) {
//         if (cnt == ind) {
//             name    = i.first;
//             p4_ctrl = i.second;
//         }
//         cnt++;
//     }
//     std::vector<const IR::Type *> params;
//     for (size_t i = 0; i < p4_ctrl->type->applyParams->parameters.size();
//          i++) {
//         auto par = p4_ctrl->type->applyParams->parameters.at(i);
//         params.push_back(par->type);
//     }
//     IR::Vector<IR::Argument> *args = expression::construct_params(params);
//     if (args->size() != params.size()) {
//         return nullptr;
//     }
//     IR::Member *mem =
//         new IR::Member(new IR::PathExpression(name), "apply");
//     mcs = new IR::MethodCallStatement(new IR::MethodCallExpression(mem,
//                                                                    args));

//     return mcs;
// }

IR::MethodCallStatement *assignmentOrMethodCallStatement::gen_methodcall() {
    IR::MethodCallExpression *mce = nullptr;
    std::vector<int> percent      = { 40, 40, 20 };

    switch (randind(percent)) {
        case 0: {
                auto actions = P4Scope::get_action_decls();
                if (actions.size() == 0) {
                    return nullptr;
                }

                size_t idx   = rand() % actions.size();
                auto *p4_fun = actions.at(idx);
                std::vector<const IR::Type *> params;
                for (auto par: p4_fun->getParameters()->parameters) {
                    params.push_back(par->type);
                }
                IR::Vector<IR::Argument> *args =
                    expression::construct_params(params);
                if (args->size() != params.size()) {
                    return nullptr;
                }
                auto path_expr = new IR::PathExpression(p4_fun->name);
                mce = new IR::MethodCallExpression(path_expr, args);
                break;
            }
        case 1: {
                auto funcs = P4Scope::get_func_decls();
                if (funcs.size() == 0) {
                    return nullptr;
                }

                size_t idx   = rand() % funcs.size();
                auto *p4_fun = funcs.at(idx);
                std::vector<const IR::Type *> params;
                for (auto par: p4_fun->getParameters()->parameters) {
                    params.push_back(par->type);
                }
                IR::Vector<IR::Argument> *args =
                    expression::construct_params(params);
                if (args->size() != params.size()) {
                    return nullptr;
                }
                auto path_expr = new IR::PathExpression(p4_fun->name);
                mce = new IR::MethodCallExpression(path_expr, args);
                break;
            }
        case 2: {
                auto tbl_set = P4Scope::get_callable_tables();
                // return nullptr if there are no tables left
                if (tbl_set->size() == 0) {
                    return nullptr;
                }
                auto idx      = rand() % tbl_set->size();
                auto tbl_iter = std::begin(*tbl_set);
                std::advance(tbl_iter, idx);
                const IR::P4Table *tbl = *tbl_iter;
                auto mem =
                    new IR::Member(new IR::PathExpression(tbl->name),
                                   "apply");
                mce = new IR::MethodCallExpression(mem);
                tbl_set->erase(tbl_iter);
                break;
            }
    }
    if (mce)
        return new IR::MethodCallStatement(mce);
    else
        return nullptr;
}


IR::Statement *assignmentOrMethodCallStatement::gen() {
    std::vector<int> percent = { 50, 50 };
    auto val = randind(percent);

    if (val == 0) {
        return gen_assign();
    } else {
        return gen_methodcall();
    }
}
} // namespace CODEGEN
