#include "Temp.h"

using namespace Temp;

int CLabel::nextUniqueId = 0;

CLabel::CLabel()
{
	name = "L" + (++nextUniqueId);
}

CLabel::CLabel( const CSymbol* s )
{
	name = s->getString();
}

CLabel::CLabel( string& s ) : name( s ) {}

const string& CLabel::Name() const
{ 
	return name; 
}

int CTemp::nextUniqueId = 0;

CTemp::CTemp()
{
	name = "T" + (++nextUniqueId);
}

const string& CTemp::Name() const 
{ 
	return name; 
}




