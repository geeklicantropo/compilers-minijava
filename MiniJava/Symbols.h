#pragma once
#include <map>
#include <string>
using namespace std;


class CSymbol {
public:
	string getString() const;
	static const CSymbol* CSymbolGet( string );
private:
	CSymbol( string );
	string s;
	static map<string, CSymbol*> storage;
};