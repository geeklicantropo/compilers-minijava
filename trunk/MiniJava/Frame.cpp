#include "Frame.h"

IAccess::~IAccess()
{}

int CFrame::FormalsCount() const
{
	return formals.size();
}

const IAccess* CFrame::Formal(int index) const
{
	return 0;
}
