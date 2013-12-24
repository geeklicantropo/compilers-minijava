#include "Translator.h"
#include "Frame.h"
#include "miniJava.h"
#include "TypeChecker.h"

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
	return new IRTree::CExp( expr );
}

const IRTree::IStatement* CExpConverter::ToConditional( const Temp::CLabel* t, const Temp::CLabel* f ) const
{
	return new IRTree::CCJump( IRTree::TCJump::NE, expr, new IRTree::CConst( 0 ), t, f );
}

class CStmConverter : public ISubtreeWrapper
{
public:
	CStmConverter( const IRTree::IStatement* s ) : stm( s ) {}

	const IRTree::IExpression* ToExp() const;
	const IRTree::IStatement* ToStm() const;
	const IRTree::IStatement* ToConditional( const Temp::CLabel* t, const Temp::CLabel* f ) const;

private:
	const IRTree::IStatement* stm;
};

const IRTree::IExpression* CStmConverter::ToExp() const
{
	assert( false );
	return 0;
}

const IRTree::IStatement* CStmConverter::ToStm() const
{
	return stm;
}

const IRTree::IStatement* CStmConverter::ToConditional( const Temp::CLabel* t, const Temp::CLabel* f ) const
{
	assert( false );
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
	assert( false );
	return 0;
}

class CAndConverter : public CConditionalConverter
{
public:
	CAndConverter( const IRTree::IExpression* e1, const IRTree::IExpression* e2 );
	const IRTree::IStatement* ToConditional( const Temp::CLabel* t, const Temp::CLabel* f ) const;
private:
	const IRTree::IExpression* expr1;
	const IRTree::IExpression* expr2;
};

CAndConverter::CAndConverter( const IRTree::IExpression* e1, const IRTree::IExpression* e2 )
{
	expr1 = e1;
	expr2 = e2;
}

const IRTree::IStatement* CAndConverter::ToConditional( const Temp::CLabel* t, const Temp::CLabel* f ) const
{
	Temp::CLabel* l = new Temp::CLabel();
	return new IRTree::CSeq( new IRTree::CCJump( IRTree::TCJump::NE, expr1, new IRTree::CConst( 0 ), l, f ),
		new IRTree::CSeq( new IRTree::CLabel( l ), 
		new IRTree::CCJump( IRTree::TCJump::NE, expr2, new IRTree::CConst( 0 ), t, f ) ) );
}

class CRelativeCmpConverter : public CConditionalConverter
{
public:
	CRelativeCmpConverter( IRTree::TCJump _op, const IRTree::IExpression* e1, const IRTree::IExpression* e2 );
	const IRTree::IStatement* ToConditional( const Temp::CLabel* t, const Temp::CLabel* f ) const;
private:
	IRTree::TCJump op;
	const IRTree::IExpression* expr1;
	const IRTree::IExpression* expr2;
};

CRelativeCmpConverter::CRelativeCmpConverter( IRTree::TCJump _op, const IRTree::IExpression* e1, const IRTree::IExpression* e2 )
{
	op = _op;
	expr1 = e1;
	expr2 = e2;
}

const IRTree::IStatement* CRelativeCmpConverter::ToConditional( const Temp::CLabel* t, const Temp::CLabel* f ) const
{
	return new IRTree::CCJump( op, expr1, expr2, t, f );
}

CTranslator::CTranslator( CSymbolTable* st, const CCodeFragment** cf ) :
	symbolTable( st ), lastCodeFragment( cf )
{
	currentClass = 0;
	currentMethod = 0;
	currentFrame = 0;
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
	*lastCodeFragment = new CCodeFragment( currentFrame, new IRTree::CEseq( lastValue->ToStm(), new IRTree::CConst(0) ), *lastCodeFragment );
	currentClass = 0;
	return 0;
}

int CTranslator::Visit( const CClassDeclareStar* n )
{
	if( n->GetClassDeclareStar() != 0 ) n->GetClassDeclareStar()->Accept( this );
	n->GetClassDeclare()->Accept( this );
	return 0;
}

int CTranslator::Visit( const CClassDeclare* n )
{
	currentClass = symbolTable->LookUpClass( n->GetId() );
	if( n->GetMethodDeclareStar() != 0 ) n->GetMethodDeclareStar()->Accept( this );
	currentClass = 0;
	return 0;
}

