#include "Frame.h"

IAccess::~IAccess()
{

}

class CInReg : public IAccess 
{
public:
	CInReg( Temp::CTemp* t );
	const IRTree::IExpression* GetVar() const;
private:
	Temp::CTemp* temp;
};

class CInFrame : public IAccess 
{
public:
	CInFrame( const Temp::CTemp* _fp, int _offset );
	const IRTree::IExpression* GetVar() const;
private:
	const Temp::CTemp* const fp;
	const int offset;
};


CInReg::CInReg( Temp::CTemp* t )
{
	temp = t;
}

const IRTree::IExpression* CInReg::GetVar() const 
{
	return new IRTree::CTemp( temp );
}

CInFrame::CInFrame( const Temp::CTemp* _fp, int _offset ) : fp(_fp), offset(_offset)
{
	
}

const IRTree::IExpression* CInFrame::GetVar() const
{
	return new IRTree::CMem( new IRTree::CBinOp( IRTree::TBinOp::PLUS, new IRTree::CTemp( fp ), new IRTree::CConst( offset ) ) );
}

CAccessList::CAccessList( const IAccess* a, const CAccessList* n )
{
	access = a; 
	next = n;
}
	
void CAccessList::SetAccess( const IAccess* a ) 
{
	next = new CAccessList( access, next );
	access = a;
}
	
void CAccessList::SetNext( const CAccessList* n ) 
{
	next = n;
}
	
const IAccess* CAccessList::GetAccess() const
{
	return access;
}
	
const CAccessList* CAccessList::GetNext() const
{
	return next;
}

CFrame::CFrame( Temp::CLabel* _name, int formalsCount )
{
	name = _name;

}
	
Temp::CLabel* CFrame::GetName() const 
{
	return name;
}
	
CAccessList* CFrame::GetFormals() const
{
	return formals;
}
	
Temp::CTemp* CFrame::GetThis() const 
{
	return th;
}

Temp::CTemp* CFrame::GetFP() const
{
	return pointer;
}
	
int CFrame::GetWordSize() const
{
	return sizeof(int);
}

IAccess* CFrame::AllocLocal()
{
	IAccess* temp = new CInReg(new Temp::CTemp());
	CAccessList* tempList = new CAccessList(temp, locals);
	locals = tempList;
	return temp;
}

