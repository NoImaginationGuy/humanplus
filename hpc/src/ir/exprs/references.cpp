// => src/ir/exprs/references.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/ir/builders.h>
#include <hpc/ast/decls/class.h>
#include <hpc/ast/exprs/reference.h>
#include <hpc/ast/exprs/members.h>

#include <llvm/IR/Value.h>

using namespace hpc;

void codegen::ModuleBuilder::visitVarRef(ast::VarRef *varRef) {
    table.setValForComponent(varRef, builder->CreateLoad(table.getValForComponent(varRef->getVar()))); // FIXME alignment
}

void codegen::ModuleBuilder::visitFunctionCall(ast::FunctionCall *functionCall) {
    std::vector<llvm::Value *> builtArgs;
    for (ast::Expr *param : functionCall->getActualParams()) builtArgs.push_back(build(param));

    ast::FunctionDecl *prototypeFunction = functionCall->getFunctionDecl();
    
    build(prototypeFunction);
    
    llvm::Function *irPrototype = static_cast<llvm::Function *>(table.getValForComponent(prototypeFunction));
    table.setValForComponent(functionCall, builder->CreateCall(irPrototype, builtArgs));
}

void codegen::ModuleBuilder::visitFieldRef(ast::FieldRef *fieldRef) {
    table.setValForComponent(fieldRef, builder->CreateLoad(table.getOrCreateReference(fieldRef))); // FIXME alignment
}
