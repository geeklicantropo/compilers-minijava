#include "IRTranslationTree.h"

using namespace IRTree;

CExpList::CExpList( const IExpression* e, const CExpList* n ) :
	exp(e), next(n) {}

const IExpression* CExpList::GetExp() const 
{
	return  exp;
}

const CExpList* CExpList::GetNext() const 
{
	return next;
}

void CExpList::SetExp( const IExpression* e )
{
	exp = e;
} 

void CExpList::SetNext( const CExpList* n )
{
	next = n;
} 

CStmList::CStmList( const IStatement* e, const CStmList* n ) : 
	stm( e ), next( n ) {}

const IStatement* CStmList::GetStm() const 
{
	return stm;
}

void CStmList::SetStm( const IStatement* e ) 
{
	stm = e;
}

void CStmList::SetNext( const CStmList* n ) 
{
	next = n;
}

CConst::CConst( int v )
{
	val = v;
}

int CConst::GetValue() const 
{
	return val;
}

void CConst::Accept( IRTreeVisitor* v ) const
{
	v->Visit( *this );
}

const CExpList* CConst::GetChild() const
{
	return 0;
}