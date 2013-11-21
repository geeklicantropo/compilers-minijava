#include "TypeChecker.h"
#include "miniJava.h"
#include "iostream"

using std::cout;

CTypeChecker::CTypeChecker( CSymbolTable* st ): 
	symbolTable( st ) 
{
	currentClass = 0;
	currentMethod = 0;
	currentType = 0;
}

int CTypeChecker::Visit( const CProgram* n ) 
{
	n->GetMainClass()->Accept( this );
	if ( n->GetClassDeclareStar() != 0 ) n->GetClassDeclareStar()->Accept( this );
	return 0;
}

int CTypeChecker::Visit( const CMainClass* n ) 
{
	currentClass = symbolTable->LookUpClass( n->GetId() );
	n->GetStatement()->Accept( this );
	currentClass = 0;
	return 0;
}
int CTypeChecker::Visit( const CClassDeclareStar* n ) 
{
	if ( n->GetClassDeclareStar() != 0 ) n->GetClassDeclareStar()->Accept( this );
	n->GetClassDeclare()->Accept( this );
	return 0;
}
int CTypeChecker::Visit( const CClassDeclare* n )
{
	currentClass = symbolTable->LookUpClass( n->GetId() );
	if ( n->GetVarDeclareStar() != 0 ) n->GetVarDeclareStar()->Accept( this );
	if ( n->GetMethodDeclareStar() != 0 ) n->GetMethodDeclareStar()->Accept( this );
	currentClass = 0;
	return 0;
}
int CTypeChecker::Visit( const CClassDeclareExtends* n ) 
{
	currentClass = symbolTable->LookUpClass( n->GetId() );
	if( n->GetVarDeclareStar() != 0) n->GetVarDeclareStar()->Accept( this );
	if( n->GetMethodDeclareStar() != 0)  n->GetMethodDeclareStar()->Accept( this );
	currentClass = 0;
	return 0;
}
int CTypeChecker::Visit( const CVarDeclareStar* n ) 
{
	if( n->GetVarDeclareStar() != 0 ) n->GetVarDeclareStar()->Accept( this );
	n->GetVarDeclare()->Accept( this );
	return 0;
}
int CTypeChecker::Visit( const CVarDeclare* n ) 
{
	return 0;
}
int CTypeChecker::Visit( const CMethodDeclare* n ) 
{
	assert( currentClass != 0 );
	currentMethod = currentClass->LookUpMethod( n->GetId() );
	if( n->GetFormalList() != 0 ) n->GetFormalList()->Accept( this );
	if( n->GetVarDeclareStar() != 0 ) n->GetVarDeclareStar()->Accept( this );
	if( n->GetStatementStar() != 0 ) n->GetStatementStar()->Accept( this );
	n->GetExpression()->Accept( this );
	currentMethod = 0;
	return 0;
}
int CTypeChecker::Visit( const CMethodDeclareStar* n ) 
{
	if( n->GetMethodDeclareStar() != 0 ) n->GetMethodDeclareStar()->Accept( this );
	n->GetMethodDeclare()->Accept( this );
	return 0;
}
int CTypeChecker::Visit( const CFormalList* n ) 
{
	if( n->GetFormalRestStar() != 0 ) n->GetFormalRestStar()->Accept( this );
	return 0;
}
int CTypeChecker::Visit( const CFormalRestStar* n ) 
{
	if( n->GetFormalRestStar() != 0 ) n->GetFormalRestStar()->Accept( this );
	return 0;
}
int CTypeChecker::Visit( const CStatement* n ) 
{
	if( n->GetStatementStar() != 0 ) n->GetStatementStar()->Accept( this );
	return 0;
} 
int CTypeChecker::Visit( const CStatementStar* n ) {
	n->GetStatement()->Accept( this );
	if( n->GetStatementStar() != 0 ) n->GetStatementStar()->Accept( this );
	return 0;
}

int CTypeChecker::Visit( const CStatementIf* n )
{
	n->GetExpression()->Accept( this );
	assert( currentType != 0 );
	if( currentType->GetType() != BOOL )
		ErrorMessage( cout, "boolean type expected", n->GetLocation() );

	n->GetStatementIf()->Accept( this );
	n->GetStatementElse()->Accept( this );
	return 0;
}

int CTypeChecker::Visit( const CStatementWhile* n )
{
	n->GetExpression()->Accept( this );
	assert( currentType != 0 );
	if( currentType->GetType() != BOOL )
		ErrorMessage( cout, "boolean type expected", n->GetLocation() );

	n->GetStatement()->Accept( this );
	return 0;
}

int CTypeChecker::Visit( const CStatementSysOut* n ) {return 0;}
int CTypeChecker::Visit( const CStatementAssignment* n ) {return 0;}
int CTypeChecker::Visit( const CStatementArrayAssignment* n ) {return 0;}

int CTypeChecker::Visit( const CExpressionBinOp* n ) 
{
	return 0;
}

int CTypeChecker::Visit( const CExpressionArray* n ) {return 0;}
int CTypeChecker::Visit( const CExpressionLength* n ) {return 0;}
int CTypeChecker::Visit( const CExpressionCallMethod* n ) {return 0;}
int CTypeChecker::Visit( const CExpressionNumber* n ) {return 0;}
int CTypeChecker::Visit( const CExpressionBool* n ) {return 0;}
int CTypeChecker::Visit( const CExpressionVar* n ) {return 0;}
int CTypeChecker::Visit( const CExpressionThis* n ) {return 0;}
int CTypeChecker::Visit( const CExpressionNewInt* n ) {return 0;}
int CTypeChecker::Visit( const CExpressionNewId* n ) {return 0;}
int CTypeChecker::Visit( const CExpressionNegation* n ) {return 0;}
int CTypeChecker::Visit( const CExpression* n )
{
	//здесь то и нужно поюзать CSymbolTable
	return 0;
}
int CTypeChecker::Visit( const CExpList* n ) {return 0;}
int CTypeChecker::Visit( const CExpListNext* n ) {return 0;}