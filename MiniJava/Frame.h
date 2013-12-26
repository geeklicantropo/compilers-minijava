#pragma once
#include <vector>
#include "IRTranslationTree.h"
#include "SymbolTable.h"

class IAccess 
{
public:
	virtual ~IAccess();

	virtual const IRTree::IExpression* GetVar() const = 0;
};

class CAccessList
{
public:
	CAccessList ( const IAccess* a, const CAccessList* n ); 
	~CAccessList();
	const IAccess* GetAccess() const;
	const CAccessList* GetNext() const;
private:
	const IAccess* access;
	const CAccessList* next;
};

class CFrame
{
public:
	CFrame( Temp::CLabel* _name, int formalsCount );

	Temp::CLabel* GetName() const;
	CAccessList* GetFormals() const;
	Temp::CTemp* GetThis() const;
	Temp::CTemp* GetFP() const;

	const IAccess* GetFormal( int index );
	const IAccess* AllocLocal( int );

	int GetWordSize() const;

	IRTree::IExpression* ExternalCall( std::string func, IRTree::CExpList* args );
private:
	Temp::CTemp* pointer;
	Temp::CLabel* name;
	
	CAccessList* formals;
	CAccessList* locals;
	
	Temp::CTemp* th;
};


