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

void CExpList::Accept( IRTreeVisitor* v ) const
{
	return v->Visit( *this );
}

CStmList::CStmList( const IStatement* s, const CStmList* n ) : 
	stm( s ), next( n ) {}

const IStatement* CStmList::GetStm() const 
{
	return stm;
}

const CStmList* CStmList::GetNext() const
{
	return next;
}

void CStmList::SetStm( const IStatement* s ) 
{
	stm = s;
}

void CStmList::SetNext( const CStmList* n ) 
{
	next = n;
}

void CStmList::Accept( IRTreeVisitor* v ) const
{
	return v->Visit( *this );
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

CName::CName(const Temp::CLabel* l)
{
	label = l;
}

const Temp::CLabel* CName::GetLabel() const 
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

CTemp::CTemp( const Temp::CTemp* t ) 
{
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

CBinOp::CBinOp( TBinOp _binop, const IRTree::IExpression* _left, const IRTree::IExpression* _right )
{
	binop = _binop;
	left = _left;
	right = _right;
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

CMove::CMove( const IRTree::IExpression* d, const IRTree::IExpression* s )
{
	dst = d;
	src = s;
}

const IRTree::IExpression* CMove::GetDst() const
{
	return dst;
}

const IRTree::IExpression* CMove::GetSrc() const
{
	return src;
}

void CMove::Accept( IRTreeVisitor* v ) const
{
	return v->Visit( *this );
}

const CExpList* CMove::GetChild() const
{
	if( dynamic_cast<const CMem*>(dst) != 0 ) {
		return new CExpList(dynamic_cast<const CMem*>(dst)->GetExp(), new CExpList(src, 0));
	} else {
		return new CExpList(src, 0);
	}
}

CExp::CExp( const IRTree::IExpression* e )
{
	exp = e;
}

const IRTree::IExpression* CExp::GetExp() const
{
	return exp;
}

void CExp::Accept( IRTreeVisitor* v ) const
{
	return v->Visit( *this );
}

const CExpList* CExp::GetChild() const
{
	return new CExpList(exp, 0);
}

CJump::CJump( const IRTree::IExpression* e, const Temp::CLabelList* t )
{
	exp = e;
	targets = t;
}

CJump::CJump( const Temp::CLabel* t )
{
	exp = new CName(t);
	targets = new Temp::CLabelList(t, 0);
}

const IRTree::IExpression* CJump::GetExp() const
{
	return exp;
}

const Temp::CLabelList* CJump::GetTargets() const
{
	return targets;
}

void CJump::Accept( IRTreeVisitor* v ) const
{
	return v->Visit( *this );
}

const CExpList* CJump::GetChild() const
{
	return new CExpList(exp, 0);
}

CCJump::CCJump( TCJump r, 
			   const IRTree::IExpression* _left, const IRTree::IExpression* _right, 
			   const Temp::CLabel* _iftrue, const Temp::CLabel* _iffalse )
{
	relop = r;
	left = _left;
	right = _right;
	iftrue = _iftrue;
	iffalse = _iffalse;
}

TCJump CCJump::GetRelop() const
{
	return relop;
}

const IRTree::IExpression* CCJump::GetLeft() const
{
	return left;
}

const IRTree::IExpression* CCJump::GetRight() const
{
	return right;
}

const Temp::CLabel* CCJump::GetTrueLabel() const
{
	return iftrue;
}

const Temp::CLabel* CCJump::GetFalseLabel() const
{
	return iffalse;
}

void CCJump::Accept( IRTreeVisitor* v ) const
{
	return v->Visit( *this );
}

const CExpList* CCJump::GetChild() const
{
	return new CExpList(left, new CExpList(right, 0));
}

CSeq::CSeq( const IRTree::IStatement* l, const IRTree::IStatement* r )
{
	left = l;
	right = r;
}

const IRTree::IStatement* CSeq::GetLeft() const
{
	return left;
}

const IRTree::IStatement* CSeq::GetRight() const
{
	return right;
}

void CSeq::Accept( IRTreeVisitor* v ) const
{
	return v->Visit( *this );
}

const CExpList* CSeq::GetChild() const
{
	return 0;
}

CLabel::CLabel( const Temp::CLabel* l )
{
	label = l;
}

const Temp::CLabel* CLabel::GetLabel() const
{
	return label;
}

void CLabel::Accept( IRTreeVisitor* v ) const
{
	return v->Visit( *this );
}

const CExpList* CLabel::GetChild() const
{
	return 0;
}