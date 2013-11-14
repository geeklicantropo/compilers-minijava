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
	//if (currentClass == 0 ) 
	
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
	if ( n->GetVarDeclareStar() != 0 ) n->GetVarDeclareStar()->Accept( this );
	if ( n->GetMethodDeclareStar() != 0 ) n->GetMethodDeclareStar()->Accept( this );
	currentClass = 0;
	return 0; 
}

int CSymbolTableBuilder::Visit( const CClassDeclareExtends* n )
{
	currentClass = symbolTable->AddClass( n->GetId(), n->GetExtendsId() );
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
	if( currentMethod == 0 )
		currentClass->AddField( n->GetId(), new CTypeInfo( n->GetType() ) );
	else
		currentMethod->AddLocal( new CVarDescription( n->GetId(), new CTypeInfo( n->GetType() ) ) );
	return 0; 
}

int CSymbolTableBuilder::Visit( const CMethodDeclare* n )
{ 
	currentMethod = currentClass->AddMethod( n->GetId(), new CTypeInfo( n->GetType() ) );
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
	currentMethod->AddPapam( new CVarDescription( n->GetId(), new CTypeInfo( n->GetType() ) ) );
	if (n->GetFormalRestStar() != 0) n->GetFormalRestStar()->Accept( this );
	return 0; 
}
int CSymbolTableBuilder::Visit( const CFormalRestStar* n )
{
	currentMethod->AddPapam( new CVarDescription( n->GetId(), new CTypeInfo( n->GetType() ) ) );
	if (n->GetFormalRestStar() != 0) n->GetFormalRestStar()->Accept( this );
	return 0;
}
