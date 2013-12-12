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

class CInReg : public IAccess 
{
public:
	CInReg( Temp::CTemp* t );
	const IRTree::IExpression* GetVar() const;
private:
	Temp::CTemp* temp;
};

class CInFrame : public IAccess 
{
public:
	CInFrame( const Temp::CTemp* _fp, int _offset );
	const IRTree::IExpression* GetVar() const;
private:
	const Temp::CTemp* const fp;
	const int offset;
};

class CAccessList
{
public:
	CAccessList ( const IAccess* a, const CAccessList* n ); 
	~CAccessList();
	void SetAccess( const IAccess* a );
	void SetNext( const CAccessList* n );
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
	Temp::CTemp* GetThisValue() const;

	int GetWordSize() const;

	IAccess* AllocLocal();
	IRTree::IExpression* ExternalCall( std::string func, CExpList* args );
private:
	Temp::CTemp* pointer;
	Temp::CLabel* name;
	
	CAccessList* formals;
	CAccessList* locals;
	
	Temp::CTemp* th;
};


