// => src/analyzers/validator/exprs/unary.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/analyzers/validator/validator.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/ast/exprs/castings.h>
#include <hpc/ast/exprs/unary.h>

using namespace hpc;

void validator::ValidatorImpl::visitArithmeticNegationExpr(ast::ArithmeticNegationExpr *expression) {
    if (validate(expression->getOperand())) {
        ast::Type *operandTy = expression->getOperand()->evalType();
        
        if (!operandTy->isNumericType()) {
            validator.getDiags().reportError(diag::InvalidExpressionToUnaryOperation, expression->tokenRef(ast::PointToOperator)) << operandTy->asString();
            expression->resignValidation();
        }
        
    } else {
        expression->resignValidation();
    }
}

void validator::ValidatorImpl::visitLogicalNegationExpr(ast::LogicalNegationExpr *expression) {
    expression->castOperandToBoolean();
    
    if (!validate(expression->getOperand())) expression->resignValidation();
}

void validator::ValidatorImpl::visitBitwiseNegationExpr(ast::BitwiseNegationExpr *expression) {
    
    if (validate(expression->getOperand())) {
        ast::Type *operandTy = expression->getOperand()->evalType();
        
        if (!operandTy->isIntegerType()) {
            validator.getDiags().reportError(diag::InvalidExpressionToUnaryOperation, expression->tokenRef(ast::PointToOperator)) << operandTy->asString();
            expression->resignValidation();
        }
    } else {
        expression->resignValidation();
    }
}
