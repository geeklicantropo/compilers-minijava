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
	pointer = new Temp::CTemp( CSymbol::CSymbolGet( "FP" ) );
	th = new Temp::CTemp( CSymbol::CSymbolGet( "this" ) );
	formals = 0;

	IAccess* newAccess = new CInFrame( pointer, GetWordSize() );
	formals = new CAccessList( newAccess, formals );
	for( int i = 0; i < formalsCount; i++ ) {
		IAccess* newAccess = new CInFrame( pointer, (i + 1) * GetWordSize() );
		CAccessList* newFormals = new CAccessList( newAccess, formals );
		formals = newFormals;
	}		
	locals = 0;
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

const IAccess* CFrame::GetFormal( int index )
{
	const CAccessList* currentList = formals;
	for ( int i = 0; i < index; ++i )
	{
		assert( currentList != 0 );
		currentList = currentList->GetNext();
	}
	return currentList->GetAccess();
}

const IAccess* CFrame::AllocLocal( int index)
{
	IAccess* temp = new CInFrame( pointer, index * GetWordSize() );
	CAccessList* tempList = new CAccessList(temp, locals);
	locals = tempList;
	return temp;
}
	
int CFrame::GetWordSize() const
{
	return sizeof(int);
}

IRTree::IExpression* CFrame::ExternalCall( std::string func, IRTree::CExpList* args )
{
	return new IRTree::CCall( new IRTree::CName( new Temp::CLabel(func)), args );
}

