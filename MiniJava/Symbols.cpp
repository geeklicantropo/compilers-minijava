#include "Symbols.h"

map<string, CSymbol*, CSymbol::comparer> CSymbol::storage;

CSymbol::CSymbol( string _s ) :
	s( _s ) {}

string CSymbol::getString() const
{
	return s;
}


bool CSymbol::comparer::operator() (const string& first, const string& second)
{
	bool t = (first.compare( second ) == 0);
	return first.compare( second ) == 0;
}

const CSymbol* CSymbol::CSymbolGet( string s )
{
	map<string, CSymbol*, CSymbol::comparer>::iterator it = storage.find( s );
	if( storage.find( s ) != storage.end() )
		return storage.find( s )->second;
	CSymbol* tmp = new CSymbol( s );
	storage[s] = tmp;
	return tmp;
}