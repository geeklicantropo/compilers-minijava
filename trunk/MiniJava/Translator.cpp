#include "Translator.h"
#include "Frame.h"

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
	switch( op )
	{
	case LESS:
		return new IRTree::CCJump( IRTree::TCJump::LT, expr1, expr2, t, f );
	default:
		assert(false);
		break;
	}
    return 0;
}

CTranslator::CTranslator( CSymbolTable* st ) :
	symbolTable( st )
{
	currentClass = 0;
	currentMethod = 0;
	currentFrame = 0;
	lastCodeFragment = 0;
	lastValue = 0;
}

const CSymbol* CTranslator::makeLabelName( CClassDescription* classDescription, CMethodDescription* method ) const
{
	return CSymbol::CSymbolGet( classDescription->GetName()->getString() + "@" + method->GetName()->getString() );
}

int CTranslator::Visit( const CProgram* n ) 
{
	n->GetMainClass()->Accept( this );
	if ( n->GetClassDeclareStar() != 0 ) n->GetClassDeclareStar()->Accept( this );
	return 0;
}

int CTranslator::Visit( const CMainClass* n )
{
	currentClass = symbolTable->LookUpClass( n->GetId() );
	currentMethod = currentClass->LookUpMethod( CSymbol::CSymbolGet( "main" ) );
	currentFrame = new CFrame( new Temp::CLabel( makeLabelName( currentClass, currentMethod ) ), 0 );
	n->GetStatement()->Accept( this );
	lastCodeFragment = new CCodeFragment( currentFrame, lastValue->ToExp(), lastCodeFragment );
	currentClass = 0;
	return 0;
}

int CTranslator::Visit( const CClassDeclareStar* n )
{
	if ( n->GetClassDeclareStar() != 0 ) n->GetClassDeclareStar()->Accept( this );
	n->GetClassDeclare()->Accept( this );
	return 0;
}

int CTranslator::Visit( const CClassDeclare* n )
{
	currentClass = symbolTable->LookUpClass( n->GetId() );
	if ( n->GetMethodDeclareStar() != 0 ) n->GetMethodDeclareStar()->Accept( this );
	currentClass = 0;
	return 0;
}

int CTranslator::Visit( const CClassDeclareExtends* n )
{
	currentClass = symbolTable->LookUpClass( n->GetId() );
	if( n->GetMethodDeclareStar() != 0) n->GetMethodDeclareStar()->Accept( this );
	currentClass = 0;
	return 0;
}

int CTranslator::Visit( const CVarDeclareStar* n )
{
	if( n->GetVarDeclareStar() != 0 ) n->GetVarDeclareStar()->Accept( this );
	n->GetVarDeclare()->Accept( this );
	return 0;
}

int CTranslator::Visit( const CVarDeclare* n )
{ 
	return 0;
}

int CTranslator::Visit( const CFormalList* n )
{
	return 0;
}

int CTranslator::Visit( const CFormalRestStar* n )
{
	return 0;
}

int CTranslator::Visit( const CMethodDeclare* n )
{
	assert( currentClass != 0 );
	currentMethod = currentClass->LookUpMethod( n->GetId() );
	currentFrame = new CFrame( new Temp::CLabel( makeLabelName( currentClass, currentMethod ) ), 
		currentMethod->GetParamsNumber() );
	if( n->GetStatementStar() != 0 ) n->GetStatementStar()->Accept( this );
	const IRTree::IStatement* stm = lastValue->ToStm();
	n->GetExpression()->Accept( this );
	const IRTree::IExpression* exp = lastValue->ToExp();
	lastCodeFragment = new CCodeFragment( currentFrame,  new IRTree::CEseq( stm, exp ) , lastCodeFragment );
	currentMethod = 0;
	currentFrame = 0;
	return 0;
}

int CTranslator::Visit( const CMethodDeclareStar* n )
{
	if( n->GetMethodDeclareStar() != 0 ) n->GetMethodDeclareStar()->Accept( this );
	n->GetMethodDeclare()->Accept( this );
	return 0;
}

int CTranslator::Visit( const CStatement* n )
{
	if( n->GetStatementStar() != 0 ) n->GetStatementStar()->Accept( this ); 
	return 0;
}

