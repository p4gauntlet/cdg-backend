#include "functionDeclaration.h"

#include "baseType.h"
#include "blockStatement.h"
#include "expression.h"
#include "parameterList.h"
#include "scope.h"

namespace CODEGEN {

IR::Type *functionDeclaration::gen_return_type() {
    std::vector<int64_t> percent = {PCT.FUNCTIONDECLARATION_TYPE_BASE,
                                    PCT.FUNCTIONDECLARATION_TYPE_STRUCT,
                                    PCT.FUNCTIONDECLARATION_TYPE_VOID};
    IR::Type *tp = nullptr;
    std::vector<int64_t> type_probs = {
        PCT.FUNCTIONDECLARATION_BASETYPE_BOOL,
        PCT.FUNCTIONDECLARATION_BASETYPE_ERROR,
        PCT.FUNCTIONDECLARATION_BASETYPE_INT,
        PCT.FUNCTIONDECLARATION_BASETYPE_STRING,
        PCT.FUNCTIONDECLARATION_BASETYPE_BIT,
        PCT.FUNCTIONDECLARATION_BASETYPE_SIGNED_BIT,
        PCT.FUNCTIONDECLARATION_BASETYPE_VARBIT};
    switch (randind(percent)) {
    case 0: {
        tp = baseType::pick_rnd_base_type(type_probs);
        break;
    }
    case 1: {
        auto l_types = P4Scope::get_decls<IR::Type_StructLike>();
        if (l_types.size() == 0) {
            return nullptr;
        }
        auto candidate_type = l_types.at(get_rnd_int(0, l_types.size() - 1));
        auto type_name = candidate_type->name.name;
        // check if struct is forbidden
        if (P4Scope::not_initialized_structs.count(type_name) == 0) {
            tp = new IR::Type_Name(candidate_type->name.name);
        } else {
            tp = baseType::pick_rnd_base_type(type_probs);
        }
        break;
    }
    case 2: {
        // return a void type
        tp = new IR::Type_Void();
        break;
    }
    }
    return tp;
}

IR::Function *functionDeclaration::gen() {
    cstring name = randstr(7);
    IR::Type_Method *tm = nullptr;
    IR::BlockStatement *blk = nullptr;
    P4Scope::start_local_scope();
    IR::ParameterList *params = parameterList::gen();

    IR::Type *r_tp = gen_return_type();
    tm = new IR::Type_Method(r_tp, params);

    P4Scope::prop.ret_type = r_tp;
    blk = blockStatement::gen(true);
    P4Scope::prop.ret_type = nullptr;

    auto ret = new IR::Function(name, tm, blk);
    P4Scope::end_local_scope();
    P4Scope::add_to_scope(ret);
    return ret;
}

} // namespace CODEGEN
