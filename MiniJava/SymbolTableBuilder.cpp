#include "SymbolTableBuilder.h"
#include "miniJava.h"
#include "iostream"

CSymbolTableBuilder::CSymbolTableBuilder( CSymbolTable* st ) :
	symbolTable( st )
{
	currentClass = 0;
	currentMethod = 0;
}

int CSymbolTableBuilder::Visit( const CProgram* n )
{
	n->GetMainClass()->Accept( this );
	if ( n->GetClassDeclareStar() != 0 ) n->GetClassDeclareStar()->Accept( this );
	return 0;
}

int CSymbolTableBuilder::Visit( const CMainClass* n )
{ 
	currentClass = symbolTable->AddClass( n->GetId() );
	if( currentClass == 0 )
		ErrorMessage( cout, "this classname was used earlier", n->GetLocation() );
	
	currentClass = 0;
	return 0; 
}

int CSymbolTableBuilder::Visit( const CClassDeclareStar* n )
{
	if ( n->GetClassDeclareStar() != 0 ) n->GetClassDeclareStar()->Accept( this );
	n->GetClassDeclare()->Accept( this );
	return 0; 
}

int CSymbolTableBuilder::Visit( const CClassDeclare* n )
{
	currentClass = symbolTable->AddClass( n->GetId() );
	if( currentClass == 0 ) 
		ErrorMessage( cout, "this classname was used earlier", n->GetLocation() );
	
	if ( n->GetVarDeclareStar() != 0 ) n->GetVarDeclareStar()->Accept( this );
	if ( n->GetMethodDeclareStar() != 0 ) n->GetMethodDeclareStar()->Accept( this );
	currentClass = 0;
	return 0; 
}

int CSymbolTableBuilder::Visit( const CClassDeclareExtends* n )
{
	currentClass = symbolTable->AddClass( n->GetId(), n->GetExtendsId() );
	
	if( currentClass == 0 )
		ErrorMessage( cout, "this classname was used earlier", n->GetLocation() );
	
	if ( n->GetVarDeclareStar() != 0 ) n->GetVarDeclareStar()->Accept( this );
	if ( n->GetMethodDeclareStar() != 0 ) n->GetMethodDeclareStar()->Accept( this );
	currentClass = 0;
	return 0;
}

int CSymbolTableBuilder::Visit( const CVarDeclareStar* n )
{
	if( n->GetVarDeclareStar() != 0 ) n->GetVarDeclareStar()->Accept( this );
	n->GetVarDeclare()->Accept( this );
	return 0; 
}

int CSymbolTableBuilder::Visit( const CVarDeclare* n )
{
	CVarDescription* var;
	if( currentMethod == 0 )
		var = currentClass->AddField( n->GetId(), new CTypeInfo( n->GetType() ) );
	else
		var = currentMethod->AddLocal( new CVarDescription( n->GetId(), new CTypeInfo( n->GetType() ) ) );
	if( var == 0 ) {
		ErrorMessage( cout, "this variable name was used earlier", n->GetLocation() );	
	}
	return 0; 
}

int CSymbolTableBuilder::Visit( const CMethodDeclare* n )
{ 
	assert( currentClass != 0 );
	currentMethod = currentClass->AddMethod( n->GetId(), new CTypeInfo( n->GetType() ) );
	
	if( currentMethod == 0 )
		ErrorMessage( cout, "this method was declared earlier", n->GetLocation() );

	if (n->GetFormalList() != 0) n->GetFormalList()->Accept( this );
	if (n->GetVarDeclareStar() != 0) n->GetVarDeclareStar()->Accept( this );
	currentMethod = 0;
	return 0; 
}

int CSymbolTableBuilder::Visit( const CMethodDeclareStar* n )
{
	if (n->GetMethodDeclareStar() != 0) n->GetMethodDeclareStar()->Accept( this );
	n->GetMethodDeclare()->Accept( this );
	return 0; 
}

int CSymbolTableBuilder::Visit( const CFormalList* n )
{
	assert( currentMethod != 0 );
	CVarDescription* var = currentMethod->AddPapam( new CVarDescription( n->GetId(), new CTypeInfo( n->GetType() ) ) );

	if( var == 0 )
		ErrorMessage( cout, "this variable was used earlier", n->GetLocation() );

	if (n->GetFormalRestStar() != 0) n->GetFormalRestStar()->Accept( this );
	return 0; 
}
int CSymbolTableBuilder::Visit( const CFormalRestStar* n )
{
	assert( currentMethod != 0 );
	CVarDescription* var = currentMethod->AddPapam( new CVarDescription( n->GetId(), new CTypeInfo( n->GetType() ) ) );
	
	if( var == 0 )
		ErrorMessage( cout, "this variable was used earlier", n->GetLocation() );
	
	if (n->GetFormalRestStar() != 0) n->GetFormalRestStar()->Accept( this );
	return 0;
}

int CSymbolTableBuilder::Visit( const CStatement* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CStatementStar* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CStatementIf* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CStatementWhile* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CStatementSysOut* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CStatementAssignment* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CStatementArrayAssignment* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CExpressionBinOp* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CExpressionArray* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CExpressionLength* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CExpressionCallMethod* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CExpressionNumber* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CExpressionBool* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CExpressionVar* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CExpressionThis* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CExpressionNewInt* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CExpressionNewId* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CExpressionNegation* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CExpression* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CExpList* n ) {return 0;}
int CSymbolTableBuilder::Visit( const CExpListNext* n ) {return 0;}