int CTranslator::Visit( const CClassDeclareExtends* n )
{
	currentClass = symbolTable->LookUpClass( n->GetId() );
	if( n->GetMethodDeclareStar() != 0 ) n->GetMethodDeclareStar()->Accept( this );
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
	assert( currentClass != 0 );
	currentMethodLocalVariables[n->GetId()] = currentMethodLocalVariables.size();
	lastValue = new CStmConverter( new IRTree::CSeq( lastValue->ToStm(), new IRTree::CMove( currentFrame->AllocLocal()
		->GetVar(), new IRTree::CConst( 0 ) ) ) );
	return 0;
}

int CTranslator::Visit( const CFormalList* n )
{
	currentMethodArguments[n->GetId()] = currentMethodArguments.size() + 1;
	lastValue = new CStmConverter( new IRTree::CSeq( lastValue->ToStm(),
		new IRTree::CMove( currentFrame->GetFormal( currentMethodArguments[n->GetId()] - 1 )
		->GetVar(), new IRTree::CConst( 0 ) ) ) );

	if ( n->GetFormalRestStar() != 0 )
	{
		n->GetFormalRestStar()->Accept( this );
	}
	return 0;
}

int CTranslator::Visit( const CFormalRestStar* n )
{
	currentMethodArguments[n->GetId()] = currentMethodArguments.size() + 1;
	lastValue = new CStmConverter( new IRTree::CSeq( lastValue->ToStm(),
		new IRTree::CMove( currentFrame->GetFormal( currentMethodArguments[n->GetId()] - 1 )
		->GetVar(), new IRTree::CConst( 0 ) ) ) );

	if ( n->GetFormalRestStar() != 0 )
	{
		n->GetFormalRestStar()->Accept( this );
	}
	return 0;
}

int CTranslator::Visit( const CMethodDeclare* n )
{
	assert( currentClass != 0 );
	currentMethod = currentClass->LookUpMethod( n->GetId() );
	currentMethodLocalVariables.clear();
	currentMethodArguments.clear();

	currentFrame = new CFrame( new Temp::CLabel( makeLabelName( currentClass, currentMethod ) ), 
		currentMethod->GetParamsNumber() );
	
	if( n->GetFormalList() != 0 ) n->GetFormalList()->Accept( this );
	if( n->GetVarDeclareStar() != 0 ) n->GetVarDeclareStar()->Accept( this );

	if( n->GetStatementStar() != 0 ) n->GetStatementStar()->Accept( this );
	const IRTree::IStatement* stm = lastValue->ToStm();
	n->GetExpression()->Accept( this );
	const IRTree::IExpression* exp = lastValue->ToExp();
	*lastCodeFragment = new CCodeFragment( currentFrame,  new IRTree::CEseq( stm, exp ) , *lastCodeFragment );
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
	}
	if( leftStm == 0 && rightStm == 0) {
		stm = 0;
	} else if( leftStm == 0 ) {
		stm = rightStm;	
	} else if( rightStm == 0 ) {
		stm = leftStm;
	} else {
		stm = new IRTree::CSeq( leftStm, rightStm );
	}
	assert( stm != 0 );
	lastValue = new CStmConverter( stm );
	return 0;
}

int CTranslator::Visit( const CStatementIf* n )
{
	n->GetExpression()->Accept( this );
	assert( lastValue != 0 );
	Translator::ISubtreeWrapper* ifExpr = lastValue;
	n->GetStatementIf()->Accept( this );
	assert( lastValue != 0 );
	const IRTree::IStatement* ifStm = lastValue->ToStm();
	n->GetStatementElse()->Accept( this );
	assert( lastValue != 0 );
	const IRTree::IStatement* elseStm = lastValue->ToStm();
	Temp::CLabel* t = new Temp::CLabel();
	Temp::CLabel* f = new Temp::CLabel();
	Temp::CLabel* r = new Temp::CLabel();
	IRTree::IStatement* tSeq = new IRTree::CSeq( new IRTree::CLabel( t ), new IRTree::CSeq( ifStm, new IRTree::CJump( r ) ) );
	IRTree::IStatement* fSeq = new IRTree::CSeq( new IRTree::CLabel( f ), new IRTree::CSeq( elseStm, new IRTree::CJump( r ) ) );
	IRTree::IStatement* ifSubtree = new IRTree::CSeq( ifExpr->ToConditional( t, f ), new IRTree::CSeq( new IRTree::CSeq( tSeq, fSeq ), new IRTree::CLabel( r ) ) );
	lastValue = new CStmConverter( ifSubtree );
	return 0;
}

