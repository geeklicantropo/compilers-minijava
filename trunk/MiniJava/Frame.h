#pragma once
#include "Symbols.h"
#include <vector>
#include "SymbolTable.h"

class IAccess 
{
public:
	virtual ~IAccess();
};


class CFrame
{
private:
	vector<const CVarDescription*> formals;

public:
    CFrame (const CSymbol* name, int formalsCount);
    int FormalsCount() const;
	const IAccess* Formal( int index ) const;
};

class CLabel
{
private:
	static int nextUniqueId;
	string name;

public:
	CLabel();
	explicit CLabel( string& s );
	explicit CLabel( const CSymbol* s );
	const string& Name() const;
};

class CTemp 
{
private:
	static int nextUniqueId;
	string name;

public:
	CTemp();
	explicit CTemp( const CSymbol* symbol );
	~CTemp();

	const string& Name() const;
};
