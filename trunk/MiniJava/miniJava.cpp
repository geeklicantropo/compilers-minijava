#include <cstdio>
#include "miniJava.h"

class CProgram : public IProgram {
public:
	CProgram( const IMainClass* _mainClass, const IClassDeclareStar* _classDeclareStar ) :
	mainClass( _mainClass), classDeclareStar( _classDeclareStar )
	{
		assert( mainClass != 0 );
		assert( classDeclareStar != 0 );
	}

private:
	const IMainClass* const mainClass;
	const IClassDeclareStar* const classDeclareStar;
};

class CMainClass : public IMainClass {
public:
	CMainClass( const char* _id, const char* _argsId, const IStatement* _stm ) :
		id( _id ), argsId( _argsId ), stm( _stm )
	{
		assert( id != 0 );
		assert( argsId != 0 );
		assert( stm != 0 );
	};
private:
	const char* const id;
	const char* const argsId;
	const IStatement* const stm;
};

class CClassDeclareStar : public IClassDeclareStar {
public:
	CClassDeclareStar(const IClassDeclareStar* classDecStar, const IClassDeclare* classDec ) :
	classDeclareStar( classDecStar ), classDeclare( classDec )
	{
		assert( classDeclare != 0 );
	}

private:
	const IClassDeclareStar* const classDeclareStar;
	const IClassDeclare* const classDeclare;
};

class CClassDeclare : public IClassDeclare {
public:
	CClassDeclare( const char* _id, const IVarDeclareStar* _varDeclareStar, const IMethodDeclareStar* _methodDeclareStar) :
		id( _id ), varDeclareStar( _varDeclareStar), methodDeclareStar( _methodDeclareStar )
	{
		assert( id != 0 );
		assert( varDeclareStar != 0 );
		assert( methodDeclareStar != 0 );
	}

private:
	const char* const id;
	const IVarDeclareStar* const varDeclareStar;
	const IMethodDeclareStar* const methodDeclareStar;
};

class CClassDeclareExtends : public IClassDeclare {
public:
	CClassDeclareExtends( const char* _id, const char* _extendsId, const IVarDeclareStar* _varDeclareStar, 
		const IMethodDeclareStar* _methodDeclareStar) :
		id( _id ), extendsId( _extendsId ), varDeclareStar( _varDeclareStar), methodDeclareStar( _methodDeclareStar )
	{
		assert( id != 0 );
		assert( extendsId != 0 );
		assert( varDeclareStar != 0 );
		assert( methodDeclareStar != 0 );
	}
private:
	const char* const id;
	const char* const extendsId;
	const IVarDeclareStar* const varDeclareStar;
	const IMethodDeclareStar* const methodDeclareStar;
};

class CVarDeclareStar : public IVarDeclareStar {
public:
	CVarDeclareStar( const IVarDeclareStar* varDecStar, const IVarDeclare* varDec ) :
		varDeclareStar( varDecStar ), varDeclare( varDec ) 
	{
		assert( varDeclare != 0);
	}
private:
	const IVarDeclareStar* const varDeclareStar;
	const IVarDeclare* const varDeclare;
};

class CVarDeclare : public IVarDeclare {
public:
	CVarDeclare( const TDataTypes* _dataType, const char* _id ) :
		dataType (_dataType ), id( _id )
	{
		assert( dataType != 0 );
		assert( id != _id );
	}

private:
	const TDataTypes* const dataType;
	const char* const id;
};

class CMethodDeclareStar : public IMethodDeclareStar {
public:
	CMethodDeclareStar( const IMethodDeclareStar* methodDecStar, const IMethodDeclare* methodDec ) :
		methodDeclareStar( methodDecStar ), methodDeclare( methodDec )
	{
		assert( methodDeclare != 0 );
	}

private:
	const IMethodDeclareStar* const methodDeclareStar;
	const IMethodDeclare* const methodDeclare;
};

