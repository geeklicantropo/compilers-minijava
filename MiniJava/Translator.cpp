#include "Translator.h"

using namespace Translator;


class CExpConverter : public ISubtreeWrapper
{
public:
	CExpConverter( const IRTree::IExpression* e ) : expr(e) {}

	const IRTree::IExpression* ToExp() const;
	const IRTree::IStatement* ToStm() const; 
	const IRTree::IStatement* ToConditional( const Temp::CLabel* t, const Temp::CLabel* f ) const;

private:
	const IRTree::IExpression* expr;
};

const IRTree::IExpression* CExpConverter::ToExp() const
{
	return expr;
}

const IRTree::IStatement* CExpConverter::ToStm() const
{
	return new IRTree::CExp(expr);
}

const IRTree::IStatement* CExpConverter::ToConditional( const Temp::CLabel* t, const Temp::CLabel* f ) const
{
	assert(false);
	return 0;
}

class CStmConverter : public ISubtreeWrapper
{
public:
	CStmConverter( const IRTree::IStatement* s ) : stm(s) {}

	const IRTree::IExpression* ToExp() const;
	const IRTree::IStatement* ToStm() const; 
	const IRTree::IStatement* ToConditional( const Temp::CLabel* t, const Temp::CLabel* f ) const;

private:
	const IRTree::IStatement* stm;
};

const IRTree::IExpression* CStmConverter::ToExp() const
{
	assert(false);
	return 0;
}

const IRTree::IStatement* CStmConverter::ToStm() const
{
	return stm;
}

const IRTree::IStatement* CStmConverter::ToConditional( const Temp::CLabel* t, const Temp::CLabel* f ) const
{
	assert(false);
	return 0;
}

class CConditionalConverter : public ISubtreeWrapper
{
public:
	CConditionalConverter() {}
	const IRTree::IExpression* ToExp() const;
	const IRTree::IStatement* ToStm() const; 
	virtual const IRTree::IStatement* ToConditional( const Temp::CLabel* t, const Temp::CLabel* f ) const = 0;
};

const IRTree::IExpression* CConditionalConverter::ToExp() const
{
	Temp::CTemp* r = new Temp::CTemp();
	Temp::CLabel* t = new Temp::CLabel();
	Temp::CLabel* f = new Temp::CLabel();

	return new IRTree::CEseq( new IRTree::CSeq( new IRTree::CMove( new IRTree::CTemp( r ), new IRTree::CConst( 1 ) ),
		new IRTree::CSeq( ToConditional( t, f ), new IRTree::CSeq( new IRTree::CLabel( f ),
		new IRTree::CSeq( new IRTree::CMove( new IRTree::CTemp( r ), new IRTree::CConst( 0 ) ),
		new IRTree::CLabel( t ) ) ) ) ), new IRTree::CTemp( r ) );
}

const IRTree::IStatement* CConditionalConverter::ToStm() const
{
	assert(false);
	return 0;
}

class CRelativeCmpConverter : public CConditionalConverter
{
public:
	CRelativeCmpConverter( TBinaryOperation _op, const IRTree::IExpression* e1, const IRTree::IExpression* e2 );
	const IRTree::IStatement* ToConditional( const Temp::CLabel* t, const Temp::CLabel* f ) const;
private:
	TBinaryOperation op;
	const IRTree::IExpression* expr1;
	const IRTree::IExpression* expr2;
};

CRelativeCmpConverter::CRelativeCmpConverter(TBinaryOperation _op, const IRTree::IExpression* e1, const IRTree::IExpression* e2 )
{
	op = _op;
	expr1 = e1;
	expr2 = e2;
}

const IRTree::IStatement* CRelativeCmpConverter::ToConditional( const Temp::CLabel* t, const Temp::CLabel* f ) const
{
	switch( op ) {
	case LESS:
		return new IRTree::CCJump( IRTree::TCJump::LT, expr1, expr2, t, f );
	default:
		assert(false);
		break;
	}
	return 0;
}