int CTranslator::Visit( const CStatementWhile* n )
{
	n->GetExpression()->Accept( this );
	assert( lastValue != 0 );
	Translator::ISubtreeWrapper* whileExpr = lastValue;
	n->GetStatement()->Accept( this );
	assert( lastValue != 0 );
	const IRTree::IStatement* whileStm = lastValue->ToStm();
	Temp::CLabel* t = new Temp::CLabel();
	Temp::CLabel* f = new Temp::CLabel();
	Temp::CLabel* r = new Temp::CLabel();
	IRTree::IStatement* tSeq = new IRTree::CSeq( new IRTree::CLabel( t ), whileStm );
	IRTree::IStatement* whileSubtree = new IRTree::CSeq( new IRTree::CSeq( new IRTree::CLabel( t ), whileExpr->ToConditional( f, r ) ), 
													new IRTree::CSeq( tSeq , new IRTree::CSeq( new IRTree::CJump( t ), new IRTree::CLabel( r ) ) ) );

	lastValue = new CStmConverter( whileSubtree );
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
	auto variableNode = new CExpressionVar( n->GetId(), n->GetLocation() );
	variableNode->Accept( this );
	delete( variableNode );
	const IRTree::IExpression* variable = lastValue->ToExp();
	n->GetExpression()->Accept( this );
	assert( lastValue != 0 );
	lastValue = new CStmConverter( new IRTree::CMove( variable, lastValue->ToExp() ) );
	return 0;
}

int CTranslator::Visit( const CStatementArrayAssignment* n )
{
	n->GetExpressionArray()->Accept( this );
	assert( lastValue != 0 );
	const IRTree::IExpression* index = lastValue->ToExp();
	n->GetExpression()->Accept( this );
	assert( lastValue != 0 );
	const IRTree::IExpression* value = lastValue->ToExp();

	const IRTree::IExpression* offset = new IRTree::CBinOp( IRTree::MUL, index + 1, new IRTree::CConst( currentFrame->GetWordSize() ) );
	const IRTree::IExpression* baseAddress = new IRTree::CName( new Temp::CLabel( n->GetId() ) );
	const IRTree::IExpression* assignmentAddress = new IRTree::CMem( new IRTree::CBinOp( IRTree::PLUS, baseAddress, offset ) );

	lastValue = new CStmConverter( new IRTree::CMove( assignmentAddress, value ) );
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
		lastValue = new CAndConverter( left, right );
		break;
	case LESS:
		lastValue = new CRelativeCmpConverter( IRTree::TCJump::LT, left, right );
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
	n->GetExpression1()->Accept( this );
	assert( lastValue != 0 );
	const IRTree::IExpression* index = lastValue->ToExp();
	const IRTree::IExpression* offset = new IRTree::CBinOp( IRTree::MUL, index, new IRTree::CConst( currentFrame->GetWordSize() ) );
	n->GetExpression2()->Accept( this );
	assert( lastValue != 0 );
	lastValue = new CExpConverter( new IRTree::CMem( new IRTree::CBinOp( IRTree::PLUS, lastValue->ToExp(), offset ) ) );
	return 0;
}

int CTranslator::Visit( const CExpressionLength* n )
{
	n->GetExpression()->Accept( this );
	assert( lastValue != 0 );
	lastValue = new CExpConverter( new IRTree::CMem( new IRTree::CBinOp( IRTree::MINUS, lastValue->ToExp(), new IRTree::CConst( currentFrame->GetWordSize() ) ) ) );
	return 0;
}

