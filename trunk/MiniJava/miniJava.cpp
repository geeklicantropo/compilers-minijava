#include <cstdio>
#include "miniJava.h"

CProgram::CProgram( const IMainClass* _mainClass, const IClassDeclareStar* _classDeclareStar ) :
	mainClass( _mainClass), classDeclareStar( _classDeclareStar )
	{
		assert( mainClass != 0 );
		assert( classDeclareStar != 0 );
	}

CMainClass::CMainClass( const char* _id, const char* _argsId, const IStatement* _stm ) :
	id( _id ), argsId( _argsId ), stm( _stm )
	{
		assert( id != 0 );
		assert( argsId != 0 );
		assert( stm != 0 );
	}

CClassDeclareStar::CClassDeclareStar(const IClassDeclareStar* classDecStar, const IClassDeclare* classDec ) :
	classDeclareStar( classDecStar ), classDeclare( classDec )
	{
		assert( classDeclare != 0 );
	}


CClassDeclare::CClassDeclare( const char* _id, const IVarDeclareStar* _varDeclareStar, const IMethodDeclareStar* _methodDeclareStar) :
	id( _id ), varDeclareStar( _varDeclareStar), methodDeclareStar( _methodDeclareStar )
	{
		assert( id != 0 );
		assert( varDeclareStar != 0 );
		assert( methodDeclareStar != 0 );
	}

CClassDeclareExtends::CClassDeclareExtends( const char* _id, const char* _extendsId, const IVarDeclareStar* _varDeclareStar, 
		const IMethodDeclareStar* _methodDeclareStar) :
	id( _id ), extendsId( _extendsId ), varDeclareStar( _varDeclareStar), methodDeclareStar( _methodDeclareStar )
	{
		assert( id != 0 );
		assert( extendsId != 0 );
		assert( varDeclareStar != 0 );
		assert( methodDeclareStar != 0 );
	}

CVarDeclareStar::CVarDeclareStar( const IVarDeclareStar* varDecStar, const IVarDeclare* varDec ) :
		varDeclareStar( varDecStar ), varDeclare( varDec ) 
	{
		assert( varDeclare != 0);
	}

CVarDeclare::CVarDeclare( const TDataTypes _dataType, const char* _id ) :
		dataType (_dataType ), id( _id )
	{
		assert( dataType != 0 );
		assert( id != _id );
	}

CMethodDeclareStar::CMethodDeclareStar( const IMethodDeclareStar* methodDecStar, const IMethodDeclare* methodDec ) :
	methodDeclareStar( methodDecStar ), methodDeclare( methodDec )
	{
		assert( methodDeclare != 0 );
	}

CMethodDeclare::CMethodDeclare( const TDataTypes _dataType, const char* _id, const IFormalList* _formalList, const IVarDeclareStar* varDecStar, 
		const IStatementStar* _statementStar, const IExpression* _expression ) :
	dataType( _dataType ), id( _id ), formalList( _formalList ), varDeclareStar( varDecStar ), statementStar( _statementStar ),
	expression( _expression )
	{
		assert( dataType != 0 );
		assert( id != 0 );
		assert( formalList != 0 );
		assert( varDeclareStar != 0 );
		assert( statementStar != 0 );
		assert( expression != 0 );
	}

CFormalList::CFormalList( const TDataTypes _dataType, const char* _id, const IFormalRestStar* _formalRestStar ) :
	dataType( _dataType ), id( _id ), formalRestStar( _formalRestStar )
	{
		assert( dataType != 0 );
		assert( id != 0 );
		assert( formalRestStar != 0 );
	}

CFormalRestStar::CFormalRestStar( const TDataTypes _dataType, const char* _id, const IFormalRestStar* _formalRestStar ) :
	dataType( _dataType ), id( _id ), formalRestStar( _formalRestStar )
	{
		assert( dataType != 0 );
		assert( id != 0 );
	}

CStatementStar::CStatementStar(const IStatement* _statement, const IStatementStar* _statementStar) :
	statement( _statement ), statementStar( _statementStar )
	{
		assert( statement != 0 );
	}

CStatement::CStatement( const IStatementStar* _statementStar ) :
	statementStar( _statementStar )
	{
		assert( statementStar != 0 );
	}

CStatementIf::CStatementIf( const IExpression* _expression, const IStatement* _statementIf, const IStatement* _statementElse ) :
	expression( _expression ), statementIf( _statementIf ), statementElse( _statementElse )
	{
		assert( expression != 0 );
		assert( statementIf != 0 );
		assert( statementElse != 0 );
	}

CStatementWhile::CStatementWhile( const IExpression* _expression, const IStatement* _statement ) :
	expression( _expression ), statement( _statement )
	{
		assert( expression != 0 );
		assert( statement != 0 );
	}

CStatementSysOut::CStatementSysOut( const IExpression* _expression ) :
	expression( _expression )
	{
		assert( expression != 0 );
	}

CStatementAssignment::CStatementAssignment( const char* _id, const IExpression* _expression ) :
	id( _id ), expression( _expression )
	{
		assert( id != 0 );
		assert( expression != 0 );
	}

CStatementArrayAssignment::CStatementArrayAssignment( const char* _id, const IExpression* _expressionArray, const IExpression* _expression ) :
	id( _id ), expressionArray( _expressionArray ), expression( _expression )
	{
		assert( id != 0 );
		assert( expressionArray != 0 );
		assert( expression != 0 );
	}

CExpressionBinOp::CExpressionBinOp( const IExpression* _expressionFirst, const TBinaryOperation _operation, const IExpression* _expressionSecond ) :
	expressionFirst( _expressionFirst ), operation( _operation ), expressionSecond( _expressionSecond )
	{
		assert( expressionFirst != 0 );
		assert( operation != 0 );
		assert( expressionSecond != 0 );
	}

CExpressionArray::CExpressionArray( const IExpression* _expression1, const IExpression* _expression2 ) :
	expression1( _expression1 ), expression2( _expression2 )
	{
		assert( expression1 != 0 );
		assert( expression2 != 0 );
	}

CExpressionLength::CExpressionLength( const IExpression* _expression ) :
	expression( _expression )
	{
		assert( expression != 0 );
	}

CExpressionCallMethod::CExpressionCallMethod( const IExpression* _expression, const char* _id, const IExpList* _expList ) :
	expression( _expression ), id( _id ), expList( _expList )
	{
		assert( expression != 0 );
		assert( id != 0 );
		assert( expList != 0 );
	}

CExpressionNumber::CExpressionNumber( const int _number ) : number( _number ) {}

CExpressionBool::CExpressionBool( const bool _value ) : value( _value ) {}

CExpressionVar::CExpressionVar( const char* _id ) : id( _id )
	{
		assert( id != 0 );
	}

CExpressionThis::CExpressionThis() {}

CExpressionNewInt::CExpressionNewInt( const IExpression* _expression ) :
		expression( _expression ) {}

CExpressionNewId::CExpressionNewId( const char* _id ) : id( _id ) {}

CExpressionNegation::CExpressionNegation( const IExpression* _expression ) : expression( _expression ) {}

CExpression::CExpression( const IExpression* _expression ) : expression( _expression ) {}

CExpList::CExpList( const IExpression* _expression ) :
		expression( _expression ) {}

CExpListNext::CExpListNext( const IExpList* _expList, const IExpression* _expression ) :
		expList( _expList ), expression( _expression ) {}