#include "Interpreter.h"
#include "miniJava.h"
#include <iostream>

int CInterpreter::Visit( const CProgram* n )
{
	n->GetMainClass()->Accept( this );
	n->GetClassDeclareStar()->Accept( this );
	return 0;
}

int CInterpreter::Visit( const CMainClass* n )  { return 0; }

int CInterpreter::Visit( const CClassDeclareStar* n )  { return 0; }

int CInterpreter::Visit( const CClassDeclare* n )  { return 0; }

int CInterpreter::Visit( const CClassDeclareExtends* n )  { return 0; }

int CInterpreter::Visit( const CVarDeclareStar* n )  { return 0; }	

int CInterpreter::Visit( const CVarDeclare* n )  { return 0; }

int CInterpreter::Visit( const CMethodDeclare* n )  { return 0; }

int CInterpreter::Visit( const CMethodDeclareStar* n )  { return 0; }

int CInterpreter::Visit( const CFormalList* n )  { return 0; }

int CInterpreter::Visit( const CFormalRestStar* n )  { return 0; }

int CInterpreter::Visit( const CStatement* n )  { return 0; }

int CInterpreter::Visit( const CStatementStar* n )  { return 0; }

int CInterpreter::Visit( const CStatementIf* n )  { return 0; }

int CInterpreter::Visit( const CStatementWhile* n )  { return 0; }

int CInterpreter::Visit( const CStatementSysOut* n )  { return 0; }

int CInterpreter::Visit( const CStatementAssignment* n )  { return 0; }

int CInterpreter::Visit( const CStatementArrayAssignment* n )  { return 0; }

int CInterpreter::Visit( const CExpressionBinOp* n )  { return 0; }

int CInterpreter::Visit( const CExpressionArray* n )  { return 0; }

int CInterpreter::Visit( const CExpressionLength* n )  { return 0; }

int CInterpreter::Visit( const CExpressionCallMethod* n )  { return 0; }

int CInterpreter::Visit( const CExpressionNumber* n )  { return 0; }

int CInterpreter::Visit( const CExpressionBool* n )  { return 0; }

int CInterpreter::Visit( const CExpressionVar* n )  { return 0; }

int CInterpreter::Visit( const CExpressionThis* n )  
{ 
	std::cout << "this";
	return 0; 
}

int CInterpreter::Visit( const CExpressionNewInt* n )  { return 0; }

int CInterpreter::Visit( const CExpressionNewId* n )  { return 0; }

int CInterpreter::Visit( const CExpressionNegation* n )  { return 0; }

int CInterpreter::Visit( const CExpression* n )  { return 0; }

int CInterpreter::Visit( const CExpList* n )  { return 0; }

int CInterpreter::Visit( const CExpListNext* n )  { return 0; }