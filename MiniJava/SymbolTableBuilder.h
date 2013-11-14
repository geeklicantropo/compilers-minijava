#pragma once
#include "Visitor.h"
#include "SymbolTable.h"

class CSymbolTableBuilder: public IVisitor {
private:
	CClassDescription* currentClass;
	CMethodDescription* currentMethod;
	CSymbolTable* symbolTable;
public:
	CSymbolTableBuilder( CSymbolTable* );
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
};