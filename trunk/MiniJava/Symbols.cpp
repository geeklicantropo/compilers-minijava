#include "Symbols.h"


map<string, CSymbol*> CSymbol::storage;

CSymbol::CSymbol( string _s ) :
	s( _s ) {}

string CSymbol::getString() const
{
	return s;
}

const CSymbol* CSymbol::CSymbolGet( string s )
{
	if( storage.find( s ) != storage.end() )
		return storage.find( s )->second;
	CSymbol* tmp = new CSymbol( s );
	storage[s] = tmp;
	return tmp;
}