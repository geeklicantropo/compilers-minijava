#include "CodeFragment.h"

CCodeFragment::CCodeFragment( const CFrame* f, const IRTree::IStatement* t, const CCodeFragment* n ) :
	frame( f ), irtree( t ), next( n )
{
	assert( frame != 0 );
	assert( irtree != 0 );
}

void CCodeFragment::SetNext( const CCodeFragment* n )
{
	next = n;
}

const CFrame* CCodeFragment::GetFrame() const
{
	return frame;
}

const IRTree::IStatement* CCodeFragment::GetIRTree() const
{
	return irtree;
}

const CCodeFragment* CCodeFragment::GetNext() const
{
	return next;
}