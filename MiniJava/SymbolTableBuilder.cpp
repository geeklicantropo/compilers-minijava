#include "SymbolTableBuilder.h"
#include "miniJava.h"

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
	if ( n->GetVarDeclareStar() != 0 ) n->GetVarDeclareStar()->Accept( this );
	if ( n->GetMethodDeclareStar() != 0 ) n->GetMethodDeclareStar()->Accept( this );
	return 0; 
}

int CSymbolTableBuilder::Visit( const CClassDeclareExtends* n )
{
	currentClass = symbolTable->AddClass( n->GetId(), n->GetExtendsId() );
	if ( n->GetVarDeclareStar() != 0 ) n->GetVarDeclareStar()->Accept( this );
	if ( n->GetMethodDeclareStar() != 0 ) n->GetMethodDeclareStar()->Accept( this );
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
	currentClass->AddField( n->GetId(), new CTypeInfo( n->GetType() ) );
	return 0; 
}

int CSymbolTableBuilder::Visit( const CMethodDeclare* n )
{ 
	currentMethod = currentClass->AddMethod( n->GetId(), new CTypeInfo( n->GetType() ) );
	return 0; 
}

int CSymbolTableBuilder::Visit( const CMethodDeclareStar* n )
{ 
	return 0; 
}

int CSymbolTableBuilder::Visit( const CFormalList* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CFormalRestStar* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CStatement* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CStatementStar* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CStatementIf* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CStatementWhile* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CStatementSysOut* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CStatementAssignment* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CStatementArrayAssignment* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CExpressionBinOp* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CExpressionArray* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CExpressionLength* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CExpressionCallMethod* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CExpressionNumber* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CExpressionBool* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CExpressionVar* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CExpressionThis* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CExpressionNewInt* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CExpressionNewId* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CExpressionNegation* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CExpression* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CExpList* n ){ return 0; }
int CSymbolTableBuilder::Visit( const CExpListNext* n ){ return 0; }
