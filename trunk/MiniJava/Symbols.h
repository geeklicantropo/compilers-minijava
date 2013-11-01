#pragma once
#include <map>
#include <string.h>
using namespace std;


class CSymbol {
public:
	string getString() const;
	static const CSymbol* CSymbolGet( string );
private:
	CSymbol( string );
	string s;
	static struct comparer {
	public:
		bool operator() ( const string&, const string& );
	};
	static map<string, CSymbol*, comparer> storage;
};