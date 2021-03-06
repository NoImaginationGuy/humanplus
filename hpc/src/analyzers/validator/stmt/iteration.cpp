// => src/analyzers/validator/stmt/iteration.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/validator/validator.h>
#include <hpc/ast/stmt/whileuntil.h>
#include <hpc/ast/stmt/for.h>

using namespace hpc;

void validator::ValidatorImpl::visitSimpleIterStmt(ast::SimpleIterStmt *statement) {
    getResolver().getInnermostStack().pushBreakCatcher(statement);
    getResolver().getInnermostStack().pushContinueCatcher(statement);
    
    if (!validate(statement->getCondition())) {
        statement->resignValidation();
    }
    
    if (ast::Stmt *block = statement->getBlock()) {
        if (!validate(block)) {
            statement->resignValidation();
        }
    }
    
    getResolver().getInnermostStack().popContinueCatcher();
    getResolver().getInnermostStack().popBreakCatcher();
}

void validator::ValidatorImpl::visitForStmt(ast::ForStmt *statement) {
    getResolver().getInnermostStack().addScope();
    
    getResolver().getInnermostStack().pushBreakCatcher(statement);
    getResolver().getInnermostStack().pushContinueCatcher(statement);
    
    for (ast::Stmt *initStmt : statement->getInitStatements()) {
        if (!validate(initStmt)) {
            statement->resignValidation();
        }
    }
    
    if (statement->isValid()) {

        if (!validate(statement->getCondition())) {
            statement->resignValidation();
        }
        
        for (ast::Stmt *endStmt : statement->getEndStatements()) {
            if (!validate(endStmt)) {
                statement->resignValidation();
            }
        }
        
        if (ast::Stmt *block = statement->getBlock()) {
            if (!validate(block)) {
                statement->resignValidation();
            }
        }
    }
    
    getResolver().getInnermostStack().popContinueCatcher();
    getResolver().getInnermostStack().popBreakCatcher();
    
    getResolver().getInnermostStack().removeScope();
}