int CTranslator::Visit( const CStatementStar* n )
{
	const IRTree::IStatement* stm = 0;
	const IRTree::IStatement* leftStm = 0;
	const IRTree::IStatement* rightStm = 0;	
	if( n->GetStatement() != 0 ) {
		n->GetStatement()->Accept( this );
		assert( lastValue != 0 );
		leftStm = lastValue->ToStm();
	}
	if( n->GetStatementStar() != 0 ) {
		n->GetStatementStar()->Accept( this );
		assert( lastValue != 0 );
		rightStm = lastValue->ToStm();
		if( leftStm == 0 ) {
			stm = rightStm;
		} else {
			stm = new IRTree::CSeq( leftStm, rightStm );
		}
	}
	lastValue = new CStmConverter( stm );
	return 0;
}

int CTranslator::Visit( const CStatementIf* n )
{
	//
	return 0;
}

int CTranslator::Visit( const CStatementWhile* n )
{
	//
	return 0;
}

int CTranslator::Visit( const CStatementSysOut* n )
{
	n->GetExpression()->Accept( this );
	assert( lastValue != 0 );
	Temp::CLabel* sysoutLabel = new Temp::CLabel( CSymbol::CSymbolGet( "System.out.println" ) );
	IRTree::IStatement* callSysout = new IRTree::CExp( new IRTree::CCall( new IRTree::CName( sysoutLabel ), new IRTree::CExpList( lastValue->ToExp(), 0 ) ) );
	lastValue = new CStmConverter( callSysout );
	return 0;
}

int CTranslator::Visit( const CStatementAssignment* n )
{
	return 0;
}

int CTranslator::Visit( const CStatementArrayAssignment* n )
{
	return 0;
}

int CTranslator::Visit( const CExpressionBinOp* n )
{
	n->GetExpressionFirst()->Accept( this );
	assert( lastValue != 0 );
	const IRTree::IExpression* left = lastValue->ToExp();
	n->GetExpressionSecond()->Accept( this );
	assert( lastValue != 0 );
	const IRTree::IExpression* right = lastValue->ToExp();
	TBinaryOperation op = n->GetOperation();
	switch( op )
	{
	case AND:
		//еще нет конвертера
		break;
	case LESS:
		lastValue = new CRelativeCmpConverter( LESS, left, right );
		break;
	case PLUS:
		lastValue = new CExpConverter( new IRTree::CBinOp( IRTree::TBinOp::PLUS, left, right ) );
		break;
	case MINUS:
		lastValue = new CExpConverter( new IRTree::CBinOp( IRTree::TBinOp::MINUS, left, right ) );
		break;
	case TIMES:
		lastValue = new CExpConverter( new IRTree::CBinOp( IRTree::TBinOp::MUL, left, right ) );
		break;
	default:
		break;
	}
	return 0;
}

int CTranslator::Visit( const CExpressionArray* n )
{
	//
	return 0;
}

int CTranslator::Visit( const CExpressionLength* n )
{
	//
	return 0;
}

int CTranslator::Visit( const CExpressionCallMethod* n )
{
	//
	return 0;
}

int CTranslator::Visit( const CExpressionNumber* n )
{
	lastValue = new CExpConverter( new IRTree::CConst( n->GetNumber() ) );
	return 0;
}

int CTranslator::Visit( const CExpressionBool* n )
{
	lastValue = new CExpConverter( new IRTree::CConst( n->GetValue() ) );
	return 0;
}

int CTranslator::Visit( const CExpressionVar* n )
{
	//
	return 0;
}

int CTranslator::Visit( const CExpressionThis* n )
{
	lastValue = new CExpConverter( new IRTree::CTemp( currentFrame->GetThis() ) );
	return 0;
}

int CTranslator::Visit( const CExpressionNewInt* n )
{
	return 0;
}

int CTranslator::Visit( const CExpressionNewId* n )
{
	return 0;
}

int CTranslator::Visit( const CExpressionNegation* n )
{
	return 0;
}

int CTranslator::Visit( const CExpression* n )
{
	if( n->GetExpression() != 0 ) n->GetExpression()->Accept( this );
	return 0;
}

int CTranslator::Visit( const CExpList* n )
{
	if( n->GetExpression() != 0 ) n->GetExpression()->Accept( this );
	return 0;
}

int CTranslator::Visit( const CExpListNext* n )
{	
	if( n->GetExpression() != 0 ) n->GetExpression()->Accept( this ); 
	if( n->GetExpList() != 0 ) n->GetExpList()->Accept( this );
	return 0;
}
