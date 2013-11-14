#pragma once
#include "Visitor.h"

class CSymbolTableBuilder: public IVisitor {
public:
	int Visit( const CProgram* n );
	int Visit( const CMainClass* n );
	int Visit( const CClassDeclareStar* n );
	int Visit( const CClassDeclare* n );
	int Visit( const CClassDeclareExtends* n );
	int Visit( const CVarDeclareStar* n );
	int Visit( const CVarDeclare* n );
	int Visit( const CMethodDeclare* n );
	int Visit( const CMethodDeclareStar* n );
	int Visit( const CFormalList* n );
	int Visit( const CFormalRestStar* n );
	int Visit( const CStatement* n );
	int Visit( const CStatementStar* n );
	int Visit( const CStatementIf* n );
	int Visit( const CStatementWhile* n );
	int Visit( const CStatementSysOut* n );
	int Visit( const CStatementAssignment* n );
	int Visit( const CStatementArrayAssignment* n );
	int Visit( const CExpressionBinOp* n );
	int Visit( const CExpressionArray* n );
	int Visit( const CExpressionLength* n );
	int Visit( const CExpressionCallMethod* n );
	int Visit( const CExpressionNumber* n );
	int Visit( const CExpressionBool* n );
	int Visit( const CExpressionVar* n );
	int Visit( const CExpressionThis* n );
	int Visit( const CExpressionNewInt* n );
	int Visit( const CExpressionNewId* n );
	int Visit( const CExpressionNegation* n );
	int Visit( const CExpression* n );
	int Visit( const CExpList* n );
	int Visit( const CExpListNext* n );
};