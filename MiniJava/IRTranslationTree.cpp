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

CName::CName(const CLabel* _label)
{
	label = _label;
}

const CLabel* CName::GetLabel() const 
{
	return label;
}

void CName::Accept( IRTreeVisitor* v ) const 
{
	v->Visit( *this );
}

const CExpList* CName::GetChild() const 
{
	return 0;
}

CTemp::CTemp( const Temp::CTemp* t ) {
	temp = t;
}

const Temp::CTemp* CTemp::GetTemp() const
{
	return temp;
}

void CTemp::Accept( IRTreeVisitor* v ) const
{
	v->Visit( *this );
}

const CExpList* CTemp::GetChild() const
{
	return 0;
}

CBinOp::CBinOp( const IRTree::IExpression* _left, const IRTree::IExpression* _right, TBinOp _binop )
{
	left = _left;
	right = _right;
	binop = _binop;
}

TBinOp CBinOp::GetBinOp( ) const
{
	return binop;
}

const IRTree::IExpression* CBinOp::GetLeft() const
{
	return left;
}

const IRTree::IExpression* CBinOp::GetRight() const
{
	return right;
}

void CBinOp::Accept( IRTreeVisitor* v ) const
{
	return v->Visit( *this );
}

const CExpList* CBinOp::GetChild() const
{
	return new CExpList( left, new CExpList( right, 0 ) );
}

CMem::CMem( const IRTree::IExpression* e ) 
{
	exp = e;
}

const IRTree::IExpression* CMem::GetExp() const
{
	return exp;
}

void CMem::Accept( IRTreeVisitor* v ) const
{
	return v->Visit( *this );
}

const CExpList* CMem::GetChild() const
{
	return new CExpList( exp, 0 );
}

CCall::CCall( const IRTree::IExpression* f, const CExpList* a )
{
	func = f;
	args = a;
}

const IRTree::IExpression* CCall::GetExp() const
{
	return func;
}

const CExpList* CCall::GetArgs() const
{
	return args;
}

void CCall::Accept( IRTreeVisitor* v ) const
{
	return v->Visit( *this );
}

const CExpList* CCall::GetChild() const
{
	return new CExpList( func, args );
}

CEseq::CEseq( const IRTree::IStatement* s, const IRTree::IExpression* e )
{
	stm = s;
	exp = e;
}
const IRTree::IStatement* CEseq::GetStm() const
{
	return stm;
}

const IRTree::IExpression* CEseq::GetExp() const
{
	return exp;
}

void CEseq::Accept( IRTreeVisitor* v ) const
{
	return v->Visit( *this );
}

const CExpList* CEseq::GetChild() const
{
	return 0;
}