int CTranslator::Visit( const CExpressionCallMethod* n )
{
	auto methodType = CTypeChecker::getMethodType( symbolTable, symbolTable->LookUpClass( currentClass->GetName() ),
		symbolTable->LookUpClass( currentClass->GetName() )->LookUpMethod( currentMethod->GetName() ), n->GetExpression() );
	n->GetExpression()->Accept( this );
	assert( lastValue != 0 );
	const IRTree::IExpression* object = lastValue->ToExp();

	expLists.push( 0 );

	if( n->GetExpList() != 0 )
		n->GetExpList()->Accept( this );
	
	IRTree::CExpList* currExpList = expLists.top();
	expLists.pop();

	lastValue = new CExpConverter( new IRTree::CCall( new IRTree::CName( new Temp::CLabel( makeLabelName( symbolTable->LookUpClass( methodType->GetUserType() ),
		symbolTable->LookUpClass( methodType->GetUserType() )->LookUpMethod( n->GetId() ) ) ) ), new IRTree::CExpList( object, currExpList ) ) );
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
	if ( currentMethodLocalVariables.count( n->GetId() ) )
	{
		lastValue = new CExpConverter( currentFrame->AllocLocal()
			->GetVar() );
	}
	else
	{
		if ( currentMethodArguments.count( n->GetId() ) )
		{
			lastValue = new CExpConverter( currentFrame->GetFormal( currentMethodArguments[n->GetId()] - 1 )
				->GetVar() );
		}
		else
		{
			const IRTree::IExpression* thisExpression = currentFrame->GetFormal( 0 )->GetVar();
			lastValue = new CExpConverter( new IRTree::CMem( new IRTree::CBinOp( IRTree::PLUS, thisExpression, 
				new IRTree::CConst( currentFrame->GetWordSize() * ( symbolTable->LookUpClass( currentClass->GetName() )
				->GetFieldOffset( n->GetId() ) ) ) ) ) );
		}
	}
	return 0;
}

int CTranslator::Visit( const CExpressionThis* n )
{
	lastValue = new CExpConverter( new IRTree::CTemp( currentFrame->GetThis() ) );
	return 0;
}

int CTranslator::Visit( const CExpressionNewInt* n )
{
	n->GetExpression()->Accept( this );
	assert( lastValue != 0 );
	lastValue = new CExpConverter( new IRTree::CBinOp( IRTree::PLUS, new IRTree::CConst( currentFrame->GetWordSize() ), currentFrame->ExternalCall( "allocateMemory",
		new IRTree::CExpList( new IRTree::CBinOp( IRTree::MUL, new IRTree::CConst( currentFrame->GetWordSize() ),
		new IRTree::CBinOp( IRTree::PLUS, lastValue->ToExp(), new IRTree::CConst( 1 ) ) ), 0 ) ) ) );
	return 0;
}

int CTranslator::Visit( const CExpressionNewId* n )
{
	lastValue = new CExpConverter( currentFrame->ExternalCall( "allocateMemory",
		new IRTree::CExpList( new IRTree::CConst( currentFrame->GetWordSize() *
		( symbolTable->LookUpClass( n->GetId() )->SizeOf() ) ), 0 ) ) );
	
	return 0;
}

int CTranslator::Visit( const CExpressionNegation* n )
{
	n->GetExpression()->Accept( this );
	assert( lastValue != 0 );
	const IRTree::IExpression* operand = lastValue->ToExp();
	lastValue = new CRelativeCmpConverter( IRTree::TCJump::EQ, operand, new IRTree::CConst( 0 ) );
	
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
	assert( lastValue != 0 );
	IRTree::CExpList* currExpList = expLists.top();
	expLists.top();
	currExpList = new IRTree::CExpList( lastValue->ToExp() , currExpList );
	expLists.push( currExpList );
	return 0;
}

int CTranslator::Visit( const CExpListNext* n )
{	
	if( n->GetExpList() != 0 ) n->GetExpList()->Accept( this );
	if( n->GetExpression() != 0 ) n->GetExpression()->Accept( this );
	assert( lastValue != 0 );
	IRTree::CExpList* currExpList = expLists.top();
	expLists.top();
	currExpList = new IRTree::CExpList( lastValue->ToExp() , currExpList );
	expLists.push( currExpList );
	return 0;
}
