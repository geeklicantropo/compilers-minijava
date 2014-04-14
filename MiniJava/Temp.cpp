#include "Temp.h"

using namespace Temp;

int CLabel::nextUniqueId = 0;

CLabel::CLabel()
{
	name = "L";
	string id = std::to_string(++nextUniqueId);
	name += id;
}

CLabel::CLabel( const CSymbol* s )
{
	name = s->getString();
}

CLabel::CLabel( string& s ) : name( s ) {}

const string CLabel::Name() const
{ 
	return name; 
}

int CTemp::nextUniqueId = 0;

CTemp::CTemp()
{
	name = "T";
	string id = std::to_string(++nextUniqueId);
	name += id;
}

CTemp::CTemp( const CSymbol* symbol )
{
	name = symbol->getString();
}

const string& CTemp::Name() const 
{ 
	return name; 
}




