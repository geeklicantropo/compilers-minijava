#include "Symbols.h"

map<string, CSymbol*, CSymbol::comparer> CSymbol::storage;

CSymbol::CSymbol( string _s ) :
	s( _s ) {}

string CSymbol::getString()
{
	return s;
}


bool CSymbol::comparer::operator() (const string& first, const string& second)
{
	return first.compare( second ) == 0;
}

const CSymbol* CSymbol::CSymbolGet( string s )
{
	if( storage.find( s ) != storage.end() )
		return storage.find( s )->second;
	CSymbol* tmp = new CSymbol( s );
	storage[s] = tmp;
	return tmp;
}