#include "Symbols.h"

using namespace Symbols;

CSymbol::CSymbol( string _s ) :
	s( _s ) {}

string CSymbol::getString()
{
	return s;
}

CSymbolStorage::CSymbolStorage()
{
}

const CSymbol* CSymbolStorage::CSymbolGet( string s )
{
	if( storage.find( s ) != storage.end )
		return storage.find( s )->second;

	CSymbol* tmp = new CSymbol( s );
	storage[s] = tmp;
	return tmp;
}