class CMethodDeclare : IMethodDeclare {
public:
	CMethodDeclare( const TDataTypes* _dataType, const char* _id, const IFormalList* _formalList, const IVarDeclareStar* varDecStar, 
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
private:
	const TDataTypes* const dataType;
	const char* const id;
	const IFormalList* const formalList;
	const IVarDeclareStar* const varDeclareStar;
	const IStatementStar* const statementStar;
	const IExpression* const expression;
};

class CFormalList : public IFormalList {
public:
	CFormalList( const TDataTypes* _dataType, const char* _id, const IFormalRestStar* _formalRestStar ) :
		dataType( _dataType ), id( _id ), formalRestStar( _formalRestStar )
	{
		assert( dataType != 0 );
		assert( id != 0 );
		assert( formalRestStar != 0 );
	}
private:
	const TDataTypes* const dataType;
	const char* const id;
	const IFormalRestStar* const formalRestStar;
};

class CFormalRestStar : public IFormalRestStar {
public:
	CFormalRestStar( const TDataTypes* _dataType, const char* _id, const IFormalRestStar* _formalRestStar ) :
		dataType( _dataType ), id( _id ), formalRestStar( _formalRestStar )
	{
		assert( dataType != 0 );
		assert( id != 0 );
	}
private:
	const TDataTypes* const dataType;
	const char* const id;
	const IFormalRestStar* const formalRestStar;
};

class CStatementStar : public IStatementStar {
public:
	CStatementStar(const IStatement* _statement, const IStatementStar* _statementStar) :
		statement( _statement ), statementStar( _statementStar )
	{
		assert( statement != 0 );
	}
private:
	const IStatementStar* const statementStar;
	const IStatement* const statement;
};

class CStatement : public IStatement {
public:
	CStatement( const IStatementStar* _statementStar ) :
		statementStar( _statementStar )
	{
		assert( statementStar != 0 );
	}
private:
	const IStatementStar* const statementStar;
};

class CStatementIf : public IStatement {
public:
	CStatementIf( const IExpression* _expression, const IStatement* _statementIf, const IStatement* _statementElse ) :
		expression( _expression ), statementIf( _statementIf ), statementElse( _statementElse )
	{
		assert( expression != 0 );
		assert( statementIf != 0 );
		assert( statementElse != 0 );
	}
private:
	const IExpression* const expression;
	const IStatement* const statementIf;
	const IStatement* const statementElse;
};

class CStatementWhile : public IStatement {
public:
	CStatementWhile( const IExpression* _expression, const IStatement* _statement ) :
		expression( _expression ), statement( _statement )
	{
		assert( expression != 0 );
		assert( statement != 0 );
	}
private:
	const IExpression* const expression;
	const IStatement* const statement;
};

class CStatementSysOut : public IStatement {
public:
	CStatementSysOut( const IExpression* _expression ) :
		expression( _expression )
	{
		assert( expression != 0 );
	}
private:
	const IExpression* const expression;
};

class CStatementAssignment : public IStatement {
public:
	CStatementAssignment( const char* _id, const IExpression* _expression ) :
		id( _id ), expression( _expression )
	{
		assert( id != 0 );
		assert( expression != 0 );
	}

private:
	const char* const id;
	const IExpression* const expression;
};

class CStatementArrayAssignment : public IStatement {
public:
	CStatementArrayAssignment( const char* _id, const IExpression* _expressionArray, const IExpression* _expression ) :
		id( _id ), expressionArray( _expressionArray ), expression( _expression )
	{
		assert( id != 0 );
		assert( expressionArray != 0 );
		assert( expression != 0 );
	}

private:
	const char* const id;
	const IExpression* const expressionArray;
	const IExpression* const expression;
};

class CExpressionBinOp : public IExpression {
public:
	CExpressionBinOp( const IExpression* _expressionFirst, const TBinaryOperation* _operation, const IExpression* _expressionSecond ) :
		expressionFirst( _expressionFirst ), operation( _operation ), expressionSecond( _expressionSecond )
	{
		assert( expressionFirst != 0 );
		assert( operation != 0 );
		assert( expressionSecond != 0 );
	}
private:
	const IExpression* const expressionFirst;
	const TBinaryOperation* const operation;
	const IExpression* const expressionSecond;
};

class CExpressionArray : public IExpression {
public:
	CExpressionArray( const IExpression* _expression1, const IExpression* _expression2 ) :
		expression1( _expression1 ), expression2( _expression2 )
	{
		assert( expression1 != 0 );
		assert( expression2 != 0 );
	}
private:
	const IExpression* const expression1;
	const IExpression* const expression2;
};

class CExpressionLength : public IExpression {
public:
	CExpressionLength( const IExpression* _expression ) :
		expression( _expression )
	{
		assert( expression != 0 );
	}
private:
	const IExpression* const expression;
};

class CExpressionCallMethod : public IExpression {
public:
	CExpressionCallMethod( const IExpression* _expression, const char* _id, const IExpList* _expList ) :
		expression( _expression ), id( _id ), expList( _expList )
	{
		assert( expression != 0 );
		assert( id != 0 );
		assert( expList != 0 );
	}
private:
	const IExpression* const expression;
	const char* const id;
	const IExpList* const expList;
};

class CExpressionNumber : public IExpression {
public:
	CExpressionNumber( const int _number ) : number( _number ) {}
private:
	const int const number;
};

class CExpressionBool : public IExpression {
public:
	CExpressionBool( const bool _value ) : value( _value )
	{}
private:
	const bool const value;
};

class CExpressionVar : public IExpression {
public:
	CExpressionVar( const char* _id ) : id( _id )
	{
		assert( id != 0 );
	}
private:
	const char* const id;
};

class CExpressionThis : public IExpression {
public:
	CExpressionThis();
private:
};

class CExpressionNewInt : public IExpression {
public:
	CExpressionNewInt( const IExpression* _expression ) :
		expression( _expression ) {}
private:
	const IExpression* const expression;
};

class CExpressionNewId : public IExpression {
public:
	CExpressionNewId( const char* _id ) : id( _id ) {}
private:
	const char* const id;
};

class CExpressionNegation : public IExpression
{
public:
	CExpressionNegation( const IExpression* _expression ) : expression( _expression ) {}
private:
	const IExpression* const expression;
};

class CExpression : public IExpression {
public:
	CExpression( const IExpression* _expression ) : expression( _expression ) {}
private:
	const IExpression* const expression;
};

class CExpList : IExpList {
public:
	CExpList( const IExpression* _expression ) :
		expression( _expression ) {}
private:
	const IExpression* const expression;
};

class CExpListNext : IExpList {
public:
	CExpListNext( const IExpList* _expList, const IExpression* _expression ) :
		expList( _expList ), expression( _expression ) {}
private:
	const IExpression* const expression;
	const IExpList* const expList;
};