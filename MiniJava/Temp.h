#pragma once
#include "Symbols.h"
#include <string>

using namespace std;

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

	const string& Name() const;
};