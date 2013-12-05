#pragma once
#include <vector>
#include "SymbolTable.h"
#include "Temp.h"

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


