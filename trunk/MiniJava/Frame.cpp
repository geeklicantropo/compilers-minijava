#include "Frame.h"

IAccess::~IAccess()
{}

int CFrame::FormalsCount() const
{
	return formals.size();
}

const IAccess* CFrame::Formal(int index) const
{

}

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