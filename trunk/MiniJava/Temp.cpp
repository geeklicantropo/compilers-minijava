#include "Temp.h"

CLabel::CLabel( const CSymbol* s )
{
	name = s->getString();
}

CLabel::CLabel( string& s ) : name( s ) {}

const string& CLabel::Name() const
{ 
	return name; 
}

const string& CTemp::Name() const 
{ 
	return name; 
}

CTemp::CTemp()
{
}

CTemp::~CTemp()
{
}