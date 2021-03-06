// => hpc/ast/component.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_ast_component
#define __human_plus_compiler_ast_component

#include <hpc/analyzers/sources.h>

#include <map>

#include <llvm/Support/Casting.h>

#define llvm_rtti_impl(k) \
    virtual ASTComponentKind getKind() const { return ASTCK_##k; } \
    template <class T> static bool classof(const T *c) { return c->getKind() == ASTCK_##k; }

#define llvm_rtti_impl_superclass(k) \
    virtual ast::ASTComponentKind getKind() const { return ASTCK_##k; } \
    template <class T> static bool classof(const T *c) { \
        ast::ASTComponentKind idt = c->getKind();\
        return ASTCK_##k <= idt && idt < ASTCKSUBEND_##k; \
    }


namespace hpc {
    namespace validator {
        struct ValidationSession;
    }
    
    namespace ast {
        
        /*!
         \brief enum used for llvm RTTI implementation.
         */
        typedef enum : unsigned {
            ASTCK_Component = 0,
            ASTCK_Symbol,

#define __ast_node(class, superclass) ASTCK_##class,
#define __ast_begin_subclass(class)
#define __ast_end_subclass(class) ASTCKSUBEND_##class,
            
#include <hpc/inc/ast/typenodes.inc>
#include <hpc/inc/ast/declnodes.inc>
#include <hpc/inc/ast/stmtnodes.inc>
            
#undef __ast_node
#undef __ast_begin_subclass
#undef __ast_end_subclass
            
            ASTCKSUBEND_Component
        } ASTComponentKind;
        
        /*!
         \brief Values to use with components token refs. Each of these values indicates a specific \c source::TokenRef object for a source code that composed an \c ast::Component
         \note Not all these values will exist in every Component.
         \see \c ast::Component::tokenRef() function to use this value.
         */
        typedef enum {
            /*!
             \brief Key where should be saved the pointer to the operator in binary and unary expressions.
             */
            PointToOperator,
            /*!
             \brief Key where should be saved the pointer to the closing brace \c } at the end of a function.
             */
            PointToEndOfFunction,
            /*!
             \brief Key where should be saved the pointer to the closing brace \c } at the end of a compound statement.
             */
            PointToEndOfCompoundStatement,
            /*!
             \brief Key where should be saved the pointer to a variable identifier.
             */
            PointToVariableIdentifier,
            /*!
             \brief Key where should be saved the pointer to the main keyword of the statement.
             */
            PointToStatementQualifier,
            /*!
             \brief Key where should be saved the pointer to the first token that composes an expression.
             */
            PointToBeginOfExpression,
            /*!
             \brief Key where should be saved the pointer to the last token that composes an expression.
             */
            PointToEndOfExpression,
            /*!
             \brief Key where should be saved the pointer to the \c = symbol after a \c let declaration.
             */
            PointToInitValueIntroducer,
            /*!
             \brief Key where should be saved the pointer to the member access operator in a struct member access operation.
             */
            PointToStructAccessOperator
        } TokenRole;
        
        /*!
         \brief Base class for all the classes which compose the Abstract Syntax Tree.
         */
        class Component {

            /*!
             \brief A \c std::map object containing the \c source::TokenRef objects pointing the code which composed this \c ast::Component.
             */
            std::map<TokenRole, source::TokenRef *> tkrefs;
            
        protected:
            /*!
             \brief A boolean indicating whether this component is valid.
             \note The component is not either "valid" or "invalid" until the validator checks this object for code generation eligibility.
             */
            bool valid = true;
            
        public:
            virtual ~Component() {  };
            
            /*!
             \brief Marks the component as invalid. This is usually used by the validator when something is wrong with the component.
             */
            inline void resignValidation() {
                valid = false;
            }
            
            /*!
             \brief Returns a boolean indicating whether this component is valid.
             \note This value is \c true until \c resignValidation() is called on this object to allow the compiler to add implicit AST components without validating them, but you <b>must</b> always validate an AST component generated by the user's code.
             */
            inline bool isValid() const {
                return valid;
            }
            
            /*!
             \brief Copies the given \c source::TokenRef and bounds it to the current component, with a specific \c TokenRole, then returns it.
             \param i The role the part of code pointed by \c tkref had generating the component
             */
            source::TokenRef *tokenRef(TokenRole i, source::TokenRef &tkref) { return tkrefs[i] = new source::TokenRef(tkref); }
            /*!
             \brief Returns a \c source::TokenRef object which had a specific \c TokenRole when this component has been generated.
             \param i The role the part of code pointed by \c tkref had generating the component
             */
            source::TokenRef *tokenRef(TokenRole i) { return tkrefs[i]; }
            
            
            virtual ASTComponentKind getKind() const { return ASTCK_Component; }

        };
        
    }
}

#endif
