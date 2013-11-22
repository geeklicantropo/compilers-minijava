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

int CTypeChecker::Visit( const CStatementSysOut* n ) 
{
	n->GetExpression()->Accept( this );
	assert( currentType != 0 );
	if( currentType->GetType() != INT )
		ErrorMessage( cout, "int type expected", n->GetLocation() );
	return 0;
}

int CTypeChecker::Visit( const CStatementAssignment* n )
{
	n->GetExpression()->Accept( this );
	const CTypeInfo* varType = currentMethod->LookUp( n->GetId() )->GetType();
	bool error = false;
	assert( currentType != 0 );
	if( varType->GetType() != currentType->GetType() ) {
		error = true;
	} else if( varType->GetType() == USERTYPE && varType->GetUserType() != currentType->GetUserType() ) {
		error = true;
	}
	if( error )
		ErrorMessage( cout, "wrong type in assigment", n->GetLocation() );
	return 0;
}
int CTypeChecker::Visit( const CStatementArrayAssignment* n )
{
	assert(currentMethod != 0 );
	const CTypeInfo* varType = currentMethod->LookUp( n->GetId() )->GetType();
	if( varType->GetType() != INTARRAY ) 
		ErrorMessage( cout, "variable should be int[] type", n->GetLocation() );
	n->GetExpressionArray()->Accept( this );
	assert( currentType != 0 );
	if( currentType->GetType() != INT )
		ErrorMessage( cout, "array index should be int type", n->GetLocation() );
	n->GetExpression()->Accept( this );
	assert( currentType != 0 );
	if( currentType->GetType() != INT )
		ErrorMessage( cout, "left value should be int type", n->GetLocation() );
	return 0;
}

int CTypeChecker::Visit( const CExpressionBinOp* n ) 
{
	TBinaryOperation type =  n->GetOperation();
	switch( type ) {
		case PLUS:
			n->GetExpressionFirst()->Accept( this );
			assert( currentType != 0 );
			if( currentType->GetType() != INT )
				ErrorMessage( cout, "int type expected", n->GetLocation() );
			n->GetExpressionSecond()->Accept( this );
			assert( currentType != 0 );
			if( currentType->GetType() != INT )
				ErrorMessage( cout, "int type expected", n->GetLocation() );
			currentType = new CTypeInfo( INT );
			break;
		case MINUS:
			n->GetExpressionFirst()->Accept( this );
			assert( currentType != 0 );
			if( currentType->GetType() != INT )
				ErrorMessage( cout, "int type expected", n->GetLocation() );
			n->GetExpressionSecond()->Accept( this );
			assert( currentType != 0 );
			if( currentType->GetType() != INT )
				ErrorMessage( cout, "int type expected", n->GetLocation() );
			currentType = new CTypeInfo( INT );
			break;
		case TIMES:
			n->GetExpressionFirst()->Accept( this );
			assert( currentType != 0 );
			if( currentType->GetType() != INT )
				ErrorMessage( cout, "int type expected", n->GetLocation() );			
			n->GetExpressionSecond()->Accept( this );
			assert( currentType != 0 );
			if( currentType->GetType() != INT )	
				ErrorMessage( cout, "int type expected", n->GetLocation() );		
			currentType = new CTypeInfo( INT );
			break;
		case LESS:
			n->GetExpressionFirst()->Accept( this );
			assert( currentType != 0 );
			if( currentType->GetType() != BOOL )
				ErrorMessage( cout, "int type expected", n->GetLocation() );			
			n->GetExpressionSecond()->Accept( this );
			assert( currentType != 0 );
			if( currentType->GetType() != BOOL )	
				ErrorMessage( cout, "int type expected", n->GetLocation() );			
			currentType = new CTypeInfo( BOOL );
			break;
		case AND:
			n->GetExpressionFirst()->Accept( this );
			assert( currentType != 0 );
			if( currentType->GetType() != BOOL )
				ErrorMessage( cout, "int type expected", n->GetLocation() );			
			n->GetExpressionSecond()->Accept( this );
			assert( currentType != 0 );
			if( currentType->GetType() != BOOL )	
				ErrorMessage( cout, "int type expected", n->GetLocation() );	
			currentType = new CTypeInfo( BOOL );
			break;
	}
	return 0;
}

int CTypeChecker::Visit( const CExpressionArray* n )
{
	n->GetExpression1()->Accept( this );
	assert( currentType != 0 );
	if( currentType->GetType() != INTARRAY )
		ErrorMessage( cout, "int[] type expected", n->GetLocation() );
	n->GetExpression2()->Accept(  this );
	assert( currentType != 0 );
	if( currentType->GetType() != INT )
		ErrorMessage( cout, "index should be int type", n->GetLocation() );
	currentType = new CTypeInfo( INT );
	return 0;
}

int CTypeChecker::Visit( const CExpressionLength* n ) 
{
	n->GetExpression()->Accept( this );
	assert( currentType != 0 );
	if( currentType->GetType() != INTARRAY )
		ErrorMessage( cout, "int[] type expected", n->GetLocation() );			
	currentType = new CTypeInfo( INT );
	return 0;
}
int CTypeChecker::Visit( const CExpressionCallMethod* n ) 
{
	n->GetExpression()->Accept( this );
	if( currentType->GetType() != USERTYPE )
		ErrorMessage( cout, "usertype expected", n->GetLocation() );
	if( currentClass->LookUpMethod( n->GetId() ) == NULL )
		ErrorMessage( cout, "method doesn't exist", n->GetLocation() );
	if( n->GetExpList() != 0 ) n->GetExpList()->Accept( this ); 
	return 0;
}
int CTypeChecker::Visit( const CExpressionNumber* n ) 
{
	return 0;
}
int CTypeChecker::Visit( const CExpressionBool* n ) 
{
	return 0;
}
int CTypeChecker::Visit( const CExpressionVar* n ) 
{
	return 0;
}
int CTypeChecker::Visit( const CExpressionThis* n ) 
{
	return 0;
}
int CTypeChecker::Visit( const CExpressionNewInt* n ) 
{
	n->GetExpression()->Accept( this );
	assert( currentType != 0 );
	if( currentType->GetType() != INT )
		ErrorMessage( cout, "index should be int type", n->GetLocation() );
	currentType = new CTypeInfo( INT );
	return 0;
}
int CTypeChecker::Visit( const CExpressionNewId* n ) 
{
	if( symbolTable->LookUpClass( n->GetId() ) == NULL )
		ErrorMessage( cout, "type doesn't exist", n->GetLocation() );
	return 0;
}
int CTypeChecker::Visit( const CExpressionNegation* n ) 
{
	n->GetExpression()->Accept( this );
	if( currentType->GetType() != BOOL )
		ErrorMessage( cout, "bool type expected", n->GetLocation() );
	currentType = new CTypeInfo( BOOL );
	return 0;
}
int CTypeChecker::Visit( const CExpression* n )
{
	n->GetExpression()->Accept( this );
	return 0;
}
int CTypeChecker::Visit( const CExpList* n ) 
{
	n->GetExpression()->Accept( this );
	return 0;
}
int CTypeChecker::Visit( const CExpListNext* n )
{
	n->GetExpList()->Accept( this );
	n->GetExpression()->Accept( this );
	//if( currentType->GetType() 
	return 